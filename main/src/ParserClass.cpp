#include <iostream>
#include <vector>
#include <string>
#include "ParserClass.h"

const wchar_t* ParserClass::SELECT_VAR = L"SELECT ATTRIBUTE(S):";
const wchar_t* ParserClass::NUM_OF_GROUPING = L"NUMBER OF GROUPING VARIABLES(n):";
const wchar_t* ParserClass::GROUPING_ATTRIBUTE = L"GROUPING ATTRIBUTES(V):";
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
    std::string nextLine;
    while(std::getline(std::wcin, line))
    {
        if (line.empty())
            break;
        if (line == ParserClass::SELECT_VAR)
        {
            std::cin >> nextLine;
            this->getSelectVar(nextLine);
        }
        else if (line == ParserClass::NUM_OF_GROUPING)
        {
            int theNum;
            std::cin >> theNum;
            this->inputs.num = theNum;
        }
        else if (line == ParserClass::AGG_FUNCS)
        {
            std::cin >> nextLine;
            this->getAggFunc(nextLine);
        }
        else if (line == ParserClass::SELECT_CONDS)
        {
            for (int i = 0; i < this->inputs.num; ++i)
            {
                std::cin >> nextLine;
                this->getSelectCond(nextLine);
            }
        }
        else if (line == ParserClass::HAVING_CONDS)
        {
            std::getline(std::cin, nextLine);
            this->getHavingCond(nextLine);
        }
    }
}

void ParserClass::getSelectVar(std::string line)
{
    std::vector<std::string> vars = ParserClass::splitStr(line);
    this->inputs.select_var = vars;
}

void ParserClass::getAggFunc(std::string line)
{
    std::vector<std::string> funcs = ParserClass::splitStr(line);
    this->inputs.aggre_funcs = funcs;
}

inline void ParserClass::getSelectCond(std::string line)
{
    this->inputs.cond_vars.emplace_back(line);
}

inline void ParserClass::getHavingCond(std::string line)
{
    this->inputs.having_conds = line;
}

/**
 * INPUT: like 'varName.1_sum_quant > 2 * varName.2_sum_quant || varName.1_avg_quant > varName.3_avg_quant'
 * PARAM: line : string
 * ATTRIBUTES: this->inputs.having_conds
 * RETURN: None
 * DESCRIPTION: basically this is a function parsing if-else.
 */
/**
 * TODO: Implements funcs left;
 */