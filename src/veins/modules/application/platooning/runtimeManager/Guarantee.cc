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

#include "Guarantee.h"

#include <iostream>
#include "veins/modules/application/platooning/runtimeManager/RuntimeManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guarantees's Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Guarantee::Guarantee(RuntimeManager *rm) : rm(rm), rmParam(rm->rmParam), rmLog(rm->rmLog), controllerOut(rm->controllerOut){
    mobility = rm->mobility;
    traci = rm->traci;
    traciVehicle = rm->traciVehicle;
    positionHelper = rm->positionHelper;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guarantees's Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Guarantee::~Guarantee() {
    // This is for checking memory release
#if DEBUG_RM1
    std::cout << "Guarantee is destroyed successfully." << std::endl;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guarantees's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Guarantee::operator()(std::shared_ptr<Assumption> assumption) const {
    throw std::runtime_error("This method should be override by the Derived classes...");
//        std::cerr << "Error: " << __FILE__
//                  << "\n\tLine: " << __LINE__
//                  << "\n\tCompiled on: " << __DATE__
//                  << " at " << __TIME__
//                  << "\n\tfunction " << __func__
//                  << " This function should be implemented by the derived class!!!"
//                  << std::endl;
}

