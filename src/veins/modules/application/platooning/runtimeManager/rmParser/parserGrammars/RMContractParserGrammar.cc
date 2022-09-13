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
#include "../RMParser.h"
#include "RMContractParserGrammar.h"

////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////
RMContractParserGrammar::RMContractParserGrammar() {
    initRegexPattern();
    initParametersValueMap();
}

////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////
RMContractParserGrammar::~RMContractParserGrammar() {

}
////////////////////////////////////////////////////
// Member function
////////////////////////////////////////////////////
void RMContractParserGrammar::initRegexPattern() {
    std::string contractPattern = "::contract\\s*\\[\\s*"
                                  "(ctype)\\s*=\\s*(wifi)\\s*[:]+\\s*"
                                  "(c2f)\\s*=\\s*(ok|poor|critical)[\\s;]+"
                                  "(c2l)\\s*=\\s*(ok|poor|critical)[\\s;]+"
                                  "(mode)\\s*=\\s*(acc|ploeg|cacc)\\s*[:]+\\s*"
                                  "((transition2mode)\\s*=\\s*(acc|ploeg|cacc)[\\s;]*)?"
                                  "((dist2pred)\\s*=\\s*(default|increase)[\\s;]*)?"
                                  "\\]";
    expression = std::regex(contractPattern, std::regex::icase);
}

void RMContractParserGrammar::initParametersValueMap() {
    // CONTRACT_TYPE
    atypeMap.insert(std::make_pair(std::string("wifi"), ASSUMPTION_TYPE::WIFI));
    // QUALITY
    qualityMap.insert(std::make_pair(std::string("ok"), WIFI_QUALITY::OK));
    qualityMap.insert(std::make_pair(std::string("poor"), WIFI_QUALITY::POOR));
    qualityMap.insert(std::make_pair(std::string("critical"), WIFI_QUALITY::CRITICAL));
    // ACTIVE_CONTROLLER
    controllerMap.insert(std::make_pair(std::string("acc"), Plexe::ACTIVE_CONTROLLER::ACC));
    controllerMap.insert(std::make_pair(std::string("ploeg"), Plexe::ACTIVE_CONTROLLER::PLOEG));
    controllerMap.insert(std::make_pair(std::string("cacc"), Plexe::ACTIVE_CONTROLLER::CACC));
    // GAP_CONTROL
    dist2predMap.insert(std::make_pair(std::string("default"), GAP2FRONT::DEFAULT));
    dist2predMap.insert(std::make_pair(std::string("increase"), GAP2FRONT::INCREASE));
}

void RMContractParserGrammar::parse(RuntimeManager *rm, RMParser *rmParser, std::string &s, unsigned lineNo) {
    for(std::sregex_iterator it(s.begin(), s.end(), expression), end; it != end; ++it) {
        //std::cout << "LineNo: " << lineNo << "\n" << it->str() << std::endl;
        //std::cout << "Found:::::: " << (*it)[4].str() << std::endl;
        evaluate(rm, rmParser, *it, lineNo);
    }
}

