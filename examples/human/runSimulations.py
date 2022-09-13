# Main script for automate the execution of simulations to find optimal wait time in different scenarios
# It must be copied and run in the file where omnetpp.ini is (src/plexe-veins-plexe-2.1/examples/safetyApps/)

import simParam
import simUtils
import itertools
import csv
import processResult
import os


# "SB-nCars=7,platoonSize=7,nLanes=1,humanCars=10,humanLanes=3,caccSpacing=5,ploegH=0.5,controller=0,sController=ACC,headway=1.2,leaderHeadway=1.2,leaderSpeed=100,2,1.2-#0.vec"

def main():
    """
    Function to automate execution of simulations with the aim to find the best waitTime for each defined scenario. 

    Parameters (Defined in simParam)
    ----------
    WTMAX and WTMIN : integer
        Stablish limits for the search of optimal wt

    WT_MAXTRIES : integer
        Define how many runs should be done for each scenario.

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
                         "PacketSize", "Ctr_Param(Ploegh||Spacing)", "waitTime", "Collision"])
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
            # Redefine initial state (waitTime, limits, tries)
            waitTimeTop, waitTimeBot = simUtils.getWaitTimeLimits(numHumanCars, beacinthum)
            waitTimeValue = waitTimeTop
            waitTimeTries = 0
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

            """ 
			TODO: 
				- Take into account more output parameters (breakDistance, breakTime...) TOBEDEFINED
				- What if in MAXTRIES there is still collision?
			"""
            for waitTimeTries in range(simParam.WT_MAXTRIES):

                waitTimeValue, waitTimeTop, waitTimeBot = simUtils.selectWaitTime(
                    waitTimeValue, waitTimeTop, waitTimeBot, collision)
                waitTimeTop = waitTimeTop
                simUtils.changeValue(
                    waitTimeValue, simParam.WAITTIME_LINE, simParam.WAITTIME_STR)

                for seed in range(1):

                    simUtils.runSimulation(seed)

                    #waitTimeTries += 1
                    numTest += 1
                    """""""""""""""""""""""""""""""""""""""""
					TODO process results file to get outputs.
					For testing purpose, collision will be random
					"""""""""""""""""""""""""""""""""""""""""
                    collision = processResult.createResultCSV(simParam.vector_file.format(seed),
                     simParam.parse_file.format(simParam.CONTROLLER_SCN, numTest, numTestScenario, waitTimeTries, seed))
                    """"""""""""""""""""""""""""""""""""""""""
                    os.system('rm -rf results/*')

                    # Save results into csv

                    if collision:
                        break
                csvFile = open("output/simulationResults.csv", "a", encoding='UTF8', newline='')
                writer = csv.writer(csvFile)
                writer.writerow([numTest, numTestScenario, seed, simParam.CONTROLLER_SCN, numCars, numHumanCars,
                 spdLeadCar, beacint, beacinthum, beacint, decrate, packsize, ctr, round(waitTimeValue, 3), collision])
                csvFile.close()

    finally:
        csvFile.close()

if __name__ == "__main__":
    main()
