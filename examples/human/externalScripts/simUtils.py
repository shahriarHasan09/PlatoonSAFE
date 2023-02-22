# -----------------------------------------------------------------------------------------------------
# This file contains all necessary functions to execute runSimulations.py
# -----------------------------------------------------------------------------------------------------

import subprocess
import sys
import simParam
import re
import time


def changeValue(value, line, string, file="../RTM-CEB-ML.ini"):
    """
    Change value of specific parameter in omnetpp.ini file.
    It needs to change the complete line where the value is defined.

    Parameters
    ----------
    value : integer, float, str (any type)
        The new value that will be inserted
    
    line : integer
        The line number in the omnetpp.ini file where the value is defined.
        This is done to make the changes faster, instead of finding where the value is located.

    string : str
        The default string which has to be changed. The string MUST have a formatter included (specified in simParam.py)
    """
    list_of_lines = open(file, "r").readlines()
    list_of_lines[line] = string.format(value)

    a_file = open(file, "w")
    a_file.writelines(list_of_lines)
    a_file.close()


def runSimulation(seed):
    """
    Run command in bash that executes the simulation.
    Parameters of the command are "static", but they can be modified in simParam.py before execution
    """
    try:
        print("Running with command line arguments: %s" % ' '.join([simParam.SUMOCMD]))
        
        timeStarted = time.time()
        retcode = subprocess.call(simParam.SUMOCMD.format(seed), shell = True, cwd="../")
        timeDelta = time.time() - timeStarted 
        if retcode < 0:
            print("Child was terminated by signal", -retcode, file=sys.stderr)
        else:
            print("Child returned", retcode, file=sys.stderr)
        return timeDelta
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)

def getControllerScn(controller):
    if controller == "ACC":
        return simParam.ACC_SCN, simParam.PLOEGH_LINE, simParam.PLOEGH_STR
    elif controller == "PLOEG":
        return simParam.PLOEG_SCN, simParam.PLOEGH_LINE, simParam.PLOEGH_STR
    elif controller == "CACC":
        return simParam.CACC_SCN, simParam.CACCSPACING_LINE, simParam.CACCSPACING_STR
    return []

def createResultCSV(vector_file, parse_file, start_brake_time = 100, platoon_size = 7):
    collision = False
    vectors_parsed = False
    dictionary = {}
    speed_0 = [False for i in range(platoon_size)]
    start_distance = None
    stop_distance = None
    travel_distance = 0
    with open(vector_file, "r") as a_file:
        with open(parse_file, "w+") as output_file:
            output_file.writelines('"Variable", "Node", "Time", "Value"\n')
            for line in a_file:
                a = line.split()
                if len(a) > 0:
                    # Save all vectors as new entries
                    if a[0] == "vector":
                        vec_num = int(a[1])
                        var_name = a[3]
                        node = int(re.search(r"\d+", a[2]).group())
                        dictionary[vec_num] = [var_name,node]
                        vectors_parsed = True
                    # If the vector has already an entry, start saving its values
                    elif vectors_parsed:
                        if vec_num != int(a[0]):
                            vec_num = int(a[0])
                            var_name, node = dictionary[vec_num]
                        sim_time = float(a[2])
                        value = float(a[3])
                        # If the distance between any platoon vehicles gets to 0 or lower, there has been a collision
                        if var_name == "distance" and value <= 0 and node != 0:
                            collision = True
                        # Get time when all vehicles have stopped
                        if var_name == "speed" and value < 0.01 and speed_0[node] != True:
                            speed_0[node] = True
                            if all(speed_0) == True:
                                stop_time = sim_time
                        # Get the distance of the LV when it starts breaking and the position that it finishes the simulation
                        if var_name == "posx" and node == 0 and sim_time >= start_brake_time:
                            if start_distance == None:
                                start_distance = value
                            stop_distance = value
                        
                        out_line = f"{var_name}, {node}, {sim_time}, {value}\n"
                        output_file.writelines(out_line)

    travel_distance = stop_distance - start_distance
    travel_time = stop_time - start_brake_time
    return collision, travel_distance, travel_time