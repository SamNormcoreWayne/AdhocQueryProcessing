import psycopg2 as pstgre
import re
from collections import defaultdict
from enum import Enum
from MFStruct import operateClass, MFTable

'''
    Algorithm 2.1 in the paper.
'''

class aggfunc_enum(Enum):
    SUM = "sum"
    MAX = "max"
    MIN = "min"
    COUNT = "count"
    AVG = "avg"

class postgresCon():
    avg_func_dict = dict()
    def __init__(self, user, pwd, obj : operateClass):
        db_name = 'sales'
        host = 'localhost'
        port = '5432'
        self.db_name = db_name
        self.user = user
        self.pwd = pwd
        self.host = host
        self.port = port
        self.conn = pstgre.connect(host=host, database=db_name, user=user, password=pwd, port=port)
        self.sql_dict = defaultdict(str)
        self.operate_obj = obj
        self.mf_table = []
        self.cur = None

    def get_group_attr_data(self):
        if self.cur is None:
            self.cur = self.conn.cursor(cursor_factory=pstgre.extras.DictCursor)
        while(True):
            try:
                select_attr = self.operate_obj.mfstruct.group_attr
                self.cur.execute("SELECT {} FROM sales GROUP BY {} ORDER BY {};".format(select_attr, select_attr, select_attr[0]))
                output = self.cur.fetchone()
            except(pstgre.ProgrammingError) as dberr:
                print("Error while execute query in get_gourp_attr_data: ", dberr)
            finally:
                if output is None:
                    break
                mftable = MFTable(select_attr)
                for var_name in select_attr:
                    mftable.group_attr[var_name] = output[var_name]
                self.mf_table.append(mftable)

    def main_algo(self):
        select_attr = self.operate_obj.mfstruct.group_attr
        for i in range(len(self.operate_obj.mfstruct.agg_func_parsed)):
            if self.cur is None:
                self.cur = self.conn.cursor()
            while(True):
                self.cur.execute("SELECT * FROM sales;")
                output = self.cur.fetchone()
                for j in range(len(self.mf_table)):
                    pivot = True
                    for item in select_attr:
                        if output[item] != self.mf_table[j].group_attr[item]:
                            pivot = False
                            break
                    if pivot is True:
                        self.check_select_cond(i, j, output)
                '''
                    获取数据，判断条件
                '''

    def check_select_cond(self, var : int, line_in_table : int, line_data):
        select_conds = self.operate_obj.select_conds[var]
        agg_funcs = self.operate_obj.mfstruct.agg_func_parsed[var]
        for cond in select_conds:
            try:
                line_data[cond]
            except KeyError as ke:
                print(ke)
            finally:
                None

    def update_agg_func_values(self, var : int, line_in_table : int, line_data):
        agg_funcs = self.operate_obj.mfstruct.agg_func_parsed[var]
        for agg_func in agg_funcs:
            func, var_name = postgresCon.unpack_agg_func(agg_func)
            if self.mf_table[line_in_table].agg_func[agg_func] == 0:
                if func == aggfunc_enum.COUNT:
                    self.mf_table[line_in_table].agg_func[agg_func] += 1
                else:
                    self.mf_table[line_in_table].agg_func[agg_func] = line_data[var_name]
            else:
                if func == aggfunc_enum.COUNT:
                    self.mf_table[line_in_table].agg_func[agg_func] += 1
                if func == aggfunc_enum.SUM:
                    self.mf_table[line_in_table].agg_func[agg_func] += line_data[var_name]
                if func == aggfunc_enum.MAX:
                    tmp = self.mf_table[line_in_table].agg_func[agg_func]
                    tmp = max(tmp, line_data[var_name])
                    self.mf_table[line_in_table].agg_func[agg_func] = tmp
                if func == aggfunc_enum.MIN:
                    tmp = self.mf_table[line_in_table].agg_func[agg_func]
                    tmp = min(tmp, line_data[var_name])
                    self.mf_table[line_in_table].agg_func[agg_func] = tmp
                if func == aggfunc_enum.AVG:
                    count = postgresCon.avg_func_dict[agg_func]["size"]
                    num = postgresCon.avg_func_dict[agg_func]["value"]
                    new_avg = (num * count + line_data[var_name]) / (count + 1)
                    self.mf_table[line_in_table].agg_func[agg_func] = new_avg
                    postgresCon.avg_func_dict[agg_func]["size"] += 1
                    postgresCon.avg_func_dict[agg_func]["value"] = new_avg

    @staticmethod
    def unpack_agg_func(agg_func : str):
        var_name = re.search(r"\((.*)\)", agg_func)
        func_name = re.search(r".+?(\()", agg_func)
        return func_name, var_name

    def closeCur(self):
        if self.cur.closed is False:
            self.cur.close()

    def closeDB(self):
        self.closeCur()
        self.conn.close()
