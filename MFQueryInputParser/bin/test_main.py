from prettytable import PrettyTable
from MFStruct import operateClass
from postgresCon import postgresCon

def main():
	selectAttr = "cust, 1_sum_quant, 2_sum_quant, 3_sum_quant"
	havingCond = "1_sum_quant > 2 * 2_sum_quant or 1_avg_quant > 3_avg_quant"
	group_attr = "cust"
	agg_func = dict([[1,"1_sum_quant, 1_avg_quant"],[2,"2_sum_quant"],[3,"3_sum_quant, 3_avg_quant"]])
	select_cond = dict([[1,"1.state=’NY’"],[2,"2.state=’NJ’"],[3,"3.state=’CT’"]])
	size = 3
	mf_struct = operateClass(selectAttr, havingCond, size, group_attr, agg_func, select_cond)
	'''
		TODO: parse Input and connect to DB
	'''

	conn = postgresCon("usrname", "password", mf_struct)
	conn.get_group_attr_data()
	conn.main_algo()
	conn.closeDB()
	output = conn.project_data()
	print(output)


if __name__ == "__main__":
	main()
