#pragma once
#include <string>
#include <vector>
#include <map>
/**
 * SELECT ATTRIBUTES is made of GROUPING ATTRIBUTES and F-VECTs.
 * selectAggFunc<varName, func> : <string, string>
 * selectAggVar<varName> : [string]
 * aggVar : [string] - this vars would be used in JOIN
 * aggFunc<varName, aggFunc> - this funcs would be used in JOIN
 * having conds.
 */

struct MFStruct
{
    std::string groupAttri;
    std::map<int, std::string> aggFunc;
    std::map<int, std::string> selectCondVect;
};

struct ParsedStruct
{
    std::vector<std::string> selectAttr;
    std::string havingCond;
    std::vector<MFStruct> partitions;
};