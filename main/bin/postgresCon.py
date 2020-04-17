import psycopg2 as pstgre


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

    def execSql(self):
        cur = self.conn.cursor()
        for sql in self.sqlLst:
            cur.execute(sql)
        # TODO: store the output in a proper way
        cur.close()

    def optHaving(self):
        None

    def outputSQL(self):
        None

    def closeDB(self):
        self.conn.close()
