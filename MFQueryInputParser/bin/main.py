from prettytable import PrettyTable
from MFStruct import operateClass
from postgresCon import postgresCon

def main():
	selectAttr = "cust, prod, month, 1_sum_quant, 2_sum_quant, 3_sum_quant"
	havingCond = "1.sum(quant) > 2 * 2.sum(quant) or 1.avg(quant) > 3.avg(quant)"
	group_attr = ",".join(["cust, prod, month"])
	agg_func = dict([[0,"sum_quant"],[1,"1_sum_quant, 1_avg_quant"],[2,"2_sum_quant"],[3,"3_sum_quant, 3_avg_quant"]])
	select_cond = dict([[1,"1.state = NY and 1.state = sum(quant)"],[2,"2.state = NJ"],[3,"3.state = CT"],[67,"sum_quant, 1_sum_quant, 1_avg_quant, 2_sum_quant, 3_sum_quant, 3_avg_quant"]])
	for key in agg_func:
		agg_func[key] = agg_func[key].split(", ")
	size = 3
	mf_struct = operateClass(selectAttr, havingCond, size, group_attr, agg_func, select_cond)
	mf_struct.parse_agg_group()
	mf_struct.parse_select_attr()

	usrname = input("Input username for connecting DB: ")
	pwd = input("Input pwd for connecting DB")
	host = input("Input DB address: ")
	conn = postgresCon(usrname, pwd, host, mf_struct)
	conn.get_group_attr_data()
	conn.main_algo()
	conn.closeDB()
	output = conn.project_data()
	print(output)


if __name__ == "__main__":
	main()
