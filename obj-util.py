import csv,sys,getopt

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

def fix_file_headers(inputfile):
	content = []
	num_headers=0
	num_columns=0
	num_rows=0
	with open(inputfile,'r') as file:
		for line in file:
			content.append(line)
			if ":" in line:
				num_headers+=1
				print line
			if "," in line:
				num_rows+=1
				cols = line.count(",")+1
				if(num_columns==0):
					num_columns=cols
				elif (num_columns!=0 and cols!=num_columns):
					print("!!!!ERROR: inconsistent number of columns:"+inputfile)
	print ("num of row: "+str(num_rows)+" num cols "+str(num_columns))
	if ".var" in inputfile:
		print "var file"
		for i in xrange(0,num_headers+num_rows-1):
			print "vp vs ["+line+"]"+ " -- "+str(i)
			if "vp" in content[i]:
				print "replacing vp"
				content[i]="vp: "+str(num_columns)+"\n"
			if "non" in content[i]:
				content[i]="non: "+str(num_rows)+"\n"
	with open(inputfile,'w') as file:
		for line in content:
			file.write(line)	
			print line

def append_column(inputfile, num, row_interest):
	rowi=0
	reader = csv.reader(open(inputfile ))
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
	fix_file_headers(inputfile)
		
def main(argv):
	inputfile = ''
	replace_with=""
	col=1
	row_interest=-1
	rowi=0
	validate=0
	replace_col=0#set by R -> calls change_all_rows
	append_col=0
	help=0

	try:
		opts, args = getopt.getopt(argv,"f:r:c:aw:RAV",[])
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
		elif opt == "-V":
			validate=1
	#print inputfile+" "+replace_with+" "+row_interest
	if(help==1):
		print "-f input file"
		print "-r row of interest"
		print "-c column of interest"
		print "-a affect all rows"
		print "-w replace with"
		print "-R Replace column(s) - modified with either -r or -a"
		print "-A append an column(S) to end of all rows"
		print "-V validate header of file"
		print "-h print help"
		

			
	if(replace_col==1):
		change_all_rows(inputfile,replace_with,col,row_interest)
	if(append_col==1):
		append_column(inputfile, 1,row_interest)
	if(validate==1):
		fix_file_headers(inputfile)
	
if __name__ == "__main__":
   main(sys.argv[1:])
