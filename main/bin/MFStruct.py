from collections import defaultdict

class MFStruct():
    def __main__(self, var : list, aggFuncs : defaultdict(int)):
        '''
            var : list<str> - select attributes names
            aggFuncs<aggFunc, value> : dict<str, int> - aggFuncs in select
        '''
        self.var = var
        self.aggFunc = aggFuncs
