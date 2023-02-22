# -----------------------------------------------------------------------------------------------------
# Main script to automate the execution of simulations
#
# Parameters are defined in simParam.py
# Functions are defined in simUtils.py
# -----------------------------------------------------------------------------------------------------

import simParam
import simUtils
import itertools
import csv
import os
import glob

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
    numTestScenario = 0

    # Open csv and write header to write results on
    csvFile = open("output/simulationResults.csv", "a", encoding='UTF8', newline='')
    
    try:
        writer = csv.writer(csvFile)
        writer.writerow(["ID", "scenario", "seed", "Controller", "numPlatCars",
                        "numHumanCars", "spdLeader", "BeaconIntervalPlat", "BeaconIntervalHuman", "DENMInterval", "DecelerationRate",
                         "PacketSize", "Ctr_Param(Ploegh||Spacing)", "Collision", "TravelDistance", "TravelTime", "TTC", "MinGap"])
        csvFile.close()
        simUtils.changeValue(
            simParam.CONTROLLER_SCN, simParam.CONTROLLER_LINE, simParam.CONTROLLER_STR)

        ctr_scn, ctr_line, ctr_str = simUtils.getControllerScn(
            simParam.CONTROLLER_SCN)

        # Get all different combinations of the scenarios of input parameters
        scenarios = list(itertools.product(simParam.NUMPLATCARS_SCN,
                         simParam.NUMHUMANCARS_SCN, simParam.SPDLEADER_SCN, simParam.BEACINT_SCN,
                                           simParam.BEACINTHUMAN_SCN, simParam.DECRATE_SCN, simParam.PACKETSIZE_SCN,
                                           ctr_scn))

        # Iterate among each specific scenario
        for (numCars, numHumanCars, spdLeadCar, beacint, beacinthum, decrate, packsize, ctr) in scenarios:
            collision = False
            numTestScenario += 1

            # Adapt omnetpp.ini to specific scenario parameters.
            simUtils.changeValue(
                numCars, simParam.NUMCARS_LINE, simParam.NUMCARS_STR)
            simUtils.changeValue(
                numCars, simParam.NUMCARPLAT_LINE, simParam.NUMCARPLAT_STR)
            simUtils.changeValue(
                numHumanCars, simParam.NUMHUMANCAR_LINE, simParam.NUMHUMANCAR_STR)
            simUtils.changeValue(
                spdLeadCar, simParam.SPDLEADER_LINE, simParam.SPDLEADER_STR)
            simUtils.changeValue(
                beacint, simParam.BEACONINT_LINE, simParam.BEACONINT_STR)
            simUtils.changeValue(
                beacint, simParam.DENMINT_LINE, simParam.DENMINT_STR)
            simUtils.changeValue(
                beacinthum, simParam.BEACONINTHUMAN_LINE, simParam.BEACONINTHUMAN_STR)
            simUtils.changeValue(
                decrate, simParam.DECELERATION_LINE, simParam.DECELERATION_STR)
            simUtils.changeValue(
                packsize, simParam.PACKETSIZE_LINE, simParam.PACKETSIZE_STR)
            simUtils.changeValue(
                ctr, ctr_line, ctr_str)

            # run simulations with different seed if needed
            for seed in range(1):

                simUtils.runSimulation(seed)
                numTest += 1
                vector_file = glob.glob("../results/*{}*#{}.vec".format(CONFIGURATION, seed))
                # Parse vector file into a csv and check for collision
                collision, travel_distance, travel_time, ttc, min_gap = simUtils.createResultCSV(vector_file[0], simParam.parse_file.format(simParam.CONTROLLER_SCN, numTest, numTestScenario, seed), BRAKE_TIME, numCars)

                os.system('rm -rf ../results/*')

                # Save results into csv
                csvFile = open("output/simulationResults.csv", "a", encoding='UTF8', newline='')
                writer = csv.writer(csvFile)
                writer.writerow([numTest, numTestScenario, seed, simParam.CONTROLLER_SCN, numCars, numHumanCars,
                 spdLeadCar, beacint, beacinthum, beacint, decrate, packsize, ctr, collision, travel_distance, travel_time, ttc, min_gap])
                csvFile.close()

    finally:
        csvFile.close()

if __name__ == "__main__":
    main()
