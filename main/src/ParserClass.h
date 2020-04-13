#pragma once
#include <string>
#include <vector>
#include "inputStruct.h"

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
    inputStruct inputs;
    std::vector<std::string> query;
protected:
    enum aggFuncType {SUM, AVG, MAX, MIN, COUNT};
    static const wchar_t* SELECT_VAR;
    static const wchar_t* NUM_OF_GROUPING;
    static const wchar_t* AGG_FUNCS;
    static const wchar_t* SELECT_CONDS;
    static const wchar_t* HAVING_CONDS;
public:
    ParserClass();
    ~ParserClass();
    ParserClass(const ParserClass &) = delete;
    ParserClass& operator= (const ParserClass &) = delete;

    // class attributes operators;
    void getSelectVar();
    void setNum();

    // class methods for specific functions;
    /**
     * TODO: parseAggFunc() have param: wstring thisLine;
     * -> parseAggFunc(wstring thieLine);
     * so as:
     *  parseInput();
     */
    void readInput();
    void parseAggFunc();
    void parseInput();
    void parseSelectCond();
    void parseHavingCond();

    // Output
    int generateCode();
};