# -----------------------------------------------------------------------------------------------------
# This file contains all the static definitions:
# 1- Definitions for adaptable input parameters that should be changed in omnetpp.ini
# 2- Waiting Time selection algorithm parameters
# 3- Scenarios of each input parameter
# 4- Command definitions to run simulator in bash
# -----------------------------------------------------------------------------------------------------

""" 
To avoid searching for parameters in the file, it is easier to define the line where they are (-1 for 0 padding) and the string to be inserted.
In the string, the format of the value to be changed shall be defined. 
Examples:
   - String -> {:s}
   - Integer <- {:d}
   - Float -> {:f}
   - Float with only two decimals -> {:.2f}
"""
NUMCARS_LINE = 113 - 1
NUMCARS_STR = "**.numberOfCars = ${{nCars = {:d}}}\n"

NUMCARPLAT_LINE = 114 - 1
NUMCARPLAT_STR = "**.numberOfCarsPerPlatoon = ${{platoonSize = {:d}}}\n"

NUMLANES_LANE = 115 - 1
NUMLANES_STR = "**.numberOfLanes = ${{nLanes = {:d}}}\n"

NUMHUMANCAR_LINE = 116 - 1
NUMHUMANCAR_STR = "**.numberOfHumanCars = ${{humanCars = {:d}}}\n"

NUMHUMLANES_LINE = 117 -1
NUMHUMANLANES_STR ="**.numberOfHumanLanes = ${{humanLanes = {:d}}}\n"

CACCSPACING_LINE = 133 -1
CACCSPACING_STR = "*.node[*].scenario.caccSpacing = ${{caccSpacing = {:.2f}}}m\n"

PLOEGH_LINE = 138 -1
PLOEGH_STR = "*.node[*].scenario.ploegH = ${{ploegH = {:.2f}}}s\n"

CONTROLLER_LINE = 144 - 1
CONTROLLER_STR = '*.node[*].scenario.controller = "{:s}"\n'

SPDLEADER_LINE = 152 - 1
SPDLEADER_STR = "*.node[*].scenario.leaderSpeed = ${{leaderSpeed = {:d}}}kmph\n"

BEACONINT_LINE = 173 -1
BEACONINT_STR = "*.node[*].prot.beaconingInterval = ${{beaconInterval = {:.2f}}}s\n"

PACKETSIZE_LINE = 177 - 1
PACKETSIZE_STR = "*.node[*].prot.packetSize = ${{packetSize = {:d}}}\n"

BEACONINTHUMAN_LINE = 187 - 1
BEACONINTHUMAN_STR= "*.human[*].prot.beaconingInterval = {:.2f} s\n"

HUMANINSERT_LINE = 206 - 1
HUMANINSERT_STR = "**.traffic.humanInsertTime = {:d} s\n"

DECELERATION_LINE = 328 - 1
DECELERATION_STR = "*.node[*].prot.decelarationRate = {:d} mpsps\n"

BRAKETIME_LINE = 329 - 1
BRAKETIME_STR = "*.node[*].prot.brakeAtTime = {:d}s\n"

WAITTIME_LINE = 332 - 1
WAITTIME_STR = "*.node[*].prot.sbWaitTime = {:.5f}s\n"

DENMINT_LINE = 333 - 1
DENMINT_STR = "*.node[*].prot.DENMInterval = {:.3f}s\n"

GUI_LINE = 316 - 1
GUI_STR = '*.manager.useGui = {:s}\n'

"""
Definition of all input parameter different scenarios
"""
"""
NUMPLATCARS_SCN = [8,10,12]
NUMHUMANCARS_SCN = [300, 400, 500]
SPDLEADER_SCN = [100] #80
BEACINT_SCN = [0.1] #DENM interval should be equal
BEACINTHUMAN_SCN = [0.01, 0.02] 
DECRATE_SCN = [8]
PACKETSIZE_SCN = [500]
CONTROLLER_SCN = "CACC" #["CACC", "PLOEG", "ACC"]
CACC_SCN = [2, 3, 4, 5]
"""
NUMPLATCARS_SCN = [7,10]
NUMHUMANCARS_SCN = [10,50,100,200]
SPDLEADER_SCN = [100, 80]
BEACINT_SCN = [0.1] #DENM interval should be equal
BEACINTHUMAN_SCN = [0.01, 0.02] 
DECRATE_SCN = [8]
PACKETSIZE_SCN = [500, 1000]
CONTROLLER_SCN = "CACC" #["CACC", "PLOEG", "ACC"]
CACC_SCN = [2]
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
vector_file = "results/{:s}-{:d}.vec"
parse_file = "output/parse_c{:s}_t{:d}_s{:d}_r{:d}.csv"

"""
Command line execution. Adapt it as necessary.
Depending on the installation and location of OMNet++ and PlatoonSAFE project, directory locations may change.
To find the own directories, a simple way is to execute a simulation from OMNet++ for Cmdenv option (Change it in run configurations) and stop the simulation immediatly
A command similar to this one should be printed in OMNet++ terminal.
"""
prefix = "../../../../omnetpp-5.6.2/bin/"
opp_run = "opp_run{:s} "
lib_flags = "-l {:s} "
ned_flags = "-n {:s} "
img_flags = "--image-path={:s} "
omnetpp_args = "-m -u Cmdenv -c {:s} RTMModule.ini" #change configuration if necessary

SUMOCMD = opp_run.format("") + lib_flags.format("../../src/veins") + ned_flags.format(".:../veins:../../src/veins") + img_flags.format("../../images") + "-r {:d} " + omnetpp_args.format("RTM-SB")