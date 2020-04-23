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

struct MFStruct
{
    std::string groupAttr;
    std::map<int, std::string> aggFunc;
    std::map<int, std::string> selectCondVect;
    MFStruct() = default;
    MFStruct(std::string attr, std::map<int, std::string> agg, std::map<int, std::string> select)
    {
        /**
         * TODO: Implement this func
         */
        this->groupAttr = attr;
        this->aggFunc = agg;
        this->selectCondVect = select;
    }
};

struct ParsedStruct
{
    std::vector<std::string> selectAttr;
    std::string havingCond;
    std::vector<MFStruct> partitions;
    int partition_size = 0;
};
