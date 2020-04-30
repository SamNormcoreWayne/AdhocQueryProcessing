from MFStruct import MFStruct, operateClass
from postgresCon import postgresCon

def main():
    selectAttr = "cust, 1_sum_quant, 2_sum_quant, 3_sum_quant"
    havingCond = "1_sum_quant > 2 * 2_sum_quant || 1_avg_quant > 3_avg_quant"
    size = 3
    mf_struct = operateClass(selectAttr, havingCond, size)
    mf_struct.get_mf_struct(MFStruct("cust", '[{"aggFunc": ["dic", "tionary"]}]', '[{"select_cond": ["dic", "tionary"]}]'))
    '''
        TODO: parse Input and connect to DB
    '''

    conn = postgresCon("usrname", "password")
if __name__ == "__main__":
    main()
