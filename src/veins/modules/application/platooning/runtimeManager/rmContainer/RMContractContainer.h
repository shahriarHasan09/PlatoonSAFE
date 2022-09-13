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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMCONTAINER_RMCONTRACTCONTAINER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMCONTAINER_RMCONTRACTCONTAINER_H_

#include <iostream>
#include <unordered_map>
#include <memory>

#include "../assumptions/WIFIAssumption.h"
#include "../Assumption.h"
#include "../Guarantee.h"
#include "veins/modules/application/platooning/runtimeManager/rmContainer/RMContainer.h"
#include "veins/modules/application/platooning/runtimeManager/guarantees/ChangeController.h"





template <typename A, typename G> class RMContractContainer;

template <typename A, typename G> std::ostream &operator<<(std::ostream &os, const RMContractContainer<A,G> &container) {
    return os << "Will add later..........";
}

template <typename A, typename G> class RMContractContainer : public RMContainer{
    friend std::ostream &operator<<<A,G>(std::ostream &os, const RMContractContainer<A,G> &container);
public:
    using container_type = std::unordered_map<const A, const std::shared_ptr<G>>;
    RMContractContainer(const A a, const std::shared_ptr<G> g, ASSUMPTION_TYPE atype);
    ~RMContractContainer();
    void addContract(const A a, std::shared_ptr<G> g);
    void provideGuarantee(std::shared_ptr<A> a) const;
//private:
    std::shared_ptr<container_type> contractsContainer;
};

template <typename A, typename G> RMContractContainer<A,G>::RMContractContainer(const A a, const std::shared_ptr<G> g, ASSUMPTION_TYPE atype) : RMContainer(atype) ,
        contractsContainer(std::make_shared<container_type>()){
    contractsContainer->insert(std::make_pair(a,g));
}

template <typename A, typename G> RMContractContainer<A,G>::~RMContractContainer() {
    // This is to check whether the container is destroyed
#if DEBUG_RM1
    std::cout << "RMCGContainer is destroyed...." << std::endl;
#endif

}


template <typename A, typename G> void RMContractContainer<A,G>::addContract(const A a, const std::shared_ptr<G> g) {
    contractsContainer->insert(std::make_pair(a,g));
}

template <typename A, typename G> void RMContractContainer<A,G>::provideGuarantee(std::shared_ptr<A> a) const {
    auto match = contractsContainer->find(*a);
    if(match != contractsContainer->end()) {
        (*(match->second))(a);
    } else {
        //std::cout << "Not matched contract found. No action needs to be taken...." << std::endl;
    }
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMCONTAINER_RMCONTRACTCONTAINER_H_ */
