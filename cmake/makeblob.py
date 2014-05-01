#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

'''This tool creates a "blob" file. A blob consists of a set of artifacts
and a verinfo file describing them, wrapped up in a zip file. Blobs are
used so that artifacts can be uploaded with information about their build
in a single file, either to the buildmaster, or another machine being
used to host built code.'''

import sys, os, zipfile
from yaml import load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

def main():
    if len(sys.argv) != 3:
        print "Usage: makeblob.py <version info file> <build number>"
        exit(1)
    
    vfilename = sys.argv[1]
    buildnum = sys.argv[2]
    dirname = os.path.dirname(os.path.realpath(vfilename))
    
    # Read version info file
    with open(vfilename, 'r') as f:
        verinfo = load(f, Loader=Loader)

    # Determine blob name
    multiplatform =  len(verinfo['platforms']) > 1
    if multiplatform:
        blobname = '%s-%s-%s' % (verinfo['project'], verinfo['version'], buildnum)
    else:
        blobname = '%s-%s-%s-%s' % (verinfo['project'], verinfo['platforms'][0], verinfo['version'], buildnum)

    # Create zip file
    zipfname = '%s.zip' % blobname
    zipfpath = os.path.join(dirname, zipfname)
    zipf = zipfile.ZipFile(zipfpath, 'w')
    
    # Add version info and artifacts to zip file
    zipf.write(vfilename, os.path.basename(vfilename))
    for f in verinfo['artifacts']:
        zipf.write(os.path.join(dirname, f), f)
    zipf.close()
    
    # Success!
    print "Blob name: %s" % blobname
    return 0
    
if __name__ == '__main__':
    sys.exit(main())
