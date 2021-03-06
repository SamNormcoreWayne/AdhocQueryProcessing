#include <iostream>
#include <exception>
#include <fstream>
#include "PyCode.hpp"
#include "ParserClassException.hpp"

int PyCode::mainFunc(std::string cwd) noexcept
{
    try
    {
        std::cout << ">>Now generating MFStruct.py" << std::endl;
        this->generateMFStructPy(cwd);
        std::cout << ">>Now generating PostrgresCon.py" << std::endl;
        this->generatePostgresConPy(cwd);
        std::cout << ">>Now generating Main.py" << std::endl;
        this->generateMainPy(cwd);
        this->generateRequirements(cwd);
    }
    catch(ParserClassException::FileNotOpenException &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}

int PyCode::generateMFStructPy(std::string cwd) throw()
{
    std::ofstream ofs;
    std::string path = cwd + "/MFStruct.py";
    ofs.open(path, std::ios::out);
    if (!ofs.is_open())
    {
        ofs.clear();
        ofs.close();
        throw ParserClassException::FileNotOpenException();
    }
    else
    {
        ofs << "import json\n";
        ofs << "from collections import defaultdict\n";
        ofs << "\n";
        ofs << "class operateClass():\n";
        ofs << "\tagg_func_enum = (\"sum\", \"max\", \"min\", \"count\", \"avg\")\n";
        ofs << "\tdef __init__(self, selectAttr : str, havingCond : str, size : int, group_attr, agg_func, select_cond):\n";
        ofs << "\t\tself.select_attr_agg_func = selectAttr.split(\", \")\n";
        ofs << "\t\tself.select_attr_parsed = []\n";
        ofs << "\t\tself.having_conds = havingCond\n";
        ofs << "\t\tself.group_attr = group_attr\n";
        ofs << "\t\tself.agg_func = agg_func\n";
        ofs << "\t\tself.agg_func_parsed = defaultdict(str)\n";
        ofs << "\t\tself.select_conds = select_cond\n";
        ofs << "\t\tself.part_size = size\n";
        ofs << "\n";
        ofs << "\tdef parse_agg_group(self):\n";
        ofs << "\t\tfor var_name, func_name in self.agg_func.items():\n";
        ofs << "\t\t\tself.agg_func_parsed[var_name] = self.parse_agg_func(func_name)\n";
        ofs << "\n";
        ofs << "\t@staticmethod\n";
        ofs << "\tdef parse_agg_func(unparsed_agg_func):\n";
        ofs << "\t\t\'\'\'\n";
        ofs << "\t\t\t@input: sum_varname\n";
        ofs << "\t\t\t@param: unparsed_agg_func : str\n";
        ofs << "\t\t\t@return: {sum(varname) : 0} : dict\n";
        ofs << "\t\t\'\'\'\n";
        ofs << "\t\tparsed_func = []\n";
        ofs << "\t\tfor item in unparsed_agg_func:\n";
        ofs << "\t\t\tfuc_lst = item.split('_')\n";
        ofs << "\t\t\tif len(fuc_lst) == 3:\n";
        ofs << "\t\t\t\tif fuc_lst[1] in operateClass.agg_func_enum:\n";
        ofs << "\t\t\t\t\tparsed_func.append(fuc_lst[1] + \'(\' + fuc_lst[2] + \')\')\n";
        ofs << "\t\t\t\telse:\n";
        ofs << "\t\t\t\t\traise(TypeError(\"Cannot find this aggregate functions in SQL standards.\"))\n";
        ofs << "\t\t\tif len(fuc_lst) == 2:\n";
        ofs << "\t\t\t\tif fuc_lst[0] in operateClass.agg_func_enum:\n";
        ofs << "\t\t\t\t\tparsed_func.append(fuc_lst[0] + \'(\' + fuc_lst[1] + \')\')\n";
        ofs << "\t\t\t\telse:\n";
        ofs << "\t\t\t\t\traise(TypeError(\"Cannot find this aggregate functions in SQL standards.\"))\n";
        ofs << "\t\treturn parsed_func\n";
        ofs << "\n";
        ofs << "\tdef parse_select_attr(self):\n";
        ofs << "\t\tfor item in self.select_attr_agg_func:\n";
        ofs << "\t\t\tif item in self.group_attr:\n";
        ofs << "\t\t\t\tself.select_attr_parsed.append(item)\n";
        ofs << "\t\t\telse:\n";
        ofs << "\t\t\t\ttmp = item.split(\"_\")\n";
        ofs << "\t\t\t\tself.select_attr_parsed.append(tmp[1] + \"({}.{})\".format(tmp[0], tmp[2]))\n";
        ofs << "\n";
        ofs << "class MFTable():\n";
        ofs << "\tdef __init__(self, attr_lst : list, agg_lst : list = []):\n";
        ofs << "\t\tself.group_attr = dict.fromkeys(attr_lst, 0)\n";
        ofs << "\t\tself.agg_func = defaultdict(int)\n";
        ofs.clear();
        ofs.close();
    }
    return 0;
}

int PyCode::generateMainPy(std::string cwd) throw()
{
    std::ofstream ofs;
    std::string path = cwd + "/main.py";
    ofs.open(path, std::ios::out);
    if (!ofs.is_open())
    {
        ofs.clear();
        ofs.close();
        throw ParserClassException::FileNotOpenException();
    }
    else
    {
        ofs << "from prettytable import PrettyTable\n";
        ofs << "from MFStruct import operateClass\n";
        ofs << "from postgresCon import postgresCon\n";
        ofs << "\n";
        ofs << "def main():\n";
        ofs << "\tselectAttr = \"" + this->mfStruct.selectAttr + "\"\n";
        ofs << "\thavingCond = \"" + this->mfStruct.havingCond + "\"\n";
        ofs << "\tgroup_attr = \",\".join([\"" + this->mfStruct.groupAttr + "\"])\n";
        ofs << "\tagg_func = dict(" + this->mfStruct.aggFunc + ")\n";
        ofs << "\tselect_cond = dict(" + this->mfStruct.selectCondVect + ")\n";
        ofs << "\tfor key in agg_func:\n";
        ofs << "\t\tagg_func[key] = agg_func[key].split(\", \")\n";
        ofs << "\tsize = 3\n";
        ofs << "\tmf_struct = operateClass(selectAttr, havingCond, size, group_attr, agg_func, select_cond)\n";
        ofs << "\tmf_struct.parse_agg_group()\n";
        ofs << "\tmf_struct.parse_select_attr()\n";
        ofs << "\n";
        ofs << "\tusrname = input(\"Input username for connecting DB: \")\n";
        ofs << "\tpwd = input(\"Input pwd for connecting DB\")\n";
        ofs << "\thost = input(\"Input DB address: \")\n";
        ofs << "\tconn = postgresCon(usrname, pwd, host, mf_struct)\n";
        ofs << "\tconn.get_group_attr_data()\n";
        ofs << "\tconn.main_algo()\n";
        ofs << "\tconn.closeDB()\n";
        ofs << "\toutput = conn.project_data()\n";
        ofs << "\tprint(output)\n";
        ofs << "\n\n";
        ofs << "if __name__ == \"__main__\":\n";
        ofs << "\tmain()\n";
    }
    return 0;
}

int PyCode::generatePostgresConPy(std::string cwd) throw()
{
    std::ofstream ofs;
    std::string path = cwd + "/postgresCon.py";
    ofs.open(path, std::ios::out);
    if (!ofs.is_open())
    {
        ofs.clear();
        ofs.close();
        throw ParserClassException::FileNotOpenException();
    }
    else
    {
        ofs << "import psycopg2 as pstgre\n";
        ofs << "import psycopg2.extras\n";
        ofs << "import re\n";
        ofs << "from collections import defaultdict\n";
        ofs << "from enum import Enum\n";
        ofs << "from MFStruct import operateClass, MFTable\n";
        ofs << "from prettytable import PrettyTable\n";
        ofs << "\n";
        ofs << "\'\'\'\n";
        ofs << "\tAlgorithm 3.1 in the paper.\n";
        ofs << "\'\'\'\n";
        ofs << "\n";
        ofs << "class postgresCon():\n";
        ofs << "\tavg_func_dict = defaultdict(lambda : defaultdict(float))\n";
        ofs << "\toperators = [\"and\", \"or\"]\n";
        ofs << "\thigh_operators = [\"=\", \"<\", \">\", \"<>\", \"<=\", \">=\"]\n";
        ofs << "\thighest_operators = [\"+\", \"-\", \"*\", \"/\"]\n";
        ofs << "\tdef __init__(self, user, pwd, host, obj : operateClass):\n";
        ofs << "\t\tdb_name = \'sales_db\'\n";
        ofs << "\t\tport = \'5432\'\n";
        ofs << "\t\tself.db_name = db_name\n";
        ofs << "\t\tself.user = user\n";
        ofs << "\t\tself.pwd = pwd\n";
        ofs << "\t\tself.host = host\n";
        ofs << "\t\tself.port = port\n";
        ofs << "\t\tself.conn = pstgre.connect(host=host, database=db_name, user=user, password=pwd, port=port)\n";
        ofs << "\t\tself.sql_dict = defaultdict(str)\n";
        ofs << "\t\tself.operate_obj = obj\n";
        ofs << "\t\tself.mf_table = []\n";
        ofs << "\t\tself.cur = None\n";
        ofs << "\n";
        ofs << "\tdef get_group_attr_data(self):\n";
        ofs << "\t\toutput = ()\n";
        ofs << "\t\tif self.cur is None:\n";
        ofs << "\t\t\tself.cur = self.conn.cursor(cursor_factory=pstgre.extras.DictCursor)\n";
        ofs << "\t\tselect_attr = self.operate_obj.group_attr\n";
		ofs << "\t\tselect_attr_lst = select_attr.split(\", \")\n";
		ofs << "\t\tself.cur.execute(\"SELECT {} FROM sales GROUP BY {} ORDER BY {};\".format(select_attr, select_attr, select_attr_lst[0]))\n";
        ofs << "\t\twhile(True):\n";
        ofs << "\t\t\ttry:\n";
        ofs << "\t\t\t\toutput = self.cur.fetchone()\n";
        ofs << "\t\t\texcept(pstgre.ProgrammingError) as dberr:\n";
        ofs << "\t\t\t\tprint(\"Error while execute query in get_gourp_attr_data: \", dberr)\n";
        ofs << "\t\t\tfinally:\n";
        ofs << "\t\t\t\tif output is None:\n";
        ofs << "\t\t\t\t\tbreak\n";
        ofs << "\t\t\t\tmftable = MFTable(select_attr_lst)\n";
        ofs << "\t\t\t\tfor var_name in select_attr_lst:\n";
        ofs << "\t\t\t\t\tmftable.group_attr[var_name] = output[var_name]\n";
        ofs << "\t\t\t\tself.mf_table.append(mftable)\n";
        ofs << "\n";
        ofs << "\tdef main_algo(self):\n";
        ofs << "\t\tselect_attr = self.operate_obj.group_attr.split(\", \")\n";
        ofs << "\t\tif self.cur is None:\n";
        ofs << "\t\t\tself.cur = self.conn.cursor()\n";
        ofs << "\t\tself.cur.execute(\"SELECT * FROM sales;\")\n";
        ofs << "\t\twhile(True):\n";
        ofs << "\t\t\toutput = self.cur.fetchone()\n";
        ofs << "\t\t\tif output is None:\n";
        ofs << "\t\t\t\tbreak\n";
        ofs << "\t\t\tfor j in range(len(self.mf_table)):\n";
        ofs << "\t\t\t\tpivot = True\n";
        ofs << "\t\t\t\tfor item in select_attr:\n";
        ofs << "\t\t\t\t\tif output[item] != self.mf_table[j].group_attr[item]:\n";
        ofs << "\t\t\t\t\t\t\'\'\'\n";
        ofs << "\t\t\t\t\t\t\tCheck x.gourp_attr == group_attr?\n";
        ofs << "\t\t\t\t\t\t\tif !=\n";
        ofs << "\t\t\t\t\t\t'''\n";
        ofs << "\t\t\t\t\t\tpivot = False\n";
        ofs << "\t\t\t\t\t\tbreak\n";
        ofs << "\t\t\t\t\t'''\n";
        ofs << "\t\t\t\t\t\tif all x.group_attr == group_attr\n";
        ofs << "\t\t\t\t\t\tstart to check those select with aggFuncs.\n";
        ofs << "\t\t\t\t\t\'\'\'\n";
        ofs << "\t\t\t\tif pivot is True:\n";
        ofs << "\t\t\t\t\tfor i in self.operate_obj.agg_func_parsed.keys():\n";
        ofs << "\t\t\t\t\t\tif i == 0 or self.check_select_cond(i, j, output):\n";
        ofs << "\t\t\t\t\t\t\tself.update_agg_func_values(i, j, output)\n";
        ofs << "\n";
        ofs << "\tdef project_data(self):\n";
        ofs << "\t\tpt = PrettyTable(self.operate_obj.select_attr_agg_func)\n";
        ofs << "\t\tgroup_attr_lst = self.operate_obj.group_attr.split(\", \")\n";
        ofs << "\t\tfor line in self.mf_table:\n";
        ofs << "\t\t\trow_lst = []\n";
        ofs << "\t\t\tif self.check_having_cond(line):\n";
        ofs << "\t\t\t\tfor item in self.operate_obj.select_attr_parsed:\n";
        ofs << "\t\t\t\t\tif item in group_attr_lst:\n";
        ofs << "\t\t\t\t\t\trow_lst.append(line.group_attr[item])\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tfunc_name, var = postgresCon.unpack_agg_func(item)\n";
        ofs << "\t\t\t\t\t\tvar = var.split(\".\")\n";
        ofs << "\t\t\t\t\t\tkey = var[0] + \".\" + func_name + \"({})\".format(var[1])\n";
        ofs << "\t\t\t\t\t\trow_lst.append(line.agg_func[key])\n";
        ofs << "\t\t\t\tpt.add_row(row_lst)\n";
        ofs << "\t\treturn pt\n";
        ofs << "\n";
        ofs << "\tdef check_having_cond(self, line : MFTable):\n";
        ofs << "\t\thaving_conds = self.operate_obj.having_conds\n";
        ofs << "\t\treverse_polish_having_conds = postgresCon.parse_bool_expression(having_conds)\n";
        ofs << "\t\tlast_ele = []\n";
        ofs << "\t\tbool_ele = []\n";
        ofs << "\t\tagg_func_line = line.agg_func\n";
        ofs << "\t\tfor item in reverse_polish_having_conds:\n";
        ofs << "\t\t\tif item in postgresCon.operators:\n";
        ofs << "\t\t\t\t\'\'\'\n";
        ofs << "\t\t\t\t\tCalculate bool_ele here\n";
        ofs << "\t\t\t\t\'\'\'\n";
        ofs << "\t\t\t\tif item == \"or\":\n";
        ofs << "\t\t\t\t\tif not (bool_ele[0] or bool_ele[1]):\n";
        ofs << "\t\t\t\t\t\treturn False\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele = [True]\n";
        ofs << "\t\t\t\tif item == \"and\":\n";
        ofs << "\t\t\t\t\tif not (bool_ele[0] and bool_ele[1]):\n";
        ofs << "\t\t\t\t\t\treturn False\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele = [True]\n";
        ofs << "\t\t\telif item in postgresCon.high_operators:\n";
        ofs << "\t\t\t\tif item == \"=\":\n";
        ofs << "\t\t\t\t\tif last_ele[0] != last_ele[1]:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\telif item == \"<\":\n";
        ofs << "\t\t\t\t\tif last_ele[0] < last_ele[1]:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\telif item == \">\":\n";
        ofs << "\t\t\t\t\tif last_ele[0] > last_ele[1]:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\telif item == \"<=\":\n";
        ofs << "\t\t\t\t\tif last_ele[0] <= last_ele[1]:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\telif item == \">=\":\n";
        ofs << "\t\t\t\t\tif last_ele[0] >= last_ele[1]:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\telif item == \"<>\":\n";
        ofs << "\t\t\t\t\tif last_ele[0] == last_ele[1]:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\tlast_ele = []\n";
        ofs << "\t\t\telif item in postgresCon.highest_operators:\n";
        ofs << "\t\t\t\tif item == \"*\":\n";
        ofs << "\t\t\t\t\ttmp = last_ele.pop()\n";
        ofs << "\t\t\t\t\tif tmp in agg_func_line.keys():\n";
        ofs << "\t\t\t\t\t\ttmp = agg_func_line[tmp]\n";
        ofs << "\t\t\t\t\tlast_ele[-1] = last_ele[-1] * tmp\n";
        ofs << "\t\t\t\telif item == \"+\":\n";
        ofs << "\t\t\t\t\ttmp = last_ele.pop()\n";
        ofs << "\t\t\t\t\tif tmp in agg_func_line.keys():\n";
        ofs << "\t\t\t\t\t\ttmp = agg_func_line[tmp]\n";
        ofs << "\t\t\t\t\tlast_ele[-1] = last_ele[-1] + tmp\n";
        ofs << "\t\t\t\telif item == \"-\":\n";
        ofs << "\t\t\t\t\ttmp = last_ele.pop()\n";
        ofs << "\t\t\t\t\tif tmp in agg_func_line.keys():\n";
        ofs << "\t\t\t\t\t\ttmp = agg_func_line[tmp]\n";
        ofs << "\t\t\t\t\tlast_ele[-1] = last_ele[-1] - tmp\n";
        ofs << "\t\t\t\telif item == \"/\":\n";
        ofs << "\t\t\t\t\ttmp = last_ele.pop()\n";
        ofs << "\t\t\t\t\tif tmp in agg_func_line.keys():\n";
        ofs << "\t\t\t\t\t\ttmp = agg_func_line[tmp]\n";
        ofs << "\t\t\t\t\tlast_ele[-1] = last_ele[-1] / tmp\n";
        ofs << "\t\t\telse:\n";
        ofs << "\t\t\t\tif item in agg_func_line.keys():\n";
        ofs << "\t\t\t\t\tlast_ele.append(agg_func_line[item])\n";
        ofs << "\t\t\t\telse:\n";
        ofs << "\t\t\t\t\ttry:\n";
        ofs << "\t\t\t\t\t\titem = int(item)\n";
        ofs << "\t\t\t\t\texcept ValueError:\n";
        ofs << "\t\t\t\t\t\titem = item\n";
        ofs << "\t\t\t\t\t\tlast_ele.append(agg_func_line[item])\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tlast_ele.append(item)\n";
        ofs << "\t\treturn bool_ele.pop()\n";
        ofs << "\n";
        ofs << "\tdef check_select_cond(self, var : int, line_in_table : int, line_data):\n";
        ofs << "\t\tselect_conds = self.operate_obj.select_conds[var]\n";
        ofs << "\t\treverse_polish_select_conds = postgresCon.parse_bool_expression(select_conds)\n";
        ofs << "\t\tlast_ele = []\n";
        ofs << "\t\tbool_ele = []\n";
        ofs << "\t\t\'\'\'\n";
        ofs << "\t\t\tParse X.var in reverse_polish_list\n";
        ofs << "\t\t\'\'\'\n";
        ofs << "\t\tfor item in reverse_polish_select_conds:\n";
        ofs << "\t\t\tif item in postgresCon.operators:\n";
        ofs << "\t\t\t\t\'\'\'\n";
        ofs << "\t\t\t\t\tCalculate bool_ele here\n";
        ofs << "\t\t\t\t'''\n";
        ofs << "\t\t\t\tif item == \"or\":\n";
        ofs << "\t\t\t\t\tif not (bool_ele[0] or bool_ele[1]):\n";
        ofs << "\t\t\t\t\t\treturn False\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele = [True]\n";
        ofs << "\t\t\t\tif item == \"and\":\n";
        ofs << "\t\t\t\t\tif not (bool_ele[0] and bool_ele[1]):\n";
        ofs << "\t\t\t\t\t\treturn False\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tbool_ele = [True]\n";
        ofs << "\t\t\telif item in postgresCon.high_operators:\n";
        ofs << "\t\t\t\t'''\n";
        ofs << "\t\t\t\t\tCalculate bool value of last_ele here.\n";
        ofs << "\t\t\t\t\tassign bool_ele : list here.\n";
        ofs << "\t\t\t\t\'\'\'\n";
        ofs << "\t\t\t\tif item == \"=\":\n";
        ofs << "\t\t\t\t\tif postgresCon.unpack_agg_func(last_ele[1]):\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] != self.mf_table[line_in_table].agg_func[last_ele[1]]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] != last_ele[1]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\telif item == \"<\":\n";
        ofs << "\t\t\t\t\tif postgresCon.unpack_agg_func(last_ele[1]):\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] >= self.mf_table[line_in_table].agg_func[last_ele[1]]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] >= last_ele[1]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\telif item == \">\":\n";
        ofs << "\t\t\t\t\tif postgresCon.unpack_agg_func(last_ele[1]):\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] <= self.mf_table[line_in_table].agg_func[last_ele[1]]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] <= last_ele[1]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\telif item == \"<=\":\n";
        ofs << "\t\t\t\t\tif postgresCon.unpack_agg_func(last_ele[1]):\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] > self.mf_table[line_in_table].agg_func[last_ele[1]]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] > last_ele[1]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\telif item == \">=\":\n";
        ofs << "\t\t\t\t\tif postgresCon.unpack_agg_func(last_ele[1]):\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] < self.mf_table[line_in_table].agg_func[last_ele[1]]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] < last_ele[1]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\telif item == \"<>\":\n";
        ofs << "\t\t\t\t\tif postgresCon.unpack_agg_func(last_ele[1]):\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] == self.mf_table[line_in_table].agg_func[last_ele[1]]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\tif line_data[last_ele[0]] == last_ele[1]:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(False)\n";
        ofs << "\t\t\t\t\t\telse:\n";
        ofs << "\t\t\t\t\t\t\tbool_ele.append(True)\n";
        ofs << "\t\t\telse:\n";
        ofs << "\t\t\t\tif item.find(\".\") != -1:\n";
        ofs << "\t\t\t\t\t_, value = item.split(\".\")\n";
        ofs << "\t\t\t\telse:\n";
        ofs << "\t\t\t\t\tvalue = item\n";
        ofs << "\t\t\t\tlast_ele.append(value)\n";
        ofs << "\t\treturn bool_ele.pop()\n";
        ofs << "\n";
        ofs << "\tdef update_agg_func_values(self, var : int, line_in_table : int, line_data):\n";
        ofs << "\t\t\'\'\'\n";
        ofs << "\t\t\t@param: var - sequence of grouping variables.\n";
        ofs << "\t\t\t@param: line_in_table - the line number in mftable[]\n";
        ofs << "\t\t\t@param: line_data - the data of this row in out table\n";
        ofs << "\t\t\t@var: key_name - x.var or var\n";
        ofs << "\t\t\t@var: agg_funcs - {\"sum(x.var)\" : 0, \"sum(var)\" : 0}\n";
        ofs << "\t\t\t@var: func - func name in agg_funcs e.g. sum()\n";
        ofs << "\t\t\t@var: var_name - var name in agg_funcs e.g. x.var\n";
        ofs << "\t\t\'\'\'\n";
        ofs << "\n";
        ofs << "\t\tagg_funcs = self.operate_obj.agg_func_parsed[var]\n";
        ofs << "\t\tfor agg_func in agg_funcs:\n";
        ofs << "\t\t\tfunc, var_name = postgresCon.unpack_agg_func(agg_func)\n";
        ofs << "\t\t\tif var == 0:\n";
        ofs << "\t\t\t\tkey_name = agg_func\n";
        ofs << "\t\t\telse:\n";
        ofs << "\t\t\t\tkey_name = str(var) + \".\" + agg_func\n";
        ofs << "\t\t\tif self.mf_table[line_in_table].agg_func[key_name] == 0:\n";
        ofs << "\t\t\t\tif func == \'count\':\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] += 1\n";
        ofs << "\t\t\t\telif func == \'avg\':\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] = line_data[var_name]\n";
        ofs << "\t\t\t\t\tpostgresCon.avg_func_dict[line_in_table][\"size\"] = 1.0\n";
        ofs << "\t\t\t\t\tpostgresCon.avg_func_dict[line_in_table][\"value\"] = line_data[var_name]\n";
        ofs << "\t\t\t\telse:\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] = line_data[var_name]\n";
        ofs << "\t\t\telse:\n";
        ofs << "\t\t\t\tif func == \'count\':\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] += 1\n";
        ofs << "\t\t\t\tif func == \'sum\':\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] += line_data[var_name]\n";
        ofs << "\t\t\t\tif func == \'max\':\n";
        ofs << "\t\t\t\t\ttmp = self.mf_table[line_in_table].agg_func[key_name]\n";
        ofs << "\t\t\t\t\ttmp = max(tmp, line_data[var_name])\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] = tmp\n";
        ofs << "\t\t\t\tif func == \'min\':\n";
        ofs << "\t\t\t\t\ttmp = self.mf_table[line_in_table].agg_func[key_name]\n";
        ofs << "\t\t\t\t\ttmp = min(tmp, line_data[var_name])\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] = tmp\n";
        ofs << "\t\t\t\tif func == \'avg\':\n";
        ofs << "\t\t\t\t\tcount = postgresCon.avg_func_dict[line_in_table][\"size\"]\n";
        ofs << "\t\t\t\t\tnum = postgresCon.avg_func_dict[line_in_table][\"value\"]\n";
        ofs << "\t\t\t\t\tnew_avg = (num * count + line_data[var_name]) / (count + 1.0)\n";
        ofs << "\t\t\t\t\tself.mf_table[line_in_table].agg_func[key_name] = new_avg\n";
        ofs << "\t\t\t\t\tpostgresCon.avg_func_dict[line_in_table][\"size\"] += 1.0\n";
        ofs << "\t\t\t\t\tpostgresCon.avg_func_dict[line_in_table][\"value\"] = new_avg\n";
        ofs << "\n";
        ofs << "\t@staticmethod\n";
        ofs << "\tdef unpack_agg_func(agg_func : str):\n";
        ofs << "\t\ttry:\n";
        ofs << "\t\t\tfunc_name, var_name = re.match(r\"^\\s*(\\w+)\\s*\\((.*)\\)\", agg_func).groups()\n";
        ofs << "\t\texcept AttributeError:\n";
        ofs << "\t\t\treturn False\n";
        ofs << "\t\telse:\n";
        ofs << "\t\t\treturn func_name, var_name\n";
        ofs << "\n";
        ofs << "\t@staticmethod\n";
        ofs << "\tdef parse_bool_expression(bool_str : str):\n";
        ofs << "\t\toperator_stack = []\n";
        ofs << "\t\treverse_polish_stack = []\n";
        ofs << "\t\tele_lst = bool_str.split(\" \")\n";
        ofs << "\n";
        ofs << "\t\tfor item in ele_lst:\n";
        ofs << "\t\t\tif item in postgresCon.highest_operators:\n";
        ofs << "\t\t\t\toperator_stack.append(item)\n";
        ofs << "\t\t\telif item in postgresCon.high_operators:\n";
        ofs << "\t\t\t\twhile operator_stack and (operator_stack[-1] not in postgresCon.operators):\n";
        ofs << "\t\t\t\t\treverse_polish_stack.append(operator_stack.pop())\n";
        ofs << "\t\t\t\toperator_stack.append(item)\n";
        ofs << "\t\t\telif item in postgresCon.operators:\n";
        ofs << "\t\t\t\twhile operator_stack:\n";
        ofs << "\t\t\t\t\treverse_polish_stack.append(operator_stack.pop())\n";
        ofs << "\t\t\t\toperator_stack.append(item)\n";
        ofs << "\t\t\telse:\n";
        ofs << "\t\t\t\treverse_polish_stack.append(item)\n";
        ofs << "\t\twhile operator_stack:\n";
        ofs << "\t\t\treverse_polish_stack.append(operator_stack.pop())\n";
        ofs << "\t\treturn reverse_polish_stack\n";
        ofs << "\n";
        ofs << "\tdef closeCur(self):\n";
        ofs << "\t\tif self.cur.closed is False:\n";
        ofs << "\t\t\tself.cur.close()\n";
        ofs << "\n";
        ofs << "\tdef closeDB(self):\n";
        ofs << "\t\tself.closeCur()\n";
        ofs << "\t\tself.conn.close()\n";
    }
    return 0;
}

int PyCode::generateRequirements(std::string cwd) throw()
{
    std::ofstream ofs;
    std::string path = cwd + "/requirements.txt";
    ofs.open(path, std::ios::out);
    if (!ofs.is_open())
    {
        ofs.clear();
        ofs.close();
        throw ParserClassException::FileNotOpenException();
    }
    else
    {
        ofs << "prettytable==0.7.2\n";
        ofs << "psycopg2==2.8.5\n";
    }
}