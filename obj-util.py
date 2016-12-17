import csv,sys,getopt
#TODO - update file headers

def change_all_rows(inputfile, replace_with,col,row_interest):
	rowi=0
	reader = csv.reader(open(inputfile )) # Here your csv file
	lines = [l for l in reader]

	for row in lines:
		if ":" in row[0]:
			continue
		if row_interest == -1 or rowi==row_interest:
			row[col]=replace_with
			print row
		else:
			print row
		rowi+=1

	writer = csv.writer(open(inputfile, 'w'))
	writer.writerows(lines)
def append_column(inputfile, num, row_interest):
	rowi=0
	reader = csv.reader(open(inputfile )) # Here your csv file
	lines = [l for l in reader]

	for row in lines:
		if ":" in row[0]:
			continue
		if row_interest == -1 or rowi==row_interest:
			row+="0"
			print row
		else:
			print row
		rowi+=1

	writer = csv.writer(open(inputfile, 'w'))
	writer.writerows(lines)

def main(argv):
	inputfile = ''
	replace_with=""
	col=1
	row_interest=-1
	rowi=0

	replace_col=0#set by R -> calls change_all_rows
	append_col=0
	help=0

	try:
		opts, args = getopt.getopt(argv,"f:r:c:aw:RA",[])
	except getopt.GetoptError:
		print 'test.py -i <inputfile> -o <outputfile>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == "-f":
			inputfile = arg
		elif opt == "-r":
			row_interest = int(arg)
		elif opt == "-c":
			col = int(arg)
		elif opt == "-a":
			row_interest=-1
		elif opt == "-w":
			replace_with=arg
		elif opt == "-R":
			replace_col=1
		elif opt == "-A":
			append_col=1
		elif opt == "-h":
			help=1
	#print inputfile+" "+replace_with+" "+row_interest
	if(help==1):
		print "-f input file"
		print "-r row of interest"
		print "-c column of interest"
		print "-a affect all rows"
		print "-w replace with"
		print "-R Replace column(s) - modified with either -r or -a"
		print "-A append an column(S) to end of all rows"
		print "-h print help"
		

			
	if(replace_col==1):
		change_all_rows(inputfile,replace_with,col,row_interest)
	if(append_col==1):
		append_column(inputfile, 1,row_interest)
	
if __name__ == "__main__":
   main(sys.argv[1:])
