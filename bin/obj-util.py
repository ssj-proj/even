#!/usr/bin/env python

from string import printable
from curses import erasechar, wrapper, KEY_BACKSPACE
from evepy import even
import csv,sys,getopt,threading

PRINTABLE = map(ord, printable)


#  --TODO--
#   *validate that num of neurs are consistent accross all obj files
#   *better way to manipulate files and objects
#   *random change obj (mutate)
class console_vars:
	obj_dir=""
	obj_number=""
	cfg_type=""
vars = console_vars()
console_mode = 0
lines = []
console_screen=0
max_lines = 0
eve = even()

def start_eve():
	steve_thread = threading.Thread(target=eve.start())
	
def stop_eve():
	eve.halt()


def change_all_rows(inputfile, replace_with,col,row_interest):
	rowi=0
	reader = csv.reader(open(inputfile ))
	lines = [l for l in reader]

	for row in lines:
		if ":" in row[0]:
			continue
		if row_interest == -1 or rowi==row_interest:
			row[col]=replace_with
			printc(row)
		else:
			printc(row)
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
				printc(line)
			if "," in line:
				num_rows+=1
				cols = line.count(",")+1
				if(num_columns==0):
					num_columns=cols
				elif (num_columns!=0 and cols!=num_columns):
					printc("!!!!ERROR: inconsistent number of columns:"+inputfile)
	printc("num of row: "+str(num_rows)+" num cols "+str(num_columns))
	if ".var" in inputfile:
		printc("var file")
		for i in xrange(0,num_headers+num_rows-1):
			printc("vp vs ["+line+"]"+ " -- "+str(i))
			if "vp" in content[i]:
				printc("replacing vp")
				content[i]="vp: "+str(num_columns)+"\n"
			if "non" in content[i]:
				content[i]="non: "+str(num_rows)+"\n"
	with open(inputfile,'w') as file:
		for line in content:
			file.write(line)	
			printc(line)

def append_column(inputfile, num, row_interest):
	rowi=0
	reader = csv.reader(open(inputfile ))
	lines = [l for l in reader]

	for row in lines:
		if ":" in row[0]:
			continue
		if row_interest == -1 or rowi==row_interest:
			row+="0"
			printc(row)
		else:
			printc(row)
		rowi+=1

	writer = csv.writer(open(inputfile, 'w'))
	writer.writerows(lines)
	fix_file_headers(inputfile)
def input(stdscr):
    ERASE = input.ERASE = getattr(input, "ERASE", ord(erasechar()))
    Y, X = stdscr.getyx()
    s = []

    while True:
        c = stdscr.getch()

        if c in (13, 10):
            break
        elif c == ERASE or c==KEY_BACKSPACE:
            y, x = stdscr.getyx()
            if x > X:
                del s[-1]
                stdscr.move(y, (x - 1))
                stdscr.clrtoeol()
                stdscr.refresh()
        elif c in PRINTABLE:
            s.append(chr(c))
            stdscr.addch(c)

    return "".join(s)
def get_file():
   if(len(vars.obj_dir) == 0 or len(vars.obj_number) == 0 or len(vars.cfg_type) == 0):
     return "ERROR:get_file:Unable to get file"
   return vars.obj_dir+"obj_"+vars.obj_number+"."+vars.cfg_type
def prompt(stdscr, y, x, prompt=">>> "):
    stdscr.move(y, x)
    stdscr.clrtoeol()
    stdscr.addstr(y, x, prompt)
    return input(stdscr)
def process_input(line):
	line = line.lower()
	cmd = line.split()
	if cmd[0] == "replace":
		if cmd[1] == "col":
			col=int(cmd[2])
			rep_with = cmd[4]
			file = get_file()
		        if "ERROR" in file:
				return file
			if "row" in line:
				row=int(cmd[6])
			else:
				row=-1
	      		change_all_rows(file,rep_with,col,row)
			return "replacing column"+cmd[2]+" with "+rep_with+" in file "+file
	if cmd[0] == "use":
		if cmd[1] == "obj":
			vars.obj_number = cmd[2]
			return "using object "+vars.obj_number
		elif cmd[1] == "dir":
			vars.obj_dir = cmd[2]
			return "base directory set to "+vars.obj_dir
		elif cmd[1] == "type":
			vars.cfg_type = cmd[2]
			return "config type "+vars.cfg_type
	if cmd[0] == "steve":
		start_eve()
		return "config type "+vars.cfg_type
	return "Invalid command"
def printc(display):
	if console_mode==1:
		global lines
		global console_screen
		global max_lines
		# scroll
		if len(lines) > max_lines:
			lines = lines[1:]
			console_screen.clear()
			for i, line in enumerate(lines):
				console_screen.addstr(i, 0, line)

		console_screen.addstr(len(lines), 0, display)
		lines.append(display)

		console_screen.refresh()
	else:
		print(display)

def console(stdscr):
    global console_mode
    console_mode=1
    global console_screen
    console_screen=stdscr
    global max_lines

    stdscr.clear()
    with open('util.cfg') as my_file:
        for line in my_file:
            process_input(line)

    while True:
	Y, X = stdscr.getmaxyx()
    	max_lines= (Y - 3)
        input = prompt(stdscr, (Y - 1), 0)  # noqa
        if input == ":q":
            break

	display=process_input(input)

	printc(display)

		
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
	no_args=1
	try:
		opts, args = getopt.getopt(argv,"f:r:c:aw:RAV",[])
	except getopt.GetoptError:
		printc('test.py -i <inputfile> -o <outputfile>')
		sys.exit(2)
	for opt, arg in opts:
		no_args=0
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
	#printc(inputfile+" "+replace_with+" "+row_interest
	if(help==1):
		printc("-f input file")
		printc("-r row of interest")
		printc("-c column of interest")
		printc("-a affect all rows")
		printc("-w replace with")
		printc("-R Replace column(s) - modified with either -r or -a")
		printc("-A append an column(S) to end of all rows")
		printc("-V validate header of file")
		printc("-h printc(help")
		

	if(no_args==1):
		wrapper(console)		
	if(replace_col==1):
		change_all_rows(inputfile,replace_with,col,row_interest)
	if(append_col==1):
		append_column(inputfile, 1,row_interest)
	if(validate==1):
		fix_file_headers(inputfile)
	
if __name__ == "__main__":
   main(sys.argv[1:])
