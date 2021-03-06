#pragma once
#include <string>
#include <vector>

/**
 * SELECT ATTRIBUTES
 * NUMBER OF GROUPING VARIABLES(n)
 * GROUPING ATTRIBUTES(V)
 * F-VECT([F])
 * SELECT CONDITION-VECT([σ])
 * HAVING_CONDITION(G)
 */

struct InputStruct
{
    std::vector<std::string> select_var;
    std::string group_var;
    std::vector<std::string> aggre_funcs;
    std::vector<std::string> cond_vars;

    // The format of HAVING_CONDITION should be a line of if.
    // TODO: the meaning of this var needs to specifying
    std::string having_conds;
    // std::vector<std::string> having_conds;

    // NUMBER OF GROUPING VARIABLES
    int num;

    InputStruct() : num(0)
    {}
};