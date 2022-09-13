import simParam

numCars = "**.numberOfCars "
numCarPlat = "**.numberOfCarsPerPlatoon "
numLanes = "**.numberOfLanes ="
numHumCar = "**.numberOfHumanCars "
numHumLane = "**.numberOfHumanLanes = "
caccSpacing = "*.node[*].scenario.caccSpacing = "
ploegh = "*.node[*].scenario.ploegH = "
controller = "*.node[*].scenario.controller = "
spdLeader = "*.node[*].scenario.leaderSpeed "
beaconInt = "*.node[*].prot.beaconingInterval = "
packetSize = "*.node[*].prot.packetSize = "
beaconHuman = "*.human[*].prot.beaconingInterval ="
humanTime = "**.traffic.humanInsertTime ="
deceleration = "*.node[*].prot.decelarationRate ="
brakeTime ="*.node[*].prot.brakeAtTime ="
twait = "*.node[*].prot.sbWaitTime ="
denmInt = "*.node[*].prot.DENMInterval"

# opening a text file
file1 = open("CEBModule.ini", "r")
  
# setting flag and index to 0
flag = 0
index = 0
  
# Loop through the file line by line
for line in file1:  
    index += 1 
      
    # checking string is present in line or not
    if numCars in line:
        print("numCars found in line: ", index)
    elif numCarPlat in line:
        print("numCarPlat found in line: ", index)
    elif numLanes in line:
        print("numLanes found in line: ", index)
    elif numHumCar in line:
        print("numHumCar found in line: ", index)
    elif numHumLane in line:
        print("numHumLane found in line: ", index)
    elif caccSpacing in line:
        print("caccSpacing found in line: ", index)
    elif ploegh in line:
        print("ploegh found in line: ", index)
    elif controller in line:
        print("controller found in line: ", index)
    elif spdLeader in line:
        print("spdLeader found in line: ", index)
    elif beaconInt in line:
        print("beaconInt found in line: ", index)
    elif packetSize in line:
        print("packetSize found in line: ", index)
    elif beaconHuman in line:
        print("beaconHuman found in line: ", index)
    elif humanTime in line:
        print("humanTime found in line: ", index)
    elif deceleration in line:
        print("deceleration found in line: ", index)
    elif brakeTime in line:
        print("brakeTime found in line: ", index)
    elif twait in line:
        print("twait found in line: ", index)
    elif denmInt in line:
        print("brakeTime found in line: ", index)

          
  
  
# closing text file    
file1.close() 