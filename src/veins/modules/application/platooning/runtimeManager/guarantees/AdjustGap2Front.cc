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

#include <map>
#include <iomanip>
#include <string>
#include "AdjustGap2Front.h"
#include "veins/modules/application/platooning/runtimeManager/RuntimeManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AdjustGap2Front's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AdjustGap2Front::AdjustGap2Front(RuntimeManager *rm, GAP2FRONT gap2front) : Guarantee(rm), gap2front(gap2front){

}

AdjustGap2Front::~AdjustGap2Front() {

}

void AdjustGap2Front::operator()(std::shared_ptr<Assumption> assumption) const {
#if DEBUG_RM
    std::cout << std::setw(30) << std::setfill('#') << ""
              << " VEHICLE " << positionHelper->getId() << " ::: "
              << "Contract's Status BEFORE GapControl "
              << std::setw(30) << std::setfill('#') << "" << "\n"
              << *(std::static_pointer_cast<WIFIAssumption>(assumption))
              << std::setfill(' ') << std::endl;
#endif
    // We need to store the current headway +/ spacing for output file
    RM::RMLog_Own &ego = std::get<0>(rmLog);
    if(traciVehicle->getActiveController() == Plexe::ACC){
        std::cerr << "Warning: Nothing is doing for controller Plexe::ACC" << __FILE__
                         << "\n\tLine: " << __LINE__
                         << "\n\tCompiled on: " << __DATE__
                         << " at " << __TIME__
                         << "\n\tfunction: " << __func__
                         << std::endl;
    } else if (traciVehicle->getActiveController() == Plexe::PLOEG) {
        if(gap2front == GAP2FRONT::DEFAULT) {
            // go back to the default spacing
            ego.currentPloegH = rmParam.ploegHeadwayTimeGap;
            traciVehicle->setParameter(CC_PAR_PLOEG_H, ego.currentPloegH);
#if DEBUG_RM
//            double ploegH = 200;
//            traciVehicle->getParameter(CC_PAR_PLOEG_H, ploegH);     // This method does not work

            std::cout << std::setw(34) << std::setfill('#') << ""
                      << " VEHICLE " << positionHelper->getId() << " ::: " << "PERFORMED " << gap2front << " "
                      << std::setw(34) << std::setfill('#') << "" << "\n"
                      << std::setw(34) << std::setfill(' ') << ""
                      << " Set Default headwayTimeGap : " << ego.currentPloegH << "s"
                      << std::setfill(' ')
                      << std::endl;
#endif

        } else if (gap2front == GAP2FRONT::INCREASE){
            // increase the gap
            ego.currentPloegH = rmParam.ploegHeadwayTimeGap + rmParam.ploegHeadwayTimeGap * rmParam.emergencyPloegHeadwayTimeGapFactor;
            traciVehicle->setParameter(CC_PAR_PLOEG_H, ego.currentPloegH);
#if DEBUG_RM
//            double ploegH = 200;
//            traciVehicle->getParameter(CC_PAR_PLOEG_H, ploegH);     // This method does nto work

            std::cout << std::setw(33) << std::setfill('#') << ""
                      << " VEHICLE " << positionHelper->getId() << " ::: " << "PERFORMED " << gap2front << " "
                      << std::setw(33) << std::setfill('#') << "" << "\n"
                      << std::setw(33) << std::setfill(' ') << ""
                      << " Set headwayTimeGap : " << ego.currentPloegH << "s"
                      << std::setfill(' ')
                      << std::endl;
#endif

        } else {
            // TODO decrease the gap
        }

    } else if(traciVehicle->getActiveController() == Plexe::CACC) {
        if(gap2front == GAP2FRONT::DEFAULT) {
            // go back to the default spacing
            traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing);
            ego.currentCaccSpacing = traciVehicle->getCACCConstantSpacing();
#if DEBUG_RM
            std::cout << std::setw(34) << std::setfill('#') << ""
                      << " VEHICLE " << positionHelper->getId() << " ::: " << "PERFORMED " << gap2front << " "
                      << std::setw(34) << std::setfill('#') << "" << "\n"
                      << std::setw(34) << std::setfill(' ') << ""
                      << " Set Default caccSpacing : " << ego.currentCaccSpacing << "m"
                      << std::setfill(' ')
                      << std::endl;
#endif
        } else if (gap2front == GAP2FRONT::INCREASE){

            // increase the gap

            traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing + rmParam.caccConstantSpacing * rmParam.emergencyCaccConstantSpacingFactor);
            ego.currentCaccSpacing = traciVehicle->getCACCConstantSpacing();
#if DEBUG_RM
            std::cout << std::setw(34) << std::setfill('#') << ""
                      << " VEHICLE " << positionHelper->getId() << " ::: " << "PERFORMED " << gap2front << " "
                      << std::setw(34) << std::setfill('#') << "" << "\n"
                      << std::setw(34) << std::setfill(' ') << ""
                      << " Set caccSpacing : " << ego.currentCaccSpacing << "m"
                      << std::setfill(' ')
                      << std::endl;
#endif

        } else {
            // TODO decrease the gap. Not interest in increasing right now.
        }

    }

    // changing the color of vehicle
    rm->setVehicleColor((Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController());

    // record the change in controller
    controllerOut->record(controller2int((Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController()) + (int)gap2front);
//    controllerOut->record((int)gap2front);


#if DEBUG_RM
    std::cout << std::setw(31) << std::setfill('#') << ""
              << " VEHICLE " << positionHelper->getId() << " ::: "
              << "Contract's Status AFTER GapControl "
              << std::setw(31) << std::setfill('#') << "" << "\n"
              << *(std::static_pointer_cast<WIFIAssumption>(assumption))
              << std::setfill(' ') << std::endl;
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GAP2FRONT enum's << operator implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, const GAP2FRONT gap2front) {
    std::map<GAP2FRONT, std::string> enum2string;

    if(enum2string.size() == 0) {
#define INSERT(gap2front) enum2string[gap2front] = #gap2front
        INSERT(GAP2FRONT::DEFAULT);
        INSERT(GAP2FRONT::DECREASE);
        INSERT(GAP2FRONT::INCREASE);
#undef INSERT
    }

    return os << enum2string[gap2front];
}
