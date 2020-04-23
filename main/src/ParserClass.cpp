#include <iostream>
#include <vector>
#include <string>
#include "ParserClass.h"
#include "parsedStruct.h"

const wchar_t* ParserClass::SELECT_VAR = L"SELECT ATTRIBUTE(S):";
const wchar_t* ParserClass::NUM_OF_GROUPING = L"NUMBER OF GROUPING VARIABLES(n):";
const wchar_t* ParserClass::GROUPING_ATTRIBUTE = L"GROUPING ATTRIBUTES(V):";
const wchar_t* ParserClass::AGG_FUNCS = L"F-VECT([F]):";
const wchar_t* ParserClass::SELECT_CONDS = L"SELECT CONDITION-VECT([σ]):";
const char* ParserClass::HAVING_CONDS = "HAVING_CONDITION(G)";

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
            this->setSelectVar(nextLine);
        }
        else if (line == ParserClass::NUM_OF_GROUPING)
        {
            int theNum;
            std::cin >> theNum;
            this->inputs.num = theNum;
        }
        else if (line == ParserClass::GROUPING_ATTRIBUTE)
        {
            std::cin >> nextLine;
            this->inputs.group_var = ParserClass::splitStr(nextLine);
        }
        else if (line == ParserClass::AGG_FUNCS)
        {
            std::cin >> nextLine;
            this->setAggFunc(nextLine);
        }
        else if (line == ParserClass::SELECT_CONDS)
        {
            /*
             * The number of lines in SELECT CONDITION-VECT([σ]) is unknown.
             * So we parse SELECT COND and HAVING COND together. 
             */
            do
            {
                if (nextLine != ParserClass::HAVING_CONDS)
                    this->setSelectCond(nextLine);
                else
                {
                    std::cin >> nextLine;
                    this->setHavingCond(nextLine);
                }
            } while (std::getline(std::cin, nextLine));
        }
    }
}

void ParserClass::setSelectVar(std::string line)
{
    std::vector<std::string> vars = ParserClass::splitStr(line);
    this->inputs.select_var = vars;
}

void ParserClass::setAggFunc(std::string line)
{
    std::vector<std::string> funcs = ParserClass::splitStr(line);
    this->inputs.aggre_funcs = funcs;
}

inline void ParserClass::setSelectCond(std::string line)
{
    this->inputs.cond_vars.emplace_back(line);
}

inline void ParserClass::setHavingCond(std::string line)
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

inline void ParserClass::parseSelectAttr()
{
    this->parsedInputs.selectAttr = this->inputs.select_var;
}

inline void ParserClass::parseHavingConds()
{
    this->parsedInputs.havingCond = this->inputs.having_conds;
}

void ParserClass::parseMFStruct()
{
    // parse Agg Funcs:
    std::map<int, std::string> parsedAggFuncs = this->parseAggFunc();
    std::map<int, std::string> parsedSelectCond = this->parseSelectCond();
    // parse Group Attributes:

    for (int i = 0; i < this->parsedInputs.partition_size; ++i)
    {
        this->parsedInputs.partition_size = this->inputs.group_var.size();
        this->parsedInputs.partitions.emplace_back(inputs.group_var[i], parsedAggFuncs, parsedSelectCond);
        //Test required here!
    }
}

ParsedStruct ParserClass::getParsed()
{
    return this->parsedInputs;
    // reture an Obj. Opt requried in future. Check Effective Modern C++ Item 25
    // ROV here
}