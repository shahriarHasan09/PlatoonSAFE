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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_PARSERGRAMMARS_RMCONTRACTPARSERGRAMMAR_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_PARSERGRAMMARS_RMCONTRACTPARSERGRAMMAR_H_

#include <map>
#include <memory>
#include "RMParserGrammar.h"
#include "veins/modules/application/platooning/CC_Const.h"
#include "../../assumptions/WIFIAssumption.h"
#include "veins/modules/application/platooning/runtimeManager/StateParameter.h"


class RMContractParserGrammar : public RMParserGrammar{
public:
    RMContractParserGrammar();
    ~RMContractParserGrammar() override;

    void parse(RuntimeManager *rm, RMParser *rmParser, std::string &s, unsigned lineNo) override;
private:
    void initRegexPattern();
    void initParametersValueMap();
    void evaluate(RuntimeManager *rm, RMParser *rmParser , const std::smatch &match, unsigned lineNo);

    std::map<std::string, ASSUMPTION_TYPE> atypeMap;
    std::map<std::string, WIFI_QUALITY> qualityMap;
    std::map<std::string, Plexe::ACTIVE_CONTROLLER > controllerMap;
    std::map<std::string, GAP2FRONT > dist2predMap;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_PARSERGRAMMARS_RMCONTRACTPARSERGRAMMAR_H_ */
