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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_ASSUMPTION_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_ASSUMPTION_H_

#include <iostream>
#include <map>
#include "veins/modules/application/platooning/CC_Const.h"
#include "veins/modules/application/platooning/runtimeManager/RMUtility.h"

/**
 * This is the super class for Contract
 */


enum ASSUMPTION_TYPE{
    WIFI,
    INTERNAL_ERROR,
};

std::ostream &operator<<(std::ostream &os, const ASSUMPTION_TYPE aType);

class Assumption {
    friend bool operator==(const Assumption &asm1, const Assumption &asm2 );
public:
    Assumption(ASSUMPTION_TYPE type, Plexe::ACTIVE_CONTROLLER controller);
    virtual ~Assumption();
    virtual void evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) = 0;
    virtual bool equal(const Assumption &assumption) const;
    virtual void updateOnTransition(Plexe::ACTIVE_CONTROLLER to) = 0;
    Plexe::ACTIVE_CONTROLLER getController() const;
    ASSUMPTION_TYPE getType() const;
    bool isChanged() const;

protected:
    ASSUMPTION_TYPE type;
    Plexe::ACTIVE_CONTROLLER controller;
    bool assumptionChanged;
};



#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_ASSUMPTION_H_ */
