#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""This tool creates a version info file for an OG-Maths build on
the current platform. Version info is a yaml file containing
the revision number, build number, version number, project name,
timestamp/local time, and list of artifacts."""

import sys, subprocess, time
from buildutils import platform_code
from yaml import load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

jars = [ None, 'javadoc', 'sources', 'tests' ]

def jarname(version, suffix):
    """Determines the name of an OG-Maths Jar."""
    if suffix is None:
        suffix = ''
    else:
        suffix = '-%s' % suffix
    return 'jars/og-maths-%s-%s%s.jar' % (platform_code(), version, suffix)

def get_subprojects(*args):
    """Returns a list of verinfo data for the OG-Maths subprojects."""
    ret = list()
    for val in args:
      with open(val, 'r') as f:
          verinfo = load(f, Loader=Loader)
      if len(verinfo['platforms']) != 1 or verinfo['platforms'][0] != platform_code():
          raise RuntimeError('OG subproject platform mismatch')
      ret.append(verinfo)
    return ret

def generate_verinfo(project, version, revision, lapack_verinfo_file, izy_verinfo_file, buildnumber):
    """Generates a dict containing the verinfo for this build, incorporating
    the values passed in its parameters."""
    artifacts = [ jarname (version, jar) for jar in jars ]
    subprojects = get_subprojects(lapack_verinfo_file , izy_verinfo_file);
    try:
        buildnumber = int(buildnumber)
    except ValueError:
        pass
    d = { 'project': 'OG-Maths', 'revision': revision, 'version': version, \
          'platforms': [platform_code()], 'artifacts': artifacts,
          'subprojects': subprojects, 'buildnumber': buildnumber,
          'localtime': time.strftime('%Y/%m/%d %H:%M:%S'),
          'timestamp': time.time() }
    return d

def main():
    if len(sys.argv) != 6:
        print "Usage: verinfo.py <output_file> <version> <lapack_verinfo> <izy_verinfo> <build number>"
        return 1

    process = subprocess.Popen(['git', 'rev-parse', 'HEAD'], stdout=subprocess.PIPE)
    rev, unused_err = process.communicate()
    retcode = process.poll()
    if retcode:
        raise RuntimeError('Error getting SHA1 of git HEAD')

    with open(sys.argv[1],'w') as f:
        f.write(dump(generate_verinfo('OG-Maths', sys.argv[2], rev.strip(), sys.argv[3], sys.argv[4], sys.argv[5],), Dumper=Dumper))
    return 0

if __name__ == '__main__':
    sys.exit(main())
