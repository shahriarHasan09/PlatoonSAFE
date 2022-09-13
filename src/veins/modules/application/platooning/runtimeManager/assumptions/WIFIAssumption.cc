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

#include "WIFIAssumption.h"

#include <iostream>

WIFIAssumption::WIFIAssumption(ASSUMPTION_TYPE type, Plexe::ACTIVE_CONTROLLER controller, C2F c2f, C2L c2l) : Assumption(type, controller), c2f(c2f), c2l(c2l) {
    // TODO Auto-generated constructor stub

}

WIFIAssumption::~WIFIAssumption() {
    //std::cout << "Contract is destroyed successfully." << std::endl;
}

void WIFIAssumption::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    assumptionChanged = c2f.evaluate(rmParam, rmLog, onPlatoonBeacon, index) ||
                      c2l.evaluate(rmParam, rmLog, onPlatoonBeacon, index);
}

bool WIFIAssumption::equal(const Assumption &assumption) const {
    auto rhs = dynamic_cast<const WIFIAssumption &>(assumption);
    return (c2f == rhs.c2f) &&
           (c2l == rhs.c2l);
}


void WIFIAssumption::updateOnTransition(Plexe::ACTIVE_CONTROLLER to) {
    controller = to;
}

std::ostream &operator<<(std::ostream &os, const WIFIAssumption &assumption) {
    return os << assumption.type << "\n\t\tActiveController : " << assumption.controller << "\n\t\t" << assumption.c2f << "\n\t\t" << assumption.c2l;
}

