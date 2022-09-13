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
#include <iostream>
#include "StateParameter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StateParameter's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StateParameter::StateParameter() {

}

StateParameter::~StateParameter() {
}



bool StateParameter::equal(const StateParameter &stateParameter) const {
    throw std::runtime_error("This method should be overriden by the Derived classes...");
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " This method should never be called!!!"
//              << std::endl;
    return true;
}

bool StateParameter::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    throw std::runtime_error("This method should be overriden by the Derived classes...");
//    std::cerr << "Error: " << __FILE__
//              << "\n\tLine: " << __LINE__
//              << "\n\tCompiled on: " << __DATE__
//              << " at " << __TIME__
//              << "\n\tfunction " << __func__
//              << " must have to be overriden by the derived class!!!"
//              << std::endl;
    return false;
}

// Free == operator
bool operator==(const StateParameter &sp1, const StateParameter &sp2) {
    return (typeid(sp1) == typeid(sp2)) && sp1.equal(sp2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C2X's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C2X::C2X(WIFI_QUALITY quality) : quality(quality){

}


template <typename T> void C2X::c2xQualityCheck(const RM::RMParameters &rmParam, const T &other) {
    // c2xInitiated ensures that received beacon time has been recorded
    if(other.common.c2xInitiated) {
        SimTime currentTime = simTime();
        int nBeaconMiss = (int)((currentTime.dbl() - other.common.lastBeaconArrivalTime) / rmParam.expectedBeconInterval);
//        std::cout << "nBeaconMiss: " << nBeaconMiss << std::endl;
        if(nBeaconMiss >= rmParam.nPacketLossCritical) {
            quality = WIFI_QUALITY::CRITICAL;
        } else if (nBeaconMiss < rmParam.nPacketLossCritical && nBeaconMiss >= rmParam.nPacketLossPoor) {
            quality = WIFI_QUALITY::POOR;
        } else {
            quality = WIFI_QUALITY::OK;
        }

    } else {
        // Sanity check
        ASSERT(quality == WIFI_QUALITY::CRITICAL);
    }
}

bool C2X::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    throw std::runtime_error("This method should be overriden by the Derived classes...");
//    std::cerr << "Error: " << __FILE__
//                          << "\n\tLine: " << __LINE__
//                          << "\n\tCompiled on: " << __DATE__
//                          << " at " << __TIME__
//                          << "\n\tfunction " << __func__
//                          << " Should be overriden by the derived class C2F and C2L!!!"
//                          << std::endl;
    return false;
}

// Operator overloaded
bool C2X::equal(const StateParameter &stateParameter) const {
    auto rhs = dynamic_cast<const C2X &>(stateParameter);
    return (quality == rhs.quality);
}


std::ostream &operator<<(std::ostream &os, const C2X &c2x){
    return os << "C2X: \n\tQuality : " << (WIFI_QUALITY)c2x.quality;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C2F's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C2F::C2F(WIFI_QUALITY quality, bool atSafeDistance) : C2X(quality), atSafeDistance(atSafeDistance) {

}


bool C2F::equal(const StateParameter &stateParameter) const {
    auto rhs = dynamic_cast<const C2F &>(stateParameter);
    return (quality == rhs.quality) &&
           (atSafeDistance == rhs.atSafeDistance);
//    return C2X::equal(stateParameter) &&
//            (atSafeDistance == rhs.atSafeDistance);
}

bool C2F::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    if(onPlatoonBeacon) {
        // Right now there is nothing to do here!!!
    } else {
        const RM::RMLog_Front &other = std::get<1>(rmLog);

        // This is required to check whether the state parameter is changed or not
        C2F temp = *this;

        c2xQualityCheck(rmParam, other);
        if(!(temp == *this)) {
            // StataParameter has been changed
//            std::cerr << "this: " << *this  << " \nOld" << temp << std::endl;
            return true;
        }
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const C2F &c2f) {
    os << "C2F: \n\t\t\tQuality : " << (WIFI_QUALITY)c2f.quality << "\n\t\t\tatSafeDistance: " << std::boolalpha << c2f.atSafeDistance;
    return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C2L's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C2L::C2L(WIFI_QUALITY quality) : C2X(quality) {

}

bool C2L::equal(const StateParameter &stateParameter) const {
    auto rhs = dynamic_cast<const C2L &>(stateParameter);
//    return C2X::equal(stateParameter);
    return (quality == rhs.quality);
}

bool C2L::evaluate(const RM::RMParameters &rmParam, const RM::rm_log &rmLog, const bool onPlatoonBeacon, const int index) {
    if(onPlatoonBeacon) {
        // Right now there is nothing to do here!!!
    } else {
        const RM::RMLog_Leader &other = std::get<2>(rmLog);

        // This is required to check whether the state parameter is changed or not
        C2L temp = *this;

        c2xQualityCheck(rmParam, other);

        if(!(temp == *this)) {
            // StataParameter has been changed
//            std::cerr << "this: " << *this  << " \nOld" << temp << std::endl;
            return true;
        }

        // ========== [ DEBUG
//        std::cerr << "this: " << *this  << " \nOld" << temp << std::endl;
        // ==========  DEBUG ]
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const C2L &c2l) {
    os << "C2L: \n\t\t\tQuality : " << (WIFI_QUALITY)c2l.quality;
    return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// enum WIFI_QUALITY << implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, const WIFI_QUALITY value) {
    std::map<WIFI_QUALITY, std::string> enum2string;
    if (enum2string.size() == 0) {
#define INSERT(v) enum2string[v] = #v
    INSERT(CRITICAL);
    INSERT(POOR);
    INSERT(OK);
#undef INSERT
    }
    return os << enum2string[value];
}


