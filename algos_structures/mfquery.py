'''
select cust, sum(x.quant), sum(y.quant), sum(z.quant)
    from sales
    group by cust: x, y, z
    such that x.state = ‘NY’
        and y.state = ‘NJ’
        and z.state = ‘CT’
    having sum(x.quant) > 2 * sum(y.quant) or avg(x.quant) > avg(z.quant);
'''
'''
select cust, sum(q)
    from sales
    group by cust;
'''

class mfvar:
    cust_var : str # x, y, z
    sum_quant : list

class mfquery:
    def __init__(self):
        '''
            class variables
        '''
        self.tablename : str = "sales" # static const
        self.var_list : list = []# 500 different customers are stored here. 
    def queryRunner(self, custID : int, sum_var : str):
        '''
            connect to the DB, run:
                select cust, sum(q)
                    from sales
                    group by cust;
            store the sums into list. 
        '''
    @staticmethod
    def checkHavingConds(cust_obj : mfvar) -> bool:
        '''
            for every cust,
            check having conditions.
        '''

    def output(self):
        outputLst = []
        for mfvar in self.var_list:
            if mfquery.checkHavingConds(mfvar):
                outputLst.append(mfvar)
        self.var_list = outputLst
