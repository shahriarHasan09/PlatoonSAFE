# This file contains all the static definitions:
# 1- Definitions for adaptable input parameters that should be changed in omnetpp.ini
# 2- Waiting Time selection algorithm parameters
# 3- Scenarios of each input parameter
# 4- Command definitions to run simulator in bash



""" 
To avoid searching for parameters in the file, it is easier to define the line where they are (-1 for 0 padding) and the string to be inserted.
In the string, the format of the value to be changed shall be defined. 
Examples:
   - String -> {:s}
   - Integer <- {:d}
   - Float -> {:f}
   - Float with only two decimals -> {:.2f}
"""
WAITTIME_LINE = 280 - 1
WAITTIME_STR = "*.node[*].prot.sbWaitTime = {:.3f}s\n"
NUMCARS_LINE = 106 - 1
NUMCARS_STR = "**.numberOfCars = ${{nCars = {:d}}}\n"
NUMCARPLAT_LINE = 107 - 1
NUMCARPLAT_STR = "**.numberOfCarsPerPlatoon = ${{platoonSize = {:d}}}\n"
NUMHUMANCAR_LINE = 109 - 1
NUMHUMANCAR_STR = "**.numberOfHumanCars = ${{humanCars = {:d}}}\n"
SPDLEADER_LINE = 146 - 1
SPDLEADER_STR = "*.node[*].scenario.leaderSpeed = ${{leaderSpeed = {:d}}}kmph\n"

"""
Definition of wait time maximum and minimum values and how many different wait times should be checked per scenario
"""
WTMAX = 1
WTMIN = 0.01
WT_MAXTRIES = 5

"""
Definition of all input parameter different scenarios
"""
NUMCARS_SCN = [3,7,11]
NUMHUMANCARS_SCN = [5,10,20]
SPDLEADER_SCN = [80, 100]

"""
Output file
"""
vector_file = "results/SB-nCars={:d},platoonSize={:d},nLanes=1,humanCars={:d},humanLanes=3,caccSpacing=5,ploegH=0.5,controller=0,sController=ACC,headway=1.2,leaderHeadway=1.2,leaderSpeed={:d},2,1.2-#0.vec"
parse_file = "output/parse{:d}{:d}.csv"
"""
Command line execution
"""
prefix = "../../../../omnetpp-5.6.2/bin/"
opp_run = "opp_run_{:s} "
lib_flags = "-l {:s} "
ned_flags = "-n {:s} "
img_flags = "--image-path={:s} "
omnetpp_args = "-r {:s} -m -u Cmdenv -c {:s} --record-eventlog=true --scalar-recording=true --vector-recording=true omnetpp.ini"

SUMOCMD = prefix + opp_run.format("dbg") + lib_flags.format("../../src/veins") + ned_flags.format("../human:../veins:../../src/veins") + img_flags.format("../../images") + omnetpp_args.format("0", "SB")
