//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_OUTPUT_FILEWRITER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_OUTPUT_FILEWRITER_H_

#include <iostream>
#include <fstream>
#include <string>

#include "../assumptions/WIFIAssumption.h"
#include "veins/modules/application/platooning/runtimeManager/RMUtility.h"


/**
 * This class is responsible to write to a txt file
 */
class FileWriter {
public:
    FileWriter(int vehileId, std::string outputFilename, bool &write2file);
    virtual ~FileWriter();
    void writeHeaders();
    void addEntries(const RM::RMParameters &rmParam, const RM::RMLog_Own &ego, bool distViolated = false , bool decelViolated = false);
private:
    int vehicleId;
    std::string outputFileName;
    std::fstream output;
    // Default value of variable
    // Headers
    std::string parameters = "PARAMETERS";
    std::string c2xQuality = "C2X-QUALITY";
    std::string acceleration = "ACCELERATION(mpsps)";
    std::string dist2predViolation = "DIST2PRED VIOLATION";
    std::string decelViolation = "DECELERATION VIOLATION";
    std::string dataRecordedTime = "TIME(s)";

    // parameters headers
    std::string activeMode = "ActiveMode";
    std::string beaconINR = "BeaconINR(s)";
    std::string monitorINR = "MonitorINR(s)";
    std::string PktlossPoor = "PktlossPoor";
    std::string PktLossCrit = "PktLossCrit";
    std::string accHTG = "accHTG(s)";
    std::string ploegHTG = "ploegHTG(s)";
    std::string caccSpac = "caccSpac(m)";
    std::string ploegFactor = "ploegFactor";
    std::string caccFactor = "caccFactor";

    // c2x
    std::string front = "Front";
    std::string leader = "Leader";

    // deceleration
    std::string currentAccel = "CurrentAccel";
    std::string maxDecel = "MaxDecel";

    // dist2pred violation
    std::string yesNo = "violated?";
    std::string dist2pred = "dist2pred(m)";

    // deceleration violation
    std::string decel = "decel(mpsps)";
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_OUTPUT_FILEWRITER_H_ */
