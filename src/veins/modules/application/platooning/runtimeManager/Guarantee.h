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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEE_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEE_H_

#include "Assumption.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"
#include "veins/modules/application/platooning/CC_Const.h"

class RuntimeManager;

class Guarantee {
public:
    Guarantee(RuntimeManager *rm);
    virtual ~Guarantee();
    virtual void operator()(std::shared_ptr<Assumption> assumption) const;
protected:
    // These are required for taking action
    RuntimeManager *rm;
    Veins::TraCIMobility *mobility;
    Veins::TraCICommandInterface *traci;
    Veins::TraCICommandInterface::Vehicle *traciVehicle;
    BasePositionHelper* positionHelper;

    RM::RMParameters &rmParam;
    RM::rm_log &rmLog;
    cOutVector *controllerOut;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEE_H_ */
