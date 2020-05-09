#pragma once
#include <string>
#include <vector>
#include "ParserClassException.hpp"
#include "inputStruct.hpp"
#include "parsedStruct.hpp"
#include "nlohmann/json.hpp"

/**
 * INPUT:
 * SELECT ATTRIBUTES
 * NUMBER OF GROUPING VARIABLES(n)
 * GROUPING ATTRIBUTES(V)
 * F-VECT([F])
 * SELECT CONDITION-VECT([σ])
 * HAVING_CONDITION(G)
 */

using json = nlohmann::json;

class ParserClass
{
private:
    InputStruct inputs;
    ParsedStruct parsedInputs;
protected:
    enum aggFuncType {SUM, AVG, MAX, MIN, COUNT};
    static const char* SELECT_VAR;
    static const char* NUM_OF_GROUPING;
    static const char* GROUPING_ATTRIBUTE;
    static const char* AGG_FUNCS;
    static const char* SELECT_CONDS;
    static const char* HAVING_CONDS;

    std::map<int, std::string> parseAggFunc();
    std::map<int, std::string> parseSelectCond();
public:
    ParserClass() = default;
    ~ParserClass() = default;
    ParserClass(const ParserClass &) = delete;
    ParserClass& operator= (const ParserClass &) = delete;
    void readInput();
    void readFromFile();

    // class methods for attributes operators;
    void setSelectVar(std::string line);
    void setAggFunc(std::string line);
    inline void setSelectCond(std::string line);
    inline void setHavingCond(std::string line);
    void setNum();

    // class methods for specific functions;
    void parseSelectAttr();
    void parseHavingConds();
    void parseMFStruct();

    // Output
    ParsedStruct getParsed();

    static std::vector<std::string> splitStr(std::string str, char pattern = ',')
    {
        /**
         * TODO: replace std::string.substr() by ParserClass::subStr()
         * Split a string by a specific pattern. The default pattern is ','
         * PARAMS: str : string, pattern : string
         * RETURN: stringLst : vector<string>
         */
        std::vector<std::string> strLst;
        int j = 0;
        for (int i = 0; i < str.size(); ++i)
        {
            //std::cout << "1:" << i << ": " << str[i] << std::endl;
            if (str[i] == pattern)
            {
                //std::cout << str.substr(j, i - j) << std::endl;
                strLst.push_back(str.substr(j, i - j));
                //ParserClass::subStr(str, j, i);
                while(str[i + 1] == ' ')
                {
                    ++i;
                }
                j = i + 1;
            }
        }
        strLst.push_back(str.substr(j, str.size() - 1));
        return std::move(strLst);
    }
    static char* subStr(std::string str, int start, int end)
    {
        char* outStr = new char[end - start + 1];
        
    }

    std::string convertMapToJSON(std::map<int, std::string> dict) const throw()
    {
        if (dict.size() == 0)
        {
            throw ParserClassException::ZeroSizeException();
            return "";
        }
        json data(dict);
        return data.dump();
    }
};