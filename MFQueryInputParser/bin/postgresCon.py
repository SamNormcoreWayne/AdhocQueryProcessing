import psycopg2 as pstgre
import psycopg2.extras
import re
from collections import defaultdict
from enum import Enum
from MFStruct import operateClass, MFTable
from prettytable import PrettyTable

'''
	Algorithm 3.1 in the paper.
'''

class aggfunc_enum(Enum):
	SUM = "sum"
	MAX = "max"
	MIN = "min"
	COUNT = "count"
	AVG = "avg"

class postgresCon():
	avg_func_dict = dict()
	operators = ["and", "or"]
	high_operators = ["=", "<", ">", "<>", "<=", ">="]
	def __init__(self, user, pwd, host, obj : operateClass):
		db_name = 'sales_db'
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
		output = ()
		if self.cur is None:
			self.cur = self.conn.cursor(cursor_factory=pstgre.extras.DictCursor)
		select_attr = self.operate_obj.group_attr
		select_attr_lst = select_attr.split(",")
		self.cur.execute("SELECT {} FROM sales GROUP BY {} ORDER BY {};".format(select_attr, select_attr, select_attr_lst[0]))
		while(True):
			try:
				output = self.cur.fetchone()
			except(pstgre.ProgrammingError) as dberr:
				print("Error while execute query in get_gourp_attr_data: ", dberr)
			finally:
				if output is None:
					break
				mftable = MFTable(select_attr_lst)
				for var_name in select_attr_lst:
					mftable.group_attr[var_name] = output[var_name]
				self.mf_table.append(mftable)

	def main_algo(self):
		select_attr = self.operate_obj.group_attr.split(", ")
		for i in self.operate_obj.agg_func_parsed.keys():
			if self.cur is None:
				self.cur = self.conn.cursor()
			self.cur.execute("SELECT * FROM sales;")
			while(True):
				output = self.cur.fetchone()
				if output is None:
					break
				for j in range(len(self.mf_table)):
					pivot = True
					for item in select_attr:
						if output[item] != self.mf_table[j].group_attr[item]:
							'''
								Check x.gourp_attr == group_attr?
								if !=
							'''
							pivot = False
							break
					'''
						if all x.group_attr == group_attr
						start to check those select with aggFuncs.
					'''
					if pivot is True:
						if(self.check_select_cond(i, j, output)):
							self.update_agg_func_values(i, j, output)
						break

	def project_data(self):
		pt = PrettyTable(self.operate_obj.select_attr_agg_func)
		group_attr_lst = self.operate_obj.group_attr.split(", ")
		for line in self.mf_table:
			row_lst = []
			if self.check_having_cond(line):
				for item in self.operate_obj.select_attr_parsed:
					if item in group_attr_lst:
						row_lst.append(line.group_attr[item])
					else:
						func_name, var = postgresCon.unpack_agg_func(item)
						var = var.split(".")
						key = var[0] + "." + func_name + "({})".format(var[1])
						row_lst.append(line.agg_func[key])
				pt.add_row(row_lst)
		return pt

	def check_having_cond(self, line : MFTable):
		having_conds = self.operate_obj.having_conds
		reverse_polish_having_conds = postgresCon.parse_bool_expression(having_conds)
		last_ele = []
		bool_ele = []
		agg_func_line = line.agg_func
		for item in reverse_polish_having_conds:
			if item in postgresCon.operators:
				'''
					Calculate bool_ele here
				'''
				if item == "or":
					if not (bool_ele[0] or bool_ele[1]):
						return False
					else:
						bool_ele = [True]
				if item == "and":
					if not (bool_ele[0] and bool_ele[1]):
						return False
					else:
						bool_ele = [True]
			elif item in postgresCon.high_operators:
				if item == "=":
					if agg_func_line[last_ele[0]] != agg_func_line[last_ele[1]]:
						bool_ele.append(False)
					else:
						bool_ele.append(True)
				elif item == "<":
					if agg_func_line[last_ele[0]] < agg_func_line[last_ele[1]]:
						bool_ele.append(True)
					else:
						bool_ele.append(False)
				elif item == ">":
					if agg_func_line[last_ele[0]] > agg_func_line[last_ele[1]]:
						bool_ele.append(True)
					else:
						bool_ele.append(False)
				elif item == "<=":
					if agg_func_line[last_ele[0]] <= agg_func_line[last_ele[1]]:
						bool_ele.append(True)
					else:
						bool_ele.append(False)
				elif item == ">=":
					if agg_func_line[last_ele[0]] >= agg_func_line[last_ele[1]]:
						bool_ele.append(True)
					else:
						bool_ele.append(False)
				elif item == "<>":
					if agg_func_line[last_ele[0]] == agg_func_line[last_ele[1]]:
						bool_ele.append(False)
					else:
						bool_ele.append(True)
				last_ele = []
			else:
				last_ele.append(item)
		return bool_ele.pop()

	def check_select_cond(self, var : int, line_in_table : int, line_data):
		select_conds = self.operate_obj.select_conds[var]
		reverse_polish_select_conds = postgresCon.parse_bool_expression(select_conds)
		last_ele = []
		bool_ele = []
		'''
			Parse X.var in reverse_polish_list
		'''
		for item in reverse_polish_select_conds:
			if item in postgresCon.operators:
				'''
					Calculate bool_ele here
				'''
				if item == "or":
					if not (bool_ele[0] or bool_ele[1]):
						return False
					else:
						bool_ele = [True]
				if item == "and":
					if not (bool_ele[0] and bool_ele[1]):
						return False
					else:
						bool_ele = [True]
			elif item in postgresCon.high_operators:
				'''
					Calculate bool value of last_ele here.
					assign bool_ele : list here.
				'''
				if item == "=":
					if postgresCon.unpack_agg_func(last_ele[1]):
						if line_data[last_ele[0]] != self.mf_table[line_in_table].agg_func[last_ele[1]]:
							bool_ele.append(False)
						else:
							bool_ele.append(True)
					else:
						if line_data[last_ele[0]] != last_ele[1]:
							bool_ele.append(False)
						else:
							bool_ele.append(True)
			else:
				if item.find(".") != -1:
					_, value = item.split(".")
				else:
					value = item
				last_ele.append(value)
		return True

	def update_agg_func_values(self, var : int, line_in_table : int, line_data):
		'''
			@param: var - sequence of grouping variables.
			@param: line_in_table - the line number in mftable[]
			@param: line_data - the data of this row in out table
			@var: key_name - x.var or var
			@var: agg_funcs - {"sum(x.var)" : 0, "sum(var)" : 0}
			@var: func - func name in agg_funcs e.g. sum()
			@var: var_name - var name in agg_funcs e.g. x.var
		'''

		agg_funcs = self.operate_obj.agg_func_parsed[var]
		for agg_func in agg_funcs:
			func, var_name = postgresCon.unpack_agg_func(agg_func)
			if var == 0:
				key_name = agg_func
			else:
				key_name = str(var) + "." + agg_func
			if self.mf_table[line_in_table].agg_func[key_name] == 0:
				if func == aggfunc_enum.COUNT:
					self.mf_table[line_in_table].agg_func[key_name] += 1
				else:
					self.mf_table[line_in_table].agg_func[key_name] = line_data[var_name]
			else:
				if func == aggfunc_enum.COUNT:
					self.mf_table[line_in_table].agg_func[key_name] += 1
				if func == aggfunc_enum.SUM:
					self.mf_table[line_in_table].agg_func[key_name] += line_data[var_name]
				if func == aggfunc_enum.MAX:
					tmp = self.mf_table[line_in_table].agg_func[key_name]
					tmp = max(tmp, line_data[var_name])
					self.mf_table[line_in_table].agg_func[key_name] = tmp
				if func == aggfunc_enum.MIN:
					tmp = self.mf_table[line_in_table].agg_func[key_name]
					tmp = min(tmp, line_data[var_name])
					self.mf_table[line_in_table].agg_func[key_name] = tmp
				if func == aggfunc_enum.AVG:
					count = postgresCon.avg_func_dict[key_name]["size"]
					num = postgresCon.avg_func_dict[key_name]["value"]
					new_avg = (num * count + line_data[var_name]) / (count + 1)
					self.mf_table[line_in_table].agg_func[key_name] = new_avg
					postgresCon.avg_func_dict[key_name]["size"] += 1
					postgresCon.avg_func_dict[key_name]["value"] = new_avg

	@staticmethod
	def unpack_agg_func(agg_func : str):
		try:
			func_name, var_name = re.match(r"^\s*(\w+)\s*\((.*)\)", agg_func).groups()
		except AttributeError:
			return False
		else:
			return func_name, var_name

	@staticmethod
	def parse_bool_expression(bool_str : str):
		operator_stack = []
		reverse_polish_stack = []
		ele_lst = bool_str.split(" ")

		for item in ele_lst:
			if item in postgresCon.high_operators:
				operator_stack.append(item)
			elif item in postgresCon.operators:
				while operator_stack:
					reverse_polish_stack.append(operator_stack.pop())
				operator_stack.append(item)
			else:
				reverse_polish_stack.append(item)
		while operator_stack:
			reverse_polish_stack.append(operator_stack.pop())
		return reverse_polish_stack

	def closeCur(self):
		if self.cur.closed is False:
			self.cur.close()

	def closeDB(self):
		self.closeCur()
		self.conn.close()
