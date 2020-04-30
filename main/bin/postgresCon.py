import psycopg2 as pstgre
from collections import defaultdict
from MFStruct import operateClass, MFTable

'''
    Algorithm 2.1 in the paper.
'''

class postgresCon():
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
            self.cur = self.conn.cursor()
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
                        self.check_select_cond(i, j)
                '''
                    获取数据，判断条件
                '''

    def check_select_cond(self, var : int, line_in_table : int):
        agg_func = self.operate_obj.mfstruct.agg_func_parsed[lin]
        

    def closeCur(self):
        if self.cur.closed is False:
            self.cur.close()

    def closeDB(self):
        self.closeCur()
        self.conn.close()
