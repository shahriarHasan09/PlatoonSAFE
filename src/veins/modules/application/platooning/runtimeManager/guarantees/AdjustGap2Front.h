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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_ADJUSTGAP2FRONT_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_ADJUSTGAP2FRONT_H_

#include <iostream>

#include "../assumptions/WIFIAssumption.h"
#include "../Guarantee.h"


enum class GAP2FRONT {
    DEFAULT = 0,
    INCREASE = 1,
    DECREASE = 2,
};

std::ostream &operator<<(std::ostream &os, const GAP2FRONT gap2front);


class AdjustGap2Front : virtual public Guarantee {
public:
    AdjustGap2Front(RuntimeManager *rm, GAP2FRONT gap2front);
    virtual ~AdjustGap2Front();
    virtual void operator()(std::shared_ptr<Assumption> assumption) const override;
private:
    GAP2FRONT gap2front;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_GUARANTEES_ADJUSTGAP2FRONT_H_ */
