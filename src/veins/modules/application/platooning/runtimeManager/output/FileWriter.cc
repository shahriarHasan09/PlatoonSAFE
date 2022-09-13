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

#include <iomanip>
#include "FileWriter.h"

FileWriter::FileWriter(int vehicleId, std::string outputFilename, bool &write2file) : vehicleId(vehicleId), outputFileName(outputFilename) {
    // open the file
    output.open(outputFileName, std::ios::out);
    if(!output) {
        std::cerr << outputFilename << " could not opened. Simulation will not write output." << std::endl;
        write2file = false;
    } else {
        // Write the header of the output
        writeHeaders();
    }
}

FileWriter::~FileWriter() {
    // TODO Auto-generated destructor stub
    output << "|" << std::setfill('=') << std::setw(280) << "" << "|"<< std::setfill(' ');
    output << std::endl;
    output << std::setfill('*') << std::setw(129) << "" << std::setfill(' ');
    output << " ENDS OF CURRENT RECORD ";
    output << std::setfill('*') << std::setw(129) << ""<< std::setfill(' ');
}

void FileWriter::addEntries(const RM::RMParameters &rmParam, const RM::RMLog_Own &ego, bool distViolated, bool decelViolated) {
    // parameters
    output << "|" << std::setw(14) << std::left << ego.activeController << "|" << std::right;
    output << "|" << std::setw(13) << std::left << rmParam.expectedBeconInterval << "|" << std::right;
    output << "|" << std::setw(13) << std::left << rmParam.rmMonitorInterval << "|" << std::right;
    output << "|" << std::setw(13) << std::left << rmParam.nPacketLossPoor << "|" << std::right;
    output << "|" << std::setw(13) << std::left << rmParam.nPacketLossCritical << "|" << std::right;
    if(ego.activeController == Plexe::ACC) {
        output << "|" << std::setw(13) << std::left
               << (ego.currentAccH > 0.0 && ego.currentAccH != rmParam.accHeadwaytimeGap? std::to_string(ego.currentAccH) : std::to_string(rmParam.accHeadwaytimeGap) + "(DV)")
               << "|" << std::right;
    } else {
        output << "|" << std::setw(13) << std::left
                      << "NA"
                      << "|" << std::right;
    }

    if(ego.activeController == Plexe::PLOEG) {
        output << "|" << std::setw(13) << std::left
               << (ego.currentPloegH > 0.0  && ego.currentPloegH != rmParam.ploegHeadwayTimeGap? std::to_string(ego.currentPloegH) : std::to_string(rmParam.ploegHeadwayTimeGap) + "(DV)")
               << "|" << std::right;
    } else {
        output << "|" << std::setw(13) << std::left
                       << "NA"
                       << "|" << std::right;
    }

    if(ego.activeController == Plexe::CACC) {
        output << "|" << std::setw(13) << std::left
               << (ego.currentCaccSpacing > 0.0 && ego.currentCaccSpacing != rmParam.caccConstantSpacing? std::to_string(ego.currentCaccSpacing) : std::to_string(rmParam.caccConstantSpacing) + "(DV)")
               << "|" << std::right;
    } else {
        output << "|" << std::setw(13) << std::left
                       << "NA"
                       << "|" << std::right;
    }

    output << "|" << std::setw(13) << std::left << (ego.activeController == Plexe::PLOEG ? std::to_string(rmParam.emergencyPloegHeadwayTimeGapFactor) : "NA") << "|" << std::right;
    output << "|" << std::setw(14) << std::left << (ego.activeController == Plexe::CACC ? std::to_string(rmParam.emergencyCaccConstantSpacingFactor) : "NA")<< "|" << std::right;

    // contracts
    for(auto it = (ego.assumptions)->cbegin(); it != (ego.assumptions)->cend(); ++it) {
        // c2x
        if(std::shared_ptr<WIFIAssumption> wificontract = std::dynamic_pointer_cast<WIFIAssumption>(*it)) {
            output << "|" << std::setw(9) << std::left << (wificontract->c2f).quality << "|" << std::right;
            output << "|" << std::setw(9) << std::left << (wificontract->c2l).quality << "|" << std::right;
        }
    }

    // deceleration
    output << "|" << std::setw(14) << std::left << ego.acceleration << "|" << std::right;
    output << "|" << std::setw(14) << std::left << (ego.maxDeceleration < 0.0 ? std::to_string(ego.maxDeceleration) : "NA") << "|" << std::right;

    // dist2pred violation
    output << "|" << std::setw(14) << std::left << (distViolated ? "yes" : "no") << "|" << std::right;
    output << "|" << std::setw(14) << std::left << (ego.crashed ? "crashed" : std::to_string(ego.dist2pred)) << "|" << std::right;

    // deceleration violation
    output << "|" << std::setw(14) << std::left << (decelViolated ? "yes" : "no") << "|" << std::right;
    output << "|" << std::setw(14) << std::left << (ego.maxDeceleration < 0.0 ? std::to_string(ego.maxDeceleration) : "NA") << "|" << std::right;

    // time
    output << "|" << std::setw(10) << std::left << ego.time << "|" << std::right;

    output << std::endl;
    output << "|" << std::setfill('-') << std::setw(280) << "" << "|"<< std::setfill(' ');

    output << std::endl;
}

