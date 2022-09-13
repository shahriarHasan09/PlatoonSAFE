# Main script for automate the execution of simulations to find optimal wait time in different scenarios
# It must be copied and run in the file where omnetpp.ini is (src/plexe-veins-plexe-2.1/examples/safetyApps/)

import simParam, simUtils
import itertools
import random
import csv
import processResult



#"SB-nCars=7,platoonSize=7,nLanes=1,humanCars=10,humanLanes=3,caccSpacing=5,ploegH=0.5,controller=0,sController=ACC,headway=1.2,leaderHeadway=1.2,leaderSpeed=100,2,1.2-#0.vec"

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
    csvFile = open("output/simulationResults.csv", "w", encoding='UTF8', newline='')
    writer = csv.writer(csvFile)
    writer.writerow(["ID", "scenario", "numCars","numHumanCars","spdLeader","waitTime","Collision"])

    # Get all different combinations of the scenarios of input parameters
    scenarios = list(itertools.product(simParam.NUMCARS_SCN, simParam.NUMHUMANCARS_SCN, simParam.SPDLEADER_SCN))

    # Iterate among each specific scenario
    for (numCars, numHumanCars, spdLeadCar) in scenarios:
        # Redefine initial state (waitTime, limits, tries)
        waitTimeTop = simParam.WTMAX
        waitTimeBot = simParam.WTMIN
        waitTimeValue = waitTimeTop
        waitTimeTries = 0
        collision = False
        numTestScenario += 1

        # Adapt omnetpp.ini to specific scenario parameters.
        simUtils.changeValue(numCars, simParam.NUMCARS_LINE, simParam.NUMCARS_STR)
        simUtils.changeValue(numCars, simParam.NUMCARPLAT_LINE, simParam.NUMCARPLAT_STR)
        simUtils.changeValue(numHumanCars, simParam.NUMHUMANCAR_LINE, simParam.NUMHUMANCAR_STR)
        simUtils.changeValue(spdLeadCar, simParam.SPDLEADER_LINE, simParam.SPDLEADER_STR)
        
        """ 
        TODO: 
            - Take into account more output parameters (breakDistance, breakTime...) TOBEDEFINED
            - What if in MAXTRIES there is still collision?
        """
        for waitTimeTries in range(simParam.WT_MAXTRIES):
        # while (waitTimeTries < simParam.WT_MAXTRIES) or (collision):
            
            waitTimeValue, waitTimeTop, waitTimeBot = simUtils.selectWaitTime(waitTimeValue, waitTimeTop, waitTimeBot, collision)

            simUtils.changeValue(waitTimeValue, simParam.WAITTIME_LINE, simParam.WAITTIME_STR)
            
            simUtils.runSimulation()
            
            #waitTimeTries += 1
            numTest += 1
            """""""""""""""""""""""""""""""""""""""""
            TODO process results file to get outputs.
            For testing purpose, collision will be random
            """""""""""""""""""""""""""""""""""""""""
            collision = processResult.createResultCSV(simParam.vector_file.format(numCars, numCars, numHumanCars, spdLeadCar), simParam.parse_file.format(numTestScenario, waitTimeTries))
            """"""""""""""""""""""""""""""""""""""""""

            #collision = bool(random.getrandbits(1))
            
            # Save results into csv
            writer.writerow([numTest, numTestScenario, numCars, numHumanCars,spdLeadCar, round(waitTimeValue, 3), collision])
            
            print("Scenario:\n")
            print("*********************************\n")
            print("Platoon Size: %d\n" % numCars)
            print("Human cars: %d\n" % numHumanCars)
            print("Speed: %d\n" % spdLeadCar)
            print("*********************************\n")
            print("With wait time: %.3f\n" % waitTimeValue)
            print("collision = %s\n" % collision)
            print("*********************************\n")

    csvFile.close()


if __name__ == "__main__":
    main()
