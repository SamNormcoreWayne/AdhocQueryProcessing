import psycopg2 as pstgre

'''
    Algorithm 2.1 in the paper.
'''

class postgresCon():
    def __main__(self, user, pwd, db_name='sales', host='localhost', port='5432'):
        self.db_name = db_name
        self.user = user
        self.pwd = pwd
        self.host = host
        self.port = port
        self.conn = pstgre.connect(host=host, database=db_name, user=user, password=pwd, port=port)
        self.sqlLst = []
        self.having_conds = []

    def getSql(self, sqlStr : str):
        self.sqlLst.append(sqlStr)

    def execSql(self, output=None):
        self.cur = self.conn.cursor()
        for sql in self.sqlLst:
            try:
                self.cur.execute(sql)
            except(pstgre.DatabaseError) as dberr:
                print("Error while execute query: \n '{}'\n".format(sql), dberr)

            finally:
                if output is not None:
                    output = self.conn.fetchone()
                    # this is unfinished.
        # TODO: store the output in a proper way

    def optHaving(self):
        None

    def outputSQL(self):
        None

    def closeDB(self):
        self.conn.close()
