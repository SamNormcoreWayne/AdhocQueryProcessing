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
 *  SELECT CONDITION-VECT - selectCondVect<varName, conditions> : map<int, string>
 */

struct MFStruct
{
    std::string groupAttr;
    std::map<int, std::string> aggFunc;
    std::map<int, std::string> selectCondVect;
};

struct ParsedStruct
{
    std::vector<std::string> selectAttr;
    std::string havingCond;
    std::vector<MFStruct> partitions;
};
