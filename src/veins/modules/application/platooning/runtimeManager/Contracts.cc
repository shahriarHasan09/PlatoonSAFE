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
#include "Contracts.h"
#include "veins/modules/application/platooning/runtimeManager/RuntimeManager.h"
#include "veins/modules/application/platooning/runtimeManager/rmParser/RMParser.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contracts's Member function's implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Contracts::Contracts(RuntimeManager *rm) : rm(rm) {
    initContractsList(rm);
}

Contracts::~Contracts() {
}

void Contracts::evaluate(RM::RMLog_Own &state) {
    for(auto start = (state.assumptions)->begin(); start != (state.assumptions)->end(); ++start) {
        if((*start)->isChanged()) {
            if(std::shared_ptr<WIFIAssumption> assumption = std::dynamic_pointer_cast<WIFIAssumption>(*start)) {
                // Sanity check
                ASSERT(assumption->getType() == ASSUMPTION_TYPE::WIFI);
                // get the all contract-guarantee list from rmcg for CONTRACT_TYPE::WIFI
                auto match = rmContractsList.find(ASSUMPTION_TYPE::WIFI);
                if(match != rmContractsList.end()){
                    // We found a list of contract-guarantee for WIFI contract type
                    auto matchedCGContainer = std::static_pointer_cast<RMContractContainer<WIFIAssumption, Guarantee>>(match->second);

                    matchedCGContainer->provideGuarantee(assumption);
                }
                continue;
            }
            // TODO check in other Contract_Gurantee list
        }
    }
}


