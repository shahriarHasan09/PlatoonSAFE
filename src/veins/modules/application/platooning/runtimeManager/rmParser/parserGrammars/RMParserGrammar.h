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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_PARSERGRAMMARS_RMPARSERGRAMMAR_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_PARSERGRAMMARS_RMPARSERGRAMMAR_H_

#include <regex>
#include "../../guarantees/ChangeController.h"
#include "../../guarantees/AdjustGap2Front.h"
#include "../../guarantees/ChangeControllerAndAdjustGap2Front.h"
// forward declaration
class RMParser;

class RMParserGrammar {
public:
    RMParserGrammar();
    virtual ~RMParserGrammar() = 0;
    virtual void parse(RuntimeManager *rm, RMParser *rmParser, std::string &s, unsigned lineNo) = 0;
protected:
    std::regex expression;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_PARSERGRAMMARS_RMPARSERGRAMMAR_H_ */
