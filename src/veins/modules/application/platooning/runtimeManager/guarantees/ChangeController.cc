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
#include "ChangeController.h"
#include "veins/modules/application/platooning/runtimeManager/RuntimeManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChangeController's Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ChangeController::ChangeController(RuntimeManager *rm, Plexe::ACTIVE_CONTROLLER to) : Guarantee(rm), to(to){

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChangeController's Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ChangeController::~ChangeController() {
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChangeController's methods implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ChangeController::actionOnTransition() const {
    RM::RMLog_Own &ego = std::get<0>(rmLog);
    if(to == Plexe::ACC) {
        traciVehicle->setACCHeadwayTime(rmParam.accHeadwaytimeGap);
//        traciVehicle->setCruiseControlDesiredSpeed(0.1); // ON TESTING
//        traciVehicle->setFixedAcceleration(1, -4.0);     // ON TESTING
//        traciVehicle->setFixedAcceleration(0, -4.0);     // ON TESTING
        ego.currentAccH = traciVehicle->getACCHeadwayTime();
    } else if(to == Plexe::PLOEG){
        traciVehicle->setParameter(CC_PAR_PLOEG_H, rmParam.ploegHeadwayTimeGap);
        ego.currentPloegH = rmParam.ploegHeadwayTimeGap;
    } else if(to == Plexe::CACC) {
        traciVehicle->setCACCConstantSpacing(rmParam.caccConstantSpacing);
        ego.currentCaccSpacing = traciVehicle->getCACCConstantSpacing();
    } else {
        std::cerr << "Error: Wrong Controller!!!" << __FILE__
                  << "\n\tLine: " << __LINE__
                  << "\n\tCompiled on: " << __DATE__
                  << " at " << __TIME__
                  << "\n\tfunction " << __func__
                  << std::endl;
    }
}


void ChangeController::operator()(std::shared_ptr<Assumption> assumption) const {
#if DEBUG_RM
    std::cout << std::setw(30) << std::setfill('#') << ""
              << " VEHICLE " << positionHelper->getId() << " ::: "
              << "Contract's Status BEFORE Transition "
              << std::setw(30) << std::setfill('#') << "" << "\n"
              << *(std::static_pointer_cast<WIFIAssumption>(assumption))
              << std::setfill(' ') << std::endl;
#endif

#if DEBUG_RM
    std::cout << std::setw(37) << std::setfill('#') << ""
              <<" VEHICLE " << positionHelper->getId() << " :: "
              << "Controller Transition "
              << std::setw(37) << std::setfill('#') << "" << "\n"
              << std::setw(37) << std::setfill(' ') << ""
              << (Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController() << " => " << to << std::endl
              << std::setfill(' ');
#endif
    // perform actionOnTransition by default.
    // Do it before transition, so that new controller can use the parameter value it straight away
    //if(rmParam.actionOnTransitionEnabled) actionOnTransition();


    // Perform the transition
    traciVehicle->setActiveController(to);

    // action on transition
    actionOnTransition();

    // changing the color of vehicle
    rm->setVehicleColor(to);

    // record the active controller to the output vector
//    controllerOut->record(traciVehicle->getActiveController());
    controllerOut->record(controller2int((Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController()));

    // update the vehicle's current contract status for the Active controller
    // As the consecutive Guarantee requires the current active controller (included in the key_type of the Contract-Guarantee unordered_map )
    assumption->updateOnTransition((Plexe::ACTIVE_CONTROLLER)traciVehicle->getActiveController());

#if DEBUG_RM
    std::cout << std::setw(30) << std::setfill('#') << ""
              << " VEHICLE " << positionHelper->getId() << " :::: "
              << "Contract's Status AFTER Transition "
              << std::setw(30) << std::setfill('#') << "" << "\n"
              << *(std::static_pointer_cast<WIFIAssumption>(assumption))
              << std::setfill(' ') << std::endl;
#endif
}