void Contracts::initContractsList(RuntimeManager *rm) {
    if(rm->rmParam.readContractsFromInputFile) {
        RMParser(rm, this, rm->rmParam.contractInputFilename);
    } else {

        // *************************************
        // Creating the WIFIContract-Guarantee list
        // =============================================== StateParameters ================================================
        // StateParameters C2F : atSafetyDistance = true (default)... This is used right now.
        C2F ok_c2f(WIFI_QUALITY::OK);
        C2F poor_c2f(WIFI_QUALITY::POOR);
        C2F critical_c2f(WIFI_QUALITY::CRITICAL);

        // StateParameters C2L
        C2L ok_c2l(WIFI_QUALITY::OK);
        C2L poor_c2l(WIFI_QUALITY::POOR);
        C2L critical_c2l(WIFI_QUALITY::CRITICAL);

        // ================================================== Guarantees ==================================================
        // Guarantees (ChangeController)
        std::shared_ptr<Guarantee> g2acc   = std::make_shared<ChangeController>(rm, Plexe::ACTIVE_CONTROLLER::ACC);
        std::shared_ptr<Guarantee> g2ploeg = std::make_shared<ChangeController>(rm, Plexe::ACTIVE_CONTROLLER::PLOEG);
        std::shared_ptr<Guarantee> g2cacc  = std::make_shared<ChangeController>(rm, Plexe::ACTIVE_CONTROLLER::CACC);

        // Guarantees (Gap2Front)
        std::shared_ptr<Guarantee> g2d_df = std::make_shared<AdjustGap2Front>(rm, GAP2FRONT::DEFAULT);
        std::shared_ptr<Guarantee> g2d_i  = std::make_shared<AdjustGap2Front>(rm, GAP2FRONT::INCREASE);


        // Guarantees (ChangeControllerAndDecelerate)
        std::shared_ptr<Guarantee> g2ploegN2d_i = std::make_shared<ChangeControllerAndAdjustGap2Front>(rm, Plexe::ACTIVE_CONTROLLER::PLOEG, GAP2FRONT::INCREASE);
        std::shared_ptr<Guarantee> g2caccN2d_i  = std::make_shared<ChangeControllerAndAdjustGap2Front>(rm, Plexe::ACTIVE_CONTROLLER::CACC, GAP2FRONT::INCREASE);




        // ======================================== WIFIContract for ACC controller ========================================
        // Upgrade
        WIFIAssumption acc2cacc(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, ok_c2l);
        WIFIAssumption acc2ploeg(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, critical_c2l);
        // degrade
        // ChangeController and Gap2Front
        WIFIAssumption acc2ploegN2d1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, poor_c2f, ok_c2l);
        WIFIAssumption acc2ploegN2d2(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, poor_c2f, poor_c2l);
        WIFIAssumption acc2ploegN2d3(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, poor_c2f, critical_c2l);

        WIFIAssumption acc2caccN2d1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, poor_c2l);

        // default


        // ======================================== WIFIContract for PLOEG controller =======================================
        // Upgrade
        WIFIAssumption ploeg2cacc(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, ok_c2f, ok_c2l);

        // degrade
        WIFIAssumption ploeg2acc1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, ok_c2l);
        WIFIAssumption ploeg2acc2(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, poor_c2l);
        WIFIAssumption ploeg2acc3(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, critical_c2f, critical_c2l);

        // Gap2Front
        WIFIAssumption ploeg2d1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, poor_c2f, ok_c2l);
        WIFIAssumption ploeg2d2(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, poor_c2f, poor_c2l);
        WIFIAssumption ploeg2d3(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, poor_c2f, critical_c2l);

        // default Gap2Front
        WIFIAssumption ploeg2d_default(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, ok_c2f, critical_c2l);
        // ChangeController and Gap2Front
        WIFIAssumption ploeg2caccN2d1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::PLOEG, ok_c2f, poor_c2l);


        // ======================================== WIFIContract for CACC controller ========================================
        // Degrade
        WIFIAssumption cacc2ploeg(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, ok_c2f, critical_c2l);
        // TODO The following three should be combined in one based on WIFI_QUALITY::ALL
        WIFIAssumption cacc2acc1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, ok_c2l);
        WIFIAssumption cacc2acc2(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, poor_c2l);
        WIFIAssumption cacc2acc3(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, critical_c2f, critical_c2l);
        // Gap2Front
        WIFIAssumption cacc2d1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, ok_c2f, poor_c2l);

        // default Gap2Front
        WIFIAssumption cacc2d_default(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, ok_c2f, ok_c2l);
        // ChangeController and Gap2Front
        WIFIAssumption cacc2ploegN2d1(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, poor_c2f, ok_c2l);
        WIFIAssumption cacc2ploegN2d2(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, poor_c2f, poor_c2l);
        WIFIAssumption cacc2ploegN2d3(ASSUMPTION_TYPE::WIFI, Plexe::ACTIVE_CONTROLLER::CACC, poor_c2f, critical_c2l);



        // ===================================================================================================================
        // RMCGContainer
        // ===================================================================================================================

        // upgrade
        if(rm->rmParam.upgradationEnabled) {
            // ==================== acc ====================
            // acc -> ploeg
            //addContract(acc2ploeg, g2ploeg);
            // acc->cacc
            //addContract(acc2cacc, g2cacc);
            // =================== ploeg ===================
            // ploeg->cacc
            addContract(ploeg2cacc, g2cacc);
        }
        // degrade
        if(rm->rmParam.degradationEnabled) {
            // =================== ploeg ===================
            // ploeg->acc
            addContract(ploeg2acc1, g2acc);
            addContract(ploeg2acc2, g2acc);
            addContract(ploeg2acc3, g2acc);
            // ===================  cacc ===================
            // cacc->ploeg
            addContract(cacc2ploeg, g2ploeg);

            // cacc->acc
            //addContract(cacc2acc1, g2acc);
            //addContract(cacc2acc2, g2acc);
            //addContract(cacc2acc3, g2acc);
        }

        // gapcontrol
        if(rm->rmParam.gapControlEnabled) {
            // ==================== acc ====================
            if(rm->rmParam.upgradationEnabled) {
                // acc->ploeg->gapControl
                addContract(acc2ploegN2d1, g2ploegN2d_i);
                addContract(acc2ploegN2d2, g2ploegN2d_i);
                addContract(acc2ploegN2d3, g2ploegN2d_i);
                // acc->cacc->gapControl
               // addContract(acc2caccN2d1, g2caccN2d_i);
            }
            // =================== ploeg ===================
            // ploeg->gapControl(Default)
            addContract(ploeg2d_default, g2d_df);

            // ploeg->gapControl
            addContract(ploeg2d1, g2d_i);
            addContract(ploeg2d2, g2d_i);
            addContract(ploeg2d3, g2d_i);

            // ploeg->cacc->gapControl
            if(rm->rmParam.upgradationEnabled) {
                addContract(ploeg2caccN2d1, g2caccN2d_i);
            }
            // ===================  cacc ===================
            // cacc->gapControl(Default)
            addContract(cacc2d_default, g2d_df);

            // cacc->gapControl
            addContract(cacc2d1, g2d_i);

            // cacc->ploeg->gapControl
            if(rm->rmParam.degradationEnabled) {
                addContract(cacc2ploegN2d1,g2ploegN2d_i);
                addContract(cacc2ploegN2d2,g2ploegN2d_i);
                addContract(cacc2ploegN2d3,g2ploegN2d_i);
            }

        }

    }

    // This is for checking the list of contracts
    auto sz = ((std::static_pointer_cast<RMContractContainer<WIFIAssumption, Guarantee>>(rmContractsList.find(ASSUMPTION_TYPE::WIFI)->second))->contractsContainer)->size();
    auto sz2 = rmContractsList.size();
    std::cout << "sz: " << sz << std::endl;

//#if DEBUG_RM2
//
////     // ============== Test: CHECKING WITH ADDING DUPLICATE element
////    addContract(acc2cacc, g2cacc);     // test OK
////
////
////    // ============== Test: try with add different type of contract: Test OK
////    WIFIAssumption acc2caccC(ASSUMPTION_TYPE::INTERNAL_ERROR, Plexe::ACTIVE_CONTROLLER::ACC, ok_c2f, ok_c2l);
////    std::shared_ptr<Guarantee> g2accC = std::make_shared<ChangeController>(rm, Plexe::ACTIVE_CONTROLLER::ACC);
////    addContract(acc2caccC, g2accC);
//
//    auto sz = ((std::static_pointer_cast<RMContractContainer<WIFIAssumption, Guarantee>>(rmContractsList.find(ASSUMPTION_TYPE::WIFI)->second))->contractsContainer)->size();
//    auto sz2 = rmContractsList.size();
//    std::cout << "sz: " << sz << std::endl;
//
////    ASSERT(sz == 24);
////    ASSERT(sz2 == 1);
//
////    auto cc = ((std::static_pointer_cast<RMContractContainer<WIFIAssumption, Guarantee>>(rmContractsList.find(ASSUMPTION_TYPE::WIFI)->second))->contractsContainer)->find(acc2cacc);
//
////    std::cout << "Testing find() on CG list: \n" << cc->first <<std::endl;
//
//#endif

}
