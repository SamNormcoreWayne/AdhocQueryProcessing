from MFStruct import operateClass
from postgresCon import postgresCon

def main():
    selectAttr = "cust, 1_sum_quant, 2_sum_quant, 3_sum_quant"
    havingCond = "1_sum_quant > 2 * 2_sum_quant || 1_avg_quant > 3_avg_quant"
    group_attr = "incoming gourp_attr"
    agg_func = "incoming agg_func"
    select_cond = "incoming select_cond"
    size = 3
    mf_struct = operateClass(selectAttr, havingCond, size, group_attr, agg_func, select_cond)
    '''
        TODO: parse Input and connect to DB
    '''

    conn = postgresCon("usrname", "password", mf_struct)
    conn.get_group_attr_data()
    conn.main_algo()
    conn.closeDB()
    # conn.show_data()
if __name__ == "__main__":
    main()