void RMContractParserGrammar::evaluate(RuntimeManager *rm, RMParser *rmParser, const std::smatch &match, unsigned lineNo) {
    auto atype = atypeMap.find(match[2].str());
    if (atype == atypeMap.end()) throw std::runtime_error("missing ctype!!!");
    auto c2f = qualityMap.find(match[4]);
    if (c2f == qualityMap.end()) throw std::runtime_error("missing c2l!!!");
    auto c2l = qualityMap.find(match[6]);
    if (c2l == qualityMap.end()) throw std::runtime_error("missing c2f!!!");
    auto mode = controllerMap.find(match[8]);
    if (mode == controllerMap.end()) throw std::runtime_error("missing mode!!!");

    if(match[9].matched) {
        // Sanity checking, if passed, generate the Contract instance
        if(match[8] == match[11]) {
            std::string error = "Value for mode and transition2mode are equal on line no: " + std::to_string(lineNo);
            throw std::runtime_error(error);
        }
        auto transition2mode = controllerMap.find(match[11]);
        if (transition2mode == controllerMap.end()) throw std::runtime_error("missing transition2mode!!!");
        // There is a possibility of matching match[12]
        if(match[12].matched) {
            auto dist2pred = dist2predMap.find(match[14]);
            if (dist2pred == dist2predMap.end()) throw std::runtime_error("missing dist2pred!!!");
            // add contract
            if(atype->second == ASSUMPTION_TYPE::WIFI) {
                WIFIAssumption assumption(atype->second, mode->second, c2f->second, c2l->second);
                std::shared_ptr<Guarantee> guarantee  = std::make_shared<ChangeControllerAndAdjustGap2Front>(rm, transition2mode->second, dist2pred->second);
                rmParser->addContract(assumption, guarantee);
            } else {
                std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
            }
        } else {
            // add contract
            if(atype->second == ASSUMPTION_TYPE::WIFI) {
                WIFIAssumption assumption(atype->second, mode->second, c2f->second, c2l->second);
                std::shared_ptr<Guarantee> guarantee  = std::make_shared<ChangeController>(rm, transition2mode->second);
                rmParser->addContract(assumption, guarantee);
            } else {
                std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
            }
        }
    } else if(match[12].matched){
        auto dist2pred = dist2predMap.find(match[14]);
        if (dist2pred == dist2predMap.end()) throw std::runtime_error("missing dist2pred!!!");

        // add contract
        if(atype->second == ASSUMPTION_TYPE::WIFI) {
            WIFIAssumption assumption(atype->second, mode->second, c2f->second, c2l->second);
            std::shared_ptr<Guarantee> guarantee  = std::make_shared<AdjustGap2Front>(rm, dist2pred->second);
            rmParser->addContract(assumption, guarantee);
        } else {
            std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
        }
    } else {
        throw std::runtime_error("Missing guarantee!!!");
    }

//
//    if(match[9].matched && match[12].matched) {
//        // Sanity checking, if passed, generate the Contract instance
//        if(match[8] == match[11]) {
//            std::string error = "Value for mode and transition2mode are equal on line no: " + std::to_string(lineNo);
//            throw std::runtime_error(error);
//        }
//        auto transition2mode = controllerMap.find(match[11]);
//        if (transition2mode == controllerMap.end()) throw std::runtime_error("missing transition2mode!!!");
//        auto dist2pred = dist2predMap.find(match[14]);
//        if (dist2pred == dist2predMap.end()) throw std::runtime_error("missing dist2pred!!!");
//
//        // add contract
//        if(atype->second == ASSUMPTION_TYPE::WIFI) {
//            WIFIAssumption assumption(atype->second, mode->second, c2f->second, c2l->second);
//            std::shared_ptr<Guarantee> guarantee  = std::make_shared<ChangeControllerAndAdjustGap2Front>(rm, transition2mode->second, dist2pred->second);
//            rmParser->addContract(assumption, guarantee);
//        } else {
//            std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
//        }
//
////        std::cout << "ctype : " << atype->second
////                  << " c2l : " << c2l->second
////                  << " c2f : " << c2f->second
////                  << " mode : " << mode->second
////                  << " transition2mode : " << transition2mode->second
////                  << " dist2pred : " << dist2pred->second
////                  << std::endl;
//
//    } else if (match[9].matched) {
//        // Sanity checking, if passed, generate the Contract instance
//        if(match[8] == match[11]) {
//            std::string error = "Value for mode and transition2mode are equal on line no: " + std::to_string(lineNo);
//            throw std::runtime_error(error);
//        }
//        auto transition2mode = controllerMap.find(match[11]);
//        if (transition2mode == controllerMap.end()) throw std::runtime_error("missing transition2mode!!!");
//
//        // add contract
//        if(atype->second == ASSUMPTION_TYPE::WIFI) {
//            WIFIAssumption assumption(atype->second, mode->second, c2f->second, c2l->second);
//            std::shared_ptr<Guarantee> guarantee  = std::make_shared<ChangeController>(rm, transition2mode->second);
//            rmParser->addContract(assumption, guarantee);
//        } else {
//            std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
//        }
//
////        std::cout << "ctype : " << atype->second
////                  << " c2l : " << c2l->second
////                  << " c2f : " << c2f->second
////                  << " mode : " << mode->second
////                  << " transition2mode : " << transition2mode->second
////                  << std::endl;
//    } else if (match[12].matched) {
//        auto dist2pred = dist2predMap.find(match[14]);
//        if (dist2pred == dist2predMap.end()) throw std::runtime_error("missing dist2pred!!!");
//
//        // add contract
//        if(atype->second == ASSUMPTION_TYPE::WIFI) {
//            WIFIAssumption assumption(atype->second, mode->second, c2f->second, c2l->second);
//            std::shared_ptr<Guarantee> guarantee  = std::make_shared<AdjustGap2Front>(rm, dist2pred->second);
//            rmParser->addContract(assumption, guarantee);
//        } else {
//            std::cout<<"Only ASSUMPTION_TYPE::WIFI is available right now..." << std::endl;
//        }
////        std::cout << "ctype : " << atype->second
////                  << " c2l : " << c2l->second
////                  << " c2f : " << c2f->second
////                  << " mode : " << mode->second
////                  << " dist2pred : " << dist2pred->second
////                  << std::endl;
//    } else {
//        throw std::runtime_error("missing guarantee!!!");
//    }

}

