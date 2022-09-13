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
NUMCARS_LINE = 107 - 1
NUMCARS_STR = "**.numberOfCars = ${{nCars = {:d}}}\n"

NUMCARPLAT_LINE = 108 - 1
NUMCARPLAT_STR = "**.numberOfCarsPerPlatoon = ${{platoonSize = {:d}}}\n"

NUMLANES_LANE = 109 - 1
NUMLANES_STR = "**.numberOfLanes = ${{nLanes = {:d}}}\n"

NUMHUMANCAR_LINE = 110 - 1
NUMHUMANCAR_STR = "**.numberOfHumanCars = ${{humanCars = {:d}}}\n"

NUMHUMLANES_LINE = 111 -1
NUMHUMANLANES_STR ="**.numberOfHumanLanes = ${{humanLanes = {:d}}}\n"

CACCSPACING_LINE = 128 -1
CACCSPACING_STR = "*.node[*].scenario.caccSpacing = ${{caccSpacing = {:.2f}}}m\n"

PLOEGH_LINE = 134 -1
PLOEGH_STR = "*.node[*].scenario.ploegH = ${{ploegH = {:.2f}}}s\n"

CONTROLLER_LINE = 142 - 1
CONTROLLER_STR = '*.node[*].scenario.controller = "{:s}"\n'

SPDLEADER_LINE = 154 - 1
SPDLEADER_STR = "*.node[*].scenario.leaderSpeed = ${{leaderSpeed = {:d}}}kmph\n"

BEACONINT_LINE = 176 -1
BEACONINT_STR = "*.node[*].prot.beaconingInterval = ${{beaconInterval = {:.2f}}}s\n"

PACKETSIZE_LINE = 180 - 1
PACKETSIZE_STR = "*.node[*].prot.packetSize = ${{packetSize = {:d}}}\n"

BEACONINTHUMAN_LINE = 188 - 1
BEACONINTHUMAN_STR= "*.human[*].prot.beaconingInterval = {:.2f} s\n"

HUMANINSERT_LINE = 211 - 1
HUMANINSERT_STR = "**.traffic.humanInsertTime = {:d} s\n"

DECELERATION_LINE = 295 - 1
DECELERATION_STR = "*.node[*].prot.decelarationRate = {:d} mpsps\n"

BRAKETIME_LINE = 297 - 1
BRAKETIME_STR = "*.node[*].prot.brakeAtTime = {:d}s\n"

WAITTIME_LINE = 300 - 1
WAITTIME_STR = "*.node[*].prot.sbWaitTime = {:.5f}s\n"

DENMINT_LINE = 302 - 1
DENMINT_STR = "*.node[*].prot.DENMInterval = {:.3f}s\n"


"""
Definition of wait time maximum and minimum values and how many different wait times should be checked per scenario
"""
WTMAX = [0.3125, 0.25, 0.21875, 0.1875, 0.15625, 0.125, 0.0625, 0.0375, 0.0125, 0.00625]
WTMIN = 0.001
WT_MAXTRIES = 2

"""
Definition of all input parameter different scenarios
"""
NUMPLATCARS_SCN = [8,10,12]
NUMHUMANCARS_SCN = [300, 400, 500]
SPDLEADER_SCN = [100] #80
BEACINT_SCN = [0.1] #DENM interval should be equal
BEACINTHUMAN_SCN = [0.01, 0.02] 
DECRATE_SCN = [8]
PACKETSIZE_SCN = [500]
CONTROLLER_SCN = "PLOEG" #["CACC", "PLOEG", "ACC"]
CACC_SCN = [2, 3, 4, 5]
PLOEG_SCN = [0.2, 0.3, 0.4, 0.5]#[0.2, 0.3, 0.4, 0.5]
ACC_SCN = [0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2] #No need for such many vehicles, try only with 10 human cars. No need for vehicle int, packet size.
#ACC SPDLEADER_SCN = [120, 100, 80, 60]
#ACC DECRATE_SCN = [8, 7, 6, 5, 4, 2]
#ACC WTMAX = [2.5, 2, 1.75, 1.5, 1.25, 1, 0.5, 0.3, 0.1, 0.05] /8
#ACC NUMHUMANCARS_SCN = [10]
#ACC BEACINT_SCN = [0.05] #DENM interval should be equal
#ACC BEACINTHUMAN_SCN = [0.02]
##########################################
"""
Output file
"""
vector_file = "results/SBTest-{:d}.vec"
parse_file = "output/parse_c{:s}_t{:d}_s{:d}_wt{:d}_r{:d}.csv"
"""
Command line execution
"""
prefix = "../../../../omnetpp-5.6.2/bin/"
opp_run = "opp_run{:s} "
lib_flags = "-l {:s} "
ned_flags = "-n {:s} "
img_flags = "--image-path={:s} "
omnetpp_args = "-m -u Cmdenv -c {:s} CEBModule.ini"

#opp_run -r 0 -m -u Cmdenv -c SBTest -n .:../veins:../../src/veins --image-path=../../images -l ../../src/veins CEBModule.ini

SUMOCMD = opp_run.format("") + lib_flags.format("../../src/veins") + ned_flags.format(".:../veins:../../src/veins") + img_flags.format("../../images") + "-r {:d} " + omnetpp_args.format("SBTest")
