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

#include "Assumption.h"

#include <iostream>

Assumption::Assumption(ASSUMPTION_TYPE type, Plexe::ACTIVE_CONTROLLER controller) : type(type), controller(controller), assumptionChanged(false) {

}

Assumption::~Assumption() {
    // TODO Auto-generated destructor stub
}

void Assumption::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {//    virtual void actionOnTransition() const;

    throw std::runtime_error("This method should be overriden by the Derived classes...");
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " must have to be overriden by the derived class!!!"
//              << std::endl;
}


bool Assumption::equal(const Assumption &assumption) const {
    return (type == assumption.type) &&
           (controller == assumption.controller);
}

void Assumption::updateOnTransition(Plexe::ACTIVE_CONTROLLER to) {
    throw std::runtime_error("This method should be overriden by the Derived classes...");
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " must have to be overriden by the derived class!!!"
//              << std::endl;
}

Plexe::ACTIVE_CONTROLLER Assumption::getController() const {
    return controller;
}

ASSUMPTION_TYPE Assumption::getType() const {
    return type;
}

bool Assumption::isChanged() const {
    return assumptionChanged;
}

// Contract free == operator
bool operator==(const Assumption &asm1, const Assumption &asm2 ) {
    return (typeid(asm1) == typeid(asm2)) && asm1.equal(asm2);
}


// output (<<) for enum CONTRACT_TYPE
std::ostream &operator<<(std::ostream &os, const ASSUMPTION_TYPE aType) {
    std::map<ASSUMPTION_TYPE, std::string> enum2string;
    if(enum2string.size() == 0) {
#define INSERT(v) enum2string[v] = #v
        INSERT(ASSUMPTION_TYPE::WIFI);
        INSERT(ASSUMPTION_TYPE::INTERNAL_ERROR);
#undef INSERT
    }
    return os << enum2string[aType];
}
