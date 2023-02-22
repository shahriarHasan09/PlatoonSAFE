# -----------------------------------------------------------------------------------------------------
# Main script to identify Simulation speed. Same as runSimulations.py
#
# -----------------------------------------------------------------------------------------------------

import simParam
import simUtils
import csv
import os

CONFIGURATION = "RTM-SB"
BRAKE_TIME = 40 # Define brake time to compute braking time and travel distance

def main():
    """
    Function to automate execution of simulations with the aim to find the best waitTime for each defined scenario. 

    Parameters (Defined in simParam)
    ----------
    scenarios: array
        Define different scenarios for each input parameter 

    """
    # Variables to follow the count of the test
    numTest = 0
    numCars = [10,20,40,60,80,100,120,140,160,180,200]
    numPlatCars = 7
    guis = ["false", "true"]
    seed = 0
    # Open csv and write header to write results on
    csvFile = open("output/simTimes0.csv", "a", encoding='UTF8', newline='')
    
    try:
        writer = csv.writer(csvFile)
        writer.writerow(["ID", "numCars","simTime", "realTime", "gui"])
        csvFile.close()

        # Iterate among each specific scenario
        for gui in guis:
            simUtils.changeValue(
                    gui, simParam.GUI_LINE, simParam.GUI_STR)
            for cars in numCars:
                numHumanCars = cars - numPlatCars
                simUtils.changeValue(
                    cars, simParam.NUMCARS_LINE, simParam.NUMCARS_STR)

                realTime = simUtils.runSimulation(seed)
                os.system('rm -rf ../results/*.sca')
                os.system('rm -rf ../results/*.vci')
                os.system('rm -rf ../results/*.vec')
                # Save results into csv
                csvFile = open("output/simTimes0.csv", "a", encoding='UTF8', newline='')
                writer = csv.writer(csvFile)
                writer.writerow([numTest, cars, 0, realTime, gui])
                csvFile.close()
                numTest += 1
    finally:
        csvFile.close()

if __name__ == "__main__":
    main()
