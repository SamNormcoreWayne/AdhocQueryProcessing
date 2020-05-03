#pragma once
#include <string>
#include <vector>
#include <map>
/*
 * SELECT ATTRIBUTES - selectAttr : vector<string>
 * HAVING CONDITIONS - havingCond : string
 * GROUPING ATTRIBUTES - partitions: vector<MFStruct>
 *  gourpAttr : string
 *  aggFunc<varName, aggFunc> : map<int, string>
 *      the varName should be a number, the aggFunc should be like aggFuncName_aggVarName e.g. sum_quant
 *  SELECT CONDITION-VECT - selectCondVect<varName, conditions> : map<int, string>
 */

struct ParsedStruct
{
    std::vector<std::string> selectAttr;
    std::string havingCond;
    std::string groupAttr;
    std::map<int, std::string> aggFunc;
    std::map<int, std::string> selectCondVect;
    ParsedStruct() = default;
    ParsedStruct(const ParsedStruct&) = default;
    ParsedStruct& operator=(const ParsedStruct& oldStruct)
    {
        this->selectAttr = oldStruct.selectAttr;
        this->havingCond = oldStruct.havingCond;
        this->groupAttr = oldStruct.groupAttr;
        this->aggFunc = oldStruct.aggFunc;
        this->selectCondVect = oldStruct.selectCondVect;
        return *this;
    }
};
