#!/usr/bin/python
import yaml, sys, getopt


def main(argv):
    inputfile = ''
    outputfile = ''
    try:
        opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
        if len(opts) < 2:
            print 'Usage: test.py -i <inputfile> -o <outputfile>'
            sys.exit(2)

    except getopt.GetoptError:
        print 'Usage: test.py -i <inputfile> -o <outputfile>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: test.py -i <inputfile> -o <outputfile>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg
    print 'Input file:', inputfile
    print 'Output file:', outputfile

    with open(inputfile, 'r') as stream:
        try:
            print(yaml.load(stream))
        except yaml.YAMLError as exc:
            print(exc)


if __name__ == "__main__":
    main(sys.argv[1:])
