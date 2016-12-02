import csv

file_i=open('./obj/obj_0.var','rb')
file_o=open('./obj/obj_0.var','wb')
reader=csv.reader(file_i)
writer=csv.writer(file_o)

replace_with="asdfasdfasdf"
col=1

for row in reader:
	print row
	if len(row) != 1:
		row[col]=replace_with
		print row
		writer.writerow(row)
	else:
		print row
		writer.writerow(row)

