import json
from collections import defaultdict

class operateClass():
    agg_func_enum = ("sum", "max", "min", "count", "avg")
    def __init__(self, selectAttr : str, havingCond : str, size : int, group_attr, agg_func, select_cond):
        self.select_attr_agg_func = selectAttr.split(", ")
        self.select_attr_parsed = []
        self.having_conds = havingCond
        self.group_attr = group_attr
        self.agg_func = json.loads(agg_func)
        self.agg_func_parsed = defaultdict(str)
        self.select_conds = json.loads(select_cond)
        self.part_size = size

    def parse_agg_group(self):
        for var_name, func_name in self.agg_func:
            self.agg_func_parsed[var_name] = self.parse_agg_func(func_name)

    @staticmethod
    def parse_agg_func(unparsed_agg_func):
        '''
            @input: sum_varname
            @param: unparsed_agg_func : str
            @return: {sum(varname) : 0} : dict
        '''
        parsed_func = None
        fuc_lst = unparsed_agg_func.split('_')
        if len(fuc_lst) == 3:
            if fuc_lst[1] in operateClass.agg_func_enum:
                parsed_func = fuc_lst[1] + '(' + fuc_lst[2] + ')'
            else:
                raise(TypeError("Cannot find this aggregate functions in SQL standards."))
        if len(fuc_lst) == 2:
            if fuc_lst[0] in operateClass.agg_func_enum:
                parsed_func = fuc_lst[0] + '(' + fuc_lst[1] + ')'
            else:
                raise(TypeError("Cannot find this aggregate functions in SQL standards."))
        return parsed_func

    def parse_select_attr(self):
        for item in self.select_attr_agg_func:
            if item in self.group_attr:
                self.select_attr_parsed.append(item)
            else:
                tmp = item.split("_")
                self.select_attr_parsed.append(tmp[1] + "({}.{})".format(tmp[0], tmp[2]))
            

class MFTable():
    def __init__(self, attr_lst : list, agg_lst : list = []):
        self.group_attr = dict.fromkeys(list, attr_lst, 0)
        if len(agg_lst) != 0:
            self.agg_func = dict.fromkeys(list, agg_lst, 0)