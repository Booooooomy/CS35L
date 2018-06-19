#!/usr/bin/python

import random, sys
from optparse import OptionParser

class shuf:
    def __init__(self, args, echo, count, repeat):
        self.arguments = args
        self.repeat = repeat

        if len(args) == 0 or args[0] == '-':
            self.input_text = sys.stdin.readlines()
        elif echo:
            self.input_text = [s + '\n' for s in args]
        else:
            f = open(args[0], 'r')
            self.input_text = f.readlines()
            f.close()

        self.count = count
        if count > len(self.input_text) and not repeat:
            self.count = len(self.input_text)

        self.permutation = list(range(len(self.input_text)))
        random.shuffle(self.permutation)

    def shuffle(self):
        if self.repeat:
            for i in range(0, self.count):
                sys.stdout.write(random.choice(self.input_text))
        else:
            for i in range(0, self.count):
                sys.stdout.write(self.input_text[self.permutation[i]])

def main():
    version_msg = '%prog 1.0'
    usage_msg = """%prog [OPTION]... FILE
       or:  %prog -e [OPTION]... [ARG]...
Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-e", "--echo",
                      action="store_true", dest="echo", default=False,
                      help="treat each ARG as an input line")
    parser.add_option("-n", "--head-count",
                      action="store", dest="count", default=sys.maxsize,
                      help="output at most COUNT lines")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines can be repeated")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        echo = bool(options.echo)
    except:
        parser.error("invalid option -- '{0}'".format(options.echo))
    try:
        count = int(options.count)
    except:
        parser.error("invalid line count: '{0}'".format(options.count))
    try:
        repeat = bool(options.repeat)
    except:
        parser.error("invalid option -- '{0}'".format(options.repeat))

    if count < 0:
        parser.error("invalid line count: '{0}'".format(count))

    try:
        generator = shuf(args, echo, count, repeat)
        generator.shuffle()
    except IOError as e:
        errno = e.errno
        strerror = e.strerror
        parser.error('I/O error({0}): {1}'.format(errno, strerror))

if __name__ == '__main__':
    main()
