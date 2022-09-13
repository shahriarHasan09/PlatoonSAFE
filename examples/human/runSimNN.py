# Main script for automate the execution of simulations to find optimal wait time in different scenarios
# It must be copied and run in the file where omnetpp.ini is (src/plexe-veins-plexe-2.1/examples/safetyApps/)

import NNServerThread
import subprocess
import sys
import time

#SUMOCMD = "opp_run -r {:d} -m -u Cmdenv -c RTM-SB-NN -n .:../veins:../../src/veins --image-path=../../images -l ../../src/veins --record-eventlog=false --cmdenv-stop-batch-on-error=false RTMModule.ini"
SUMOCMD = "opp_run -m -u Qtenv -c RTM-SB-NN -n .:../veins:../../src/veins --image-path=../../images -l ../../src/veins RTMModule.ini"

def runSimulation(seed):
    """
    Run command in bash that executes the simulation.
    Parameters of the command are "static", but they can be modified in simParam.py before execution
    """
    try:
        retcode = subprocess.call(SUMOCMD.format(seed), shell = True)
        if retcode < 0:
            print("Child was terminated by signal", -retcode, file=sys.stderr)
        else:
            print("Child returned", retcode, file=sys.stderr)
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)

def main():
    
    runs = [0]
    # Iterate among each specific scenario
    for seed in runs:
        t1 = NNServerThread.thread_with_trace(target = NNServerThread.runNN)
        t1.start()
        time.sleep(5)
        print("GO TO SLEEP")
        runSimulation(seed)
        print("WAKE UP")
        time.sleep(10)
        t1.kill()
        t1.join()
        if not t1.is_alive():
            print('thread killed')
        print("END")



if __name__ == "__main__":
    main()
