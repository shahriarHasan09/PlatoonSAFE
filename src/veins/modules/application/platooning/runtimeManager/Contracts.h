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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTS_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTS_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

#include "assumptions/WIFIAssumption.h"
#include "guarantees/AdjustGap2Front.h"
#include "guarantees/ChangeControllerAndAdjustGap2Front.h"
#include "Guarantee.h"
#include "rmContainer/RMContractContainer.h"
#include "veins/modules/application/platooning/CC_Const.h"
#include "veins/modules/application/platooning/runtimeManager/StateParameter.h"
#include "veins/modules/application/platooning/runtimeManager/guarantees/ChangeController.h"


// Forward declaration
class RuntimeManager;
class RMParser;

class Contracts {
public:
    Contracts(RuntimeManager *rm);
    virtual ~Contracts();
    void evaluate(RM::RMLog_Own &state);
private:
    void initContractsList(RuntimeManager *rm);
//    template <typename A, typename G> void addContract(const A a, const G *g);
    template <typename A, typename G> void addContract(const A a, const std::shared_ptr<G> g);

    // We need to make RMCGContainer shared_ptr to be destroyed automatically and will be shared
    std::map<ASSUMPTION_TYPE, std::shared_ptr<RMContainer>> rmContractsList;
    const RuntimeManager *rm;

    // friend decleration
    friend class RMParser;
};

// Template definition
template <typename A, typename G> void Contracts::addContract(const A a, const std::shared_ptr<G> g) {
    auto cgList = rmContractsList.find(a.getType());
    if(cgList != rmContractsList.end()) {
        // CG list for this contractType already been created
        if(a.getType() == ASSUMPTION_TYPE::WIFI) {
            // add the new element to the match
            // We need this cast as the base class does not defines virtual addContract() method
            (std::static_pointer_cast<RMContractContainer<WIFIAssumption, Guarantee>>(cgList->second))->addContract(a, g);
        } else if(a.getType() == ASSUMPTION_TYPE::INTERNAL_ERROR) {
            std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
        }
        // TODO add for other contract type
    } else {
        // CG list for this contractType not exists, needs to be created
        if(a.getType() == ASSUMPTION_TYPE::WIFI) {
            rmContractsList.insert(std::make_pair(ASSUMPTION_TYPE::WIFI, std::make_shared<RMContractContainer<WIFIAssumption, Guarantee>>(a, g, ASSUMPTION_TYPE::WIFI)));
        } else if(a.getType() == ASSUMPTION_TYPE::INTERNAL_ERROR) {
            std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
        }
        // TODO add for other contract type
    }

    std::cout << "A Contract added." << std::endl;
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_CONTRACTS_H_ */
