#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "ParserClass.hpp"
#include "parsedStruct.hpp"

const char* ParserClass::SELECT_VAR = "SELECT ATTRIBUTE:";
const char* ParserClass::NUM_OF_GROUPING = "NUMBER OF GROUPING VARIABLES:";
const char* ParserClass::GROUPING_ATTRIBUTE = "GROUPING ATTRIBUTES:";
const char* ParserClass::AGG_FUNCS = "F-VECT:";
const char* ParserClass::SELECT_CONDS = "SELECT CONDITION-VECT:";
const char* ParserClass::HAVING_CONDS = "HAVING_CONDITION:";

void ParserClass::readFromFile()
{
    std::string line;
    std::string nextLine;
    std::string filename = "";
    std::cout << "Input file absolute path: " << std::endl;
    std::cin >> filename;
    std::cin.clear();
    std::ifstream ifs(filename, std::ios::in);
    bool flag = false;
    while(true)
    {
        if (flag)
            break;
        getline(ifs, line);
        if (line == ParserClass::SELECT_VAR)
        {
            getline(ifs, nextLine);
            this->setSelectVar(nextLine);
        }
        else if (line == ParserClass::NUM_OF_GROUPING)
        {
            int theNum;
            ifs >> theNum;
            this->inputs.num = theNum;
            getline(ifs, line);
        }
        else if (line == ParserClass::GROUPING_ATTRIBUTE)
        {
            getline(ifs, nextLine);
            this->inputs.group_var = nextLine;
        }
        else if (line == ParserClass::AGG_FUNCS)
        {
            getline(ifs, nextLine);
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
                    getline(ifs, nextLine);
                    if (nextLine.back() == ';')
                    {
                        nextLine.pop_back();
                        flag = true;
                    }
                    this->setHavingCond(nextLine);
                    if (flag)
                        break;
                }
                getline(ifs, nextLine);
            } while (true);
        }
    }
}

void ParserClass::readInput()
{
    std::string line;
    std::string nextLine;
    getline(std::cin, line);
    bool flag = false;
    while(true)
    {
        if (flag)
            break;
        getline(std::cin, line);
        if (line == ParserClass::SELECT_VAR)
        {
            getline(std::cin, nextLine);
            this->setSelectVar(nextLine);
        }
        else if (line == ParserClass::NUM_OF_GROUPING)
        {
            int theNum;
            std::cin >> theNum;
            this->inputs.num = theNum;
            getline(std::cin, line);
        }
        else if (line == ParserClass::GROUPING_ATTRIBUTE)
        {
            getline(std::cin, nextLine);
            this->inputs.group_var = nextLine;
        }
        else if (line == ParserClass::AGG_FUNCS)
        {
            getline(std::cin, nextLine);
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
                    getline(std::cin, nextLine);
                    if (nextLine.back() == ';')
                    {
                        nextLine.pop_back();
                        flag = true;
                    }
                    this->setHavingCond(nextLine);
                    if (flag)
                        break;
                }
                getline(std::cin, nextLine);
            } while (true);
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

void ParserClass::parseSelectAttr()
{
    this->parsedInputs.selectAttr = "";
    for (auto itr : this->inputs.select_var)
    {
        if (itr == this->inputs.select_var[this->inputs.select_var.size() - 1])
        {
            this->parsedInputs.selectAttr += itr;
            break;
        }
        this->parsedInputs.selectAttr += itr + ", ";
    }
}

void ParserClass::parseHavingConds()
{
    std::vector<std::string> tmp_vec = ParserClass::splitStr(this->inputs.having_conds, ' ');
    this->parsedInputs.havingCond = "";
    for (int i = 0; i < tmp_vec.size(); ++i)
    {
        if (tmp_vec[i].size() > 3)
        {
            std::vector<std::string> func_splited = ParserClass::splitStr(tmp_vec[i], '_');
            std::string func_parsed = "";
            if (func_splited.size() == 3)
            {
                func_parsed = func_splited[0] + "." + func_splited[1] + "(" + func_splited[2] + ")";
            }
            else
            {
                func_parsed = func_splited[0] + "(" + func_splited[1] + ")";
            }
            tmp_vec[i] = func_parsed;
        }
        if (i != tmp_vec.size() - 1)
            tmp_vec[i] += ' ';
        this->parsedInputs.havingCond += tmp_vec[i];
    }
}

void ParserClass::parseMFStruct()
{
    // parse Agg Funcs:
    std::map<int, std::string> parsedAggFuncs = this->parseAggFunc();
    std::map<int, std::string> parsedSelectCond = this->parseSelectCond();
    // parse Group Attributes:
    this->parsedInputs.groupAttr = this->inputs.group_var;
    this->parsedInputs.aggFunc = ParserClass::convertMapToJSON(parsedAggFuncs);
    this->parsedInputs.selectCondVect = ParserClass::convertMapToJSON(parsedSelectCond);
}

std::map<int, std::string> ParserClass::parseAggFunc()
{
    std::map<int, std::string> agg_map;
    for (auto aggFunc : this->inputs.aggre_funcs)
    {
        if ('a' <= aggFunc[0] && aggFunc[0] <= 'z')
        {
            if (agg_map.find(0) == agg_map.end())
            {
                agg_map[0] = aggFunc;
                continue;
            }
            agg_map[0] += ", " + aggFunc;
        }
        else
        {
            int index = aggFunc[0] - '0';
            if (agg_map.find(index) == agg_map.end())
            {
                agg_map[index] = aggFunc;
                continue;
            }
            agg_map[index] += ", " + aggFunc;
        }
    }
    return agg_map;
}

std::map<int, std::string> ParserClass::parseSelectCond()
{
    std::map<int, std::string> select_map;
    for (auto select_cond : this->inputs.cond_vars)
    {
        int index = select_cond[0] - '0';
        select_map[index] = select_cond;
    }

    return select_map;
}

ParsedStruct ParserClass::getParsed()
{
    return this->parsedInputs;
    // reture an Obj. Opt requried in future. Check Effective Modern C++ Item 25
    // ROV here
}