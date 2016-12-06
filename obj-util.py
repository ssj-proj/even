import csv,sys,getopt

def getVarFromFile(filename):
    import imp
    f = open(filename)
    global data
    data = imp.load_source('data', '', f)
    f.close()

def main(argv):
	inputfile = ''
	replace_with=""
	col=1
	row_interest=-1
	rowi=0

	try:
		opts, args = getopt.getopt(argv,"f:r:c:aw:",[])
	except getopt.GetoptError:
		print 'test.py -i <inputfile> -o <outputfile>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'test.py -i <inputfile> -o <outputfile>'
			sys.exit()
		elif opt == "-f":
			inputfile = arg
		elif opt == "-r":
			row_interest = int(arg)
		elif opt == "-c":
			col = int(arg)
		elif opt == "-a":
			row_interest=-1
		elif opt == "-w":
			replace_with=arg

	#print inputfile+" "+replace_with+" "+row_interest
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
		print str(rowi)+" "+str(row_interest)

	writer = csv.writer(open(inputfile, 'w'))
	writer.writerows(lines)

if __name__ == "__main__":
   main(sys.argv[1:])
