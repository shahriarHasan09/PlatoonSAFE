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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_CHANGECONTROLLERANDADJUSTGAP2FRONT_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_CHANGECONTROLLERANDADJUSTGAP2FRONT_H_
#include "AdjustGap2Front.h"
#include "veins/modules/application/platooning/runtimeManager/guarantees/ChangeController.h"


class ChangeControllerAndAdjustGap2Front : public AdjustGap2Front, public ChangeController {
public:
    ChangeControllerAndAdjustGap2Front(RuntimeManager *rm, Plexe::ACTIVE_CONTROLLER to, GAP2FRONT gap2front);
    virtual ~ChangeControllerAndAdjustGap2Front();
    virtual void operator()(std::shared_ptr<Assumption> assumption) const override;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_CHANGECONTROLLERANDADJUSTGAP2FRONT_H_ */
