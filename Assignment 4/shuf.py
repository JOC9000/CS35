import argparse, sys, random

#put functions and classes here
def i_option(input_range, limit, repeat):
    start = 0
    stop = 0

    rangelist = input_range.split("-")
    count = 0
    for word in rangelist:
        if count == 0:
            start = int(word)
        elif count == 1:
            stop = int(word)
        else:
            print("error! Too many numbers in option")
            sys.exit(1)
        count = count + 1
    
    if 0 > start or start > stop:
        print("error! LO must be greater than 0 and HI must be greater than LO.")
        sys.exit(2)
        
    hilolist = []
    while start <= stop:
        hilolist.append(start)
        start = start + 1
    
    printResult(hilolist, limit, repeat)

            
def file_option(limit, repeat):
    if args.FILE == "-":
        holdsLines = sys.stdin.readlines()
    else: 
        f = open(args.FILE,"r")
        holdsLines = f.readlines()
        f.close
    
    holdsNoNewLines = []
    for line in holdsLines:
        holdsNoNewLines.append(line.replace("\n", ""))

    printResult(holdsNoNewLines, limit, repeat)


def printResult(printlist, limit, repeat):
    if repeat == False:
        random.shuffle(printlist)
        iterate = 0
        if limit == None:
            limit = len(printlist)
        while iterate < len(printlist) and iterate < limit:
            print(printlist[iterate])
            iterate = iterate + 1
    else:
        if limit != None:
            iterate = 0
            while iterate < limit:
                print(random.choice(printlist))
                iterate = iterate + 1
        else:
            while limit == None:
                print(random.choice(printlist))


#start of main function
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group()
    group.add_argument("FILE", help = "filename to be inspected, if empty defaults to -, reading from standard input", default = "-", nargs = '?')
    group.add_argument("-i", "--input-range", help="treat each answer LO through HI as an input line")
    parser.add_argument("-n", "--head-count", type = int, help = "output at most COUNT lines")
    parser.add_argument("-r", "--repeat", action = "store_true", help = "ouput lines can be repeated")
    args = parser.parse_args()
    holdsLines=""
    if args.input_range != None:
        i_option(args.input_range, args.head_count, args.repeat)
    else:
        file_option(args.head_count, args.repeat)
        
