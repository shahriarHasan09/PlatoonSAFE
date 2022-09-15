# -----------------------------------------------------------------------------------------------------
# Main script to connect a NN with PlatoonSAFE using UDP
# NN is defined in NNServerThread.py
# -----------------------------------------------------------------------------------------------------

import NNServerThread
import subprocess
import sys
import time

# If needed, change configuration (-c configName) and .ini file
# Depending on the installation and location of OMNet++ and PlatoonSAFE project, directory locations may change.
# To find the own directories, a simple way is to execute a simulation from OMNet++ for Cmdenv option (Change it in run configurations) and stop the simulation immediatly
# A command similar to this one should be printed in OMNet++ terminal.
SUMOCMD = "opp_run -r {:d} -m -u Cmdenv -c RTM-SB-NN -n .:../veins:../../src/veins --image-path=../../images -l ../../src/veins --record-eventlog=false --cmdenv-stop-batch-on-error=false RTMModule.ini"

def runSimulation(seed):
    """
    Run command in bash that executes the simulation.
    Parameters of the command are "static", but they can be modified in simParam.py before execution
    """
    try:
        retcode = subprocess.call(SUMOCMD.format(seed), shell = True, cwd="../")
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
        # Run NN thread
        t1 = NNServerThread.thread_with_trace(target = NNServerThread.runNN)
        t1.start()
        # Wait 5 seconds to ensure NN Thread is on
        time.sleep(5)
        # Run PlatoonSAFE simulation and wait till it finishes
        runSimulation(seed)
        # Sleep 5 second to ensure all files in the simulation are correctly saved
        time.sleep(5)

        t1.kill()
        t1.join()
        if not t1.is_alive():
            print('thread killed')
        print("END")

if __name__ == "__main__":
    main()
