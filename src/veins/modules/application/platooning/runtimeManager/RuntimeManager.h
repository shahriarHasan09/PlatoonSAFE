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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_

#include<iostream>
#include<fstream>

#include "assumptions/WIFIAssumption.h"
#include "Contracts.h"
#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/application/platooning/messages/PlatooningBeacon_m.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"
#include "veins/modules/application/platooning/runtimeManager/StateParameter.h"
#include "veins/modules/application/platooning/runtimeManager/RMUtility.h"
#include "veins/modules/application/platooning/runtimeManager/output/FileWriter.h"




class RuntimeManager : public Veins::BaseApplLayer {
public:
    RuntimeManager();
    virtual ~RuntimeManager();
    virtual void initialize(int stage) override;

    /**
     * This method is called by the application on receiving platooning beacon
     * RM log the required information in PlatooningBeacon pb
     */
    void onPlatoonBeacon(const PlatooningBeacon *pb, const SimTime currentTime);

    /**
     * This method return true if RM is enabled
     */
    bool isRMEnabled() {
        return rmParam.rmEnabled;
    }

    /**
     * Using different color for different controller
     */
    void setVehicleColor(Plexe::ACTIVE_CONTROLLER controller) const;

protected:
    virtual void handleSelfMsg(cMessage* msg) override;
private:
    /**
     * This method is used to logged the own vehicle data
     */
    void egoLog();

    /**
     * This method check for minSafetyDistance and maximum deceleration
     */
    void safetyViolationCheck() const;

    /**
     * This template function is used to perform log for both the front and the leader vehicle
     */
    template <typename T> void commonLog(const PlatooningBeacon *pb, T &loggedVehicle, const SimTime currentTime);

    /**
     * This method will start evaluating the logged info with the user provided parameters value
     * Based on the evaluation it will update all contracts
     */
    void evaluate(bool onPlatoonBeacon = false, int index = -1);

    /**
     * This method initialize all assumptions for the vehicle
     */
    void initializeDefaultAssumptions();

    // This variable keep the track of monitoring message schedule time
    int monitorMsgCounter = 0;

    // RuntimeManager's parameter
    RM::RMParameters rmParam;
    // Log to be done by RuntimeManager
    RM::rm_log rmLog;
    // Reference to the full list of Contract::Guarantees map
    std::shared_ptr<Contracts> rmContracts;

    // TraCI Interface used to communicate with the SUMO
    Veins::TraCIMobility *mobility;
    Veins::TraCICommandInterface *traci;
    Veins::TraCICommandInterface::Vehicle *traciVehicle;
    BasePositionHelper* positionHelper;

    // Self message to monitor vehicle state periodically
    cMessage *monitoringMsg;

    // output file writer
    std::shared_ptr<FileWriter> fileWriter;

    // output vector to store the record of active controller as a function of time
    cOutVector *controllerOut;

    // friend declaration
    friend class Guarantee;
    friend class Contracts;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RUNTIMEMANAGER_H_ */
