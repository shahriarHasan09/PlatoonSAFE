# This file contains all necessary functions to execute runSimulations.py

import subprocess
import sys
import statistics as stats
import simParam

def changeValue(value, line, string, file="CEBModule.ini"):
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
        retcode = subprocess.call(simParam.SUMOCMD.format(seed), shell = True)
        if retcode < 0:
            print("Child was terminated by signal", -retcode, file=sys.stderr)
        else:
            print("Child returned", retcode, file=sys.stderr)
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)


def selectWaitTime(waitTimeValue, waitTimeTop, waitTimeBot, collision):
    """
    Wait Time Selection Algorithm: (CAN BE CHANGED)
        
        - Starts with upper and lower bound (waitTime limits).
        - waitTimeValue will be the mean of the bounds.
        - Run simulation and process results
        - If "good" results -> adapt bound to last value and lower bound
        - If "bad" results -> adapt bound to upper bound and last value

    """
    if collision:
        waitTimeBot = waitTimeValue
        waitTimeValue = stats.mean([waitTimeTop, waitTimeValue])
    else:
        waitTimeTop = waitTimeValue
        waitTimeValue = stats.mean([waitTimeValue, waitTimeBot])
    return waitTimeValue, waitTimeTop, waitTimeBot


def getControllerScn(controller):
    if controller == "ACC":
        return simParam.ACC_SCN, simParam.PLOEGH_LINE, simParam.PLOEGH_STR
    elif controller == "PLOEG":
        return simParam.PLOEG_SCN, simParam.PLOEGH_LINE, simParam.PLOEGH_STR
    elif controller == "CACC":
        return simParam.CACC_SCN, simParam.CACCSPACING_LINE, simParam.CACCSPACING_STR
    return []

def getWaitTimeLimits(numHumanCars, beacinthum):
    num = numHumanCars / beacinthum

    if num >= 15000:
        return simParam.WTMAX[0], simParam.WTMIN
    elif num >= 10000:
        return simParam.WTMAX[1], simParam.WTMIN
    elif num >= 7500:
        return simParam.WTMAX[2], simParam.WTMIN
    elif num >= 5000:
        return simParam.WTMAX[3], simParam.WTMIN
    elif num >= 3000:
        return simParam.WTMAX[4], simParam.WTMIN
    elif num >= 2500:
        return simParam.WTMAX[5], simParam.WTMIN
    elif num >= 2000:
        return simParam.WTMAX[6], simParam.WTMIN
    elif num >= 1000:
        return simParam.WTMAX[7], simParam.WTMIN
    elif num >= 500:
        return simParam.WTMAX[8], simParam.WTMIN
    else:
        return simParam.WTMAX[9], simParam.WTMIN