void FileWriter::writeHeaders() {
    output << "VEHICLE: " << vehicleId << std::endl;
    // Abbreviation
    output << "\t" << "Abbreviations: " << std::endl;
    output << "\t\t" << std::setw(20) << std::left << "DV" << ": Default Value" << std::endl;
    output << "\t\t" << std::setw(20) << std::left << "BeaconINR" << ": Beacon Interval" << std::endl;
    output << "\t\t" << std::setw(20) << std::left << "MonitorINR" << ": Monitor Interval" << std::endl;
    output << "\n\n";

    output << "|" << std::setfill('=') << std::setw(280) << "" << "|"<< std::setfill(' ');
    output << std::endl;

    output << "|" << std::setw(150) << std::left << parameters << "|" << std::right;
    output << "|" << std::setw(20) << std::left << c2xQuality << "|" << std::right;
    output << "|" << std::setw(30) << std::left << acceleration << "|" << std::right;
    output << "|" << std::setw(30) << std::left << dist2predViolation << "|" << std::right;
    output << "|" << std::setw(30) << std::left << decelViolation << "|" << std::right;
    output << "|" << std::setw(10) << "" << "|";

    output << std::endl;

    output << "|" << std::setfill('-') << std::setw(150) << "" << "|"<< std::setfill(' ');
    output << "|" << std::setfill('-') << std::setw(20) << "" << "|"<< std::setfill(' ');
    output << "|" << std::setfill('-') << std::setw(30) << "" << "|"<< std::setfill(' ');
    output << "|" << std::setfill('-') << std::setw(30) << "" << "|"<< std::setfill(' ');
    output << "|" << std::setfill('-') << std::setw(30) << "" << "|"<< std::setfill(' ');
    output << "|" << std::setw(10) << std::left << dataRecordedTime << "|" << std::right;

    output << std::endl;

    // parameters
    output << "|" << std::setw(14) << std::left << activeMode << "|" << std::right;
    output << "|" << std::setw(13) << std::left << beaconINR << "|" << std::right;
    output << "|" << std::setw(13) << std::left << monitorINR << "|" << std::right;
    output << "|" << std::setw(13) << std::left << PktlossPoor << "|" << std::right;
    output << "|" << std::setw(13) << std::left << PktLossCrit << "|" << std::right;
    output << "|" << std::setw(13) << std::left << accHTG << "|" << std::right;
    output << "|" << std::setw(13) << std::left << ploegHTG << "|" << std::right;
    output << "|" << std::setw(13) << std::left << caccSpac << "|" << std::right;
    output << "|" << std::setw(13) << std::left << ploegFactor << "|" << std::right;
    output << "|" << std::setw(14) << std::left << caccFactor << "|" << std::right;

    // c2x
    output << "|" << std::setw(9) << std::left << front << "|" << std::right;
    output << "|" << std::setw(9) << std::left << leader << "|" << std::right;

    // deceleration
    output << "|" << std::setw(14) << std::left << currentAccel << "|" << std::right;
    output << "|" << std::setw(14) << std::left << maxDecel << "|" << std::right;

    // dist2pred violation
    output << "|" << std::setw(14) << std::left << yesNo << "|" << std::right;
    output << "|" << std::setw(14) << std::left << dist2pred << "|" << std::right;

    // deceleration violation
    output << "|" << std::setw(14) << std::left << yesNo << "|" << std::right;
    output << "|" << std::setw(14) << std::left << decel << "|" << std::right;

    // time
    output << "|" << std::setw(10) << "" << "|";

    output << std::endl;
    output << "|" << std::setfill('=') << std::setw(280) << "" << "|"<< std::setfill(' ');

    output << std::endl;
}
