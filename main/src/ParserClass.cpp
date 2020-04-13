#include <iostream>
#include <vector>
#include <string>
#include "ParserClass.h"

const wchar_t* ParserClass::SELECT_VAR = L"SELECT ATTRIBUTE(S):";
const wchar_t* ParserClass::NUM_OF_GROUPING = L"NUMBER OF GROUPING VARIABLES(n):";
const wchar_t* ParserClass::AGG_FUNCS = L"F-VECT([F]):";
const wchar_t* ParserClass::SELECT_CONDS = L"SELECT CONDITION-VECT([σ]):";
const wchar_t* ParserClass::HAVING_CONDS = L"HAVING_CONDITION(G)";

ParserClass::ParserClass()
{
    // TODO: need a good design here
}

void ParserClass::readInput()
{
    std::wstring line;
    while(getline(std::wcin, line))
    {
        if (line.empty())
            break;
        if (line == ParserClass::SELECT_VAR)
        {
            this->getSelectVar();
        }
        else if (line == ParserClass::NUM_OF_GROUPING)
        {
            int theNum;
            std::cin >> theNum;
            this->inputs.num = theNum;
        }
        else if (line == ParserClass::AGG_FUNCS)
        {
            this->parseAggFunc();
        }
        else if (line == ParserClass::SELECT_CONDS)
        {
            this->parseSelectCond();
        }
        else if (line == ParserClass::HAVING_CONDS)
        {
            this->parseHavingCond();
        }
    }
}

/**
 * TODO: Implements funcs left;
 */