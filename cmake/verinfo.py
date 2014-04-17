#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

import sys, subprocess

def main():
    if len(sys.argv) != 2:
        print "Usage: verinfo.py <output_file>"
        return 1

    process = subprocess.Popen(['git', 'rev-parse', 'HEAD'], stdout=subprocess.PIPE)
    rev, unused_err = process.communicate()
    retcode = process.poll()
    if retcode:
        raise RuntimeError('Error getting SHA1 of git HEAD')
    with open(sys.argv[1],'w') as f:
        f.write(rev)
    return 0

if __name__ == '__main__':
    sys.exit(main())
