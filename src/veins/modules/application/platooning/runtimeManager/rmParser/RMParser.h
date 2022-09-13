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

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_RMPARSER_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_RMPARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <algorithm>

#include "parserGrammars/RMParserGrammar.h"
#include "parserGrammars/RMContractParserGrammar.h"
#include "veins/modules/application/platooning/runtimeManager/Contracts.h"


/**
 * This is used to defined what kind of Grammar is required to parse input file
 *  Designed this way for possible future extension
 */
enum class ParserGrammarType {
    CONTRACT,
};

/**
 * This is starting point of the RMParser
 */
class RMParser {
public:
    RMParser(RuntimeManager *rm, Contracts *contracts, std::string inputFilename);
    ~RMParser();
    template <typename A, typename G> void addContract(const A a, const std::shared_ptr<G> g);     // TODO... parameter will be added later
private:
    void initGrammars();
    void parse(RuntimeManager *rm);
    // Pointer to Contracts for callback
    Contracts *contracts;
    // a ifstream object to read input file
    std::ifstream input;
    // A map of ParserGrammarType and RMParserGrammar
    std::map<ParserGrammarType , std::shared_ptr<RMParserGrammar>>  parserGrammars;
};

// template declaration
template <typename A, typename G> void RMParser::addContract(const A a, std::shared_ptr<G> g) {
    contracts->addContract(a, g);
}

// Non-member function
std::string removeCommentsAndTrim(std::string &s);
std::string removeComments(std::string &s);
std::string trim(std::string &s);
std::string left_trim(std::string &s);
std::string right_trim(std::string s);

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMPARSER_RMPARSER_H_ */
