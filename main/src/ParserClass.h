#pragma once
#include <string>
#include <vector>
#include "inputStruct.h"
#include "parsedStruct.h"

/**
 * INPUT:
 * SELECT ATTRIBUTES
 * NUMBER OF GROUPING VARIABLES(n)
 * GROUPING ATTRIBUTES(V)
 * F-VECT([F])
 * SELECT CONDITION-VECT([σ])
 * HAVING_CONDITION(G)
 */

class ParserClass
{
private:
    InputStruct inputs;
    ParsedStruct parsedInputs;
    std::vector<std::string> query;
protected:
    enum aggFuncType {SUM, AVG, MAX, MIN, COUNT};
    static const wchar_t* SELECT_VAR;
    static const wchar_t* NUM_OF_GROUPING;
    static const wchar_t* GROUPING_ATTRIBUTE;
    static const wchar_t* AGG_FUNCS;
    static const wchar_t* SELECT_CONDS;
    static const char* HAVING_CONDS;
public:
    ParserClass();
    ~ParserClass();
    ParserClass(const ParserClass &) = delete;
    ParserClass& operator= (const ParserClass &) = delete;

    // class methods for attributes operators;
    void setSelectVar(std::string line);
    void setAggFunc(std::string line);
    inline void setSelectCond(std::string line);
    inline void setHavingCond(std::string line);
    void setNum();

    // class methods for specific functions;
    void readInput();
    inline void parseSelectAttr();
    inline void parseHavingConds();
    std::map<int, std::string>&& parseAggFunc();
    std::map<int, std::string>&& parseSelectCond();
    void parseMFStruct();

    // Output
    ParsedStruct getParsed();

    static std::vector<std::string>&& splitStr(std::string str, char pattern = ',')
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
            if (str[i] == pattern)
            {
                while(str[j] != ' ')
                {
                    ++j;
                }
                strLst.push_back(str.substr(j, i - j));
                //ParserClass::subStr(str, j, i);
                j = i + 1;
            }
        }
        return std::move(strLst);
    }
    static char* subStr(std::string str, int start, int end)
    {
        char* outStr = new char[end - start + 1];
        
    }
};