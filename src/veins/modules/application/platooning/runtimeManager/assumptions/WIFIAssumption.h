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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_ASSUMPTIONS_WIFIASSUMPTION_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_ASSUMPTIONS_WIFIASSUMPTION_H_

#include<iostream>

#include "../Assumption.h"
#include "veins/modules/application/platooning/runtimeManager/StateParameter.h"


class WIFIAssumption : public Assumption {
    friend class std::hash<const WIFIAssumption>;
    friend std::ostream &operator<<(std::ostream &os, const WIFIAssumption &assumption);
public:
    WIFIAssumption(ASSUMPTION_TYPE type, Plexe::ACTIVE_CONTROLLER controller, C2F c2f, C2L c2l);

    virtual ~WIFIAssumption();
    virtual void evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon = false, const int index = -1) override;
    virtual bool equal(const Assumption &assumption) const override;
    virtual void updateOnTransition(Plexe::ACTIVE_CONTROLLER to) override;
private:
    C2F c2f;
    C2L c2l;

    // friend decleration
    friend class FileWriter;
};

// Defining template specialization for std::hash<WIFIContract> to use in unordered map
// Not using the base class in Contract-Guarantee map

namespace std{
template <> struct hash<const WIFIAssumption> {
   typedef size_t result_type;
   typedef WIFIAssumption argument_type;
   result_type operator()(const argument_type &wifi_assumption) const {
       return std::hash<std::underlying_type<ASSUMPTION_TYPE>::type>()(wifi_assumption.type) ^
              std::hash<std::underlying_type<Plexe::ACTIVE_CONTROLLER>::type>()(wifi_assumption.controller) ^
              std::hash<C2F>()(wifi_assumption.c2f) ^
              std::hash<C2L>()(wifi_assumption.c2l);
   }
};
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_ASSUMPTIONS_WIFIASSUMPTION_H_ */

