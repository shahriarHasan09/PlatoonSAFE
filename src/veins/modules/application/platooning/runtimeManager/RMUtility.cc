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

#include<map>
#include<string>
#include "RMUtility.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// enum Plexe::ACTIVE_CONTROLLERY << implementation :: Utility
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, const Plexe::ACTIVE_CONTROLLER value) {
    std::map<Plexe::ACTIVE_CONTROLLER, std::string> enum2string;
    if (enum2string.size() == 0) {
#define INSERT(v) enum2string[v] = #v
    INSERT(Plexe::DRIVER);
    INSERT(Plexe::ACC);
    INSERT(Plexe::CACC);
    INSERT(Plexe::FAKED_CACC);
    INSERT(Plexe::PLOEG);
    INSERT(Plexe::CONSENSUS);
    INSERT(Plexe::FLATBED);
#undef INSERT
    }
    return os << enum2string[value];
}

int controller2int(const Plexe::ACTIVE_CONTROLLER controller) {
    if(controller == Plexe::ACC) return 4;
    if(controller == Plexe::PLOEG) return 2;
    if(controller == Plexe::CACC) return 0;
}
