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

#include "RMParser.h"

////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////
RMParser::RMParser(RuntimeManager *rm, Contracts *contracts, std::string inputFilename) : contracts(contracts) {
    // open the input file, if succeed read line by line and create the list of Contracts to be considered by the runtime manager
    input.open(inputFilename, std::ios::in);
    if(!input) {
        std::cerr << inputFilename << " could not opened. Terminating..." << std::endl;
        exit(-1);
    }
    // init the grammar list
    initGrammars();

    // File opened successfully. Lets start parsing the input
    parse(rm);
}

////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////
RMParser::~RMParser() {
    // The following close operation is not required, as this will be called automatically by the compiler once
    // the ifstream goes out of the scope
    input.close();
    // No dynamically allocated memory(using built in pointer) is used by the class
}

////////////////////////////////////////////////////
// Member Function
////////////////////////////////////////////////////

void RMParser::initGrammars() {
    // This will be hard coded
    // CONTRACT Grammar
    parserGrammars.insert(std::make_pair(ParserGrammarType::CONTRACT, std::make_shared<RMContractParserGrammar>()));
    // [ Debug ===================
    std::cout << "CONTRACT grammar has been added to the RMParser" << std::endl;
    // ==================== Debug ]

    // TODO :: add more grammars if required (For further extension)
    // Right now we are only parsing Contract from the input
}

void RMParser::parse(RuntimeManager *rm) {

    std::cout << "RMParser start parsing the Contracts from input file ...." << std::endl;
    // parse(std::string &line) from the appropriate Grammar class will be called after reading every line
    std::string line;
    unsigned lineNo = 0;
    while(std::getline(input, line)) {
        ++lineNo;
        // First remove the comments part of the line and leading and tailing whitespace
        std::string formattedLine = removeCommentsAndTrim(line);
        if(formattedLine.find("::contract") != std::string::npos) {
            //std::cout << formattedLine << std::endl;
            auto it = parserGrammars.find(ParserGrammarType::CONTRACT);
            if(it != parserGrammars.end()) {
                (it->second)->parse(rm, this, formattedLine, lineNo);
            } else {
                throw std::runtime_error("CONTRACT Grammar not available!!!");
            }
        }
        // TODO: extension if there is any other Grammars available
    }

    std::cout << "RMParser finished parsing the Contracts from input file successfully." << std::endl;

}

////////////////////////////////////////////////////
// Non-Member Function
////////////////////////////////////////////////////
/**
 * String represents starting of comments
 */
std::string COMMENT = "#";          // add more character that can define start of comment
/**
* String represents starting of WHITESPACE
*/
std::string WHITESPACE = "  \n\r\t\f\v";       // add more character that can define WHITESPACE

std::string removeCommentsAndTrim(std::string &s) {
    std::string notComments = removeComments(s);
    return notComments.empty() ? notComments : trim(notComments);
}
std::string removeComments(std::string &s) {
    auto start = s.find_first_of(COMMENT);
    return (start == std::string::npos) ? s : s.substr(0, start);
}
std::string trim(std::string &s) {
    return right_trim(left_trim(s));
}
std::string left_trim(std::string &s) {
    auto start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
std::string right_trim(std::string s) {
    auto end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end+1);
}

