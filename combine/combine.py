#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""Takes platform-specific blobs and creates a platform-independent
blob that consists of a platform independent main jar and the tests,
sources and javadoc jars. 

Verinfo from each blob is combined to produce a single verinfo file
with information about the builds that the combined main jar came
from."""

import random, string, sys, os, zipfile
from pprint import pprint
from yaml import load, dump
from StringIO import StringIO
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

# Whether to output information about what's going on. This is left
# True, since we hypothesise it will be useful information to keep
# in the logs.
DEBUG = True

def main():
    if len(sys.argv) < 3:
        print "Usage: combine.py <build number> [source blobs]"
        exit(1)
    
    buildnumber = sys.argv[1]
    sourceblobs = sys.argv[2:]

    verinfos = read_version_info(sourceblobs)
    validate_version_info(verinfos)
    newverinfo = construct_combined_verinfo(verinfos, buildnumber)
    mainjar = create_main_jar(newverinfo, verinfos)
    blobname = create_combined_blob(newverinfo, verinfos, mainjar)
    write_verinfo(newverinfo)
    print 'Blob name: %s' % blobname
    return 0

def read_version_info(sourceblobs):
    """Read the version info from each of the specified source blobs."""
    verinfos = {}
    for blob in sourceblobs:
        zipf = zipfile.ZipFile(blob, 'r')
        verinfo = zipf.open('verinfo.yaml')
        verinfos[blob] = load(verinfo, Loader=Loader)
        if DEBUG:
            print "\nRead verinfo.yaml from %s:\n" % blob
            pprint(verinfos[blob])
        verinfo.close()
        zipf.close()
    return verinfos

def validate_version_info(verinfos):
    """Validate version info - the project, revision, and version should match across
    blobs and across each subproject in the blobs."""

    def important(attr):
        """Returns true if it is important for an attribute to be equal across blobs."""
        return attr in ['project', 'revision', 'version']
    
    # Use the first blob's verinfo as a reference
    refblob = verinfos.keys()[0]
    ref = verinfos[refblob]
    for blob, verinfo in verinfos.iteritems():
        for k, v in ref.iteritems():
            if important(k) and v != verinfo[k]:
                raise ValueError('Reference blob %s key %s = %s does not match blob %s which has value %s' %
                                 (refblob, k, v, blob, verinfo[k]))
            if k == 'subprojects':
                # Check subprojects.
                # Check the subprojects are the same subprojects in each project
                refsps = set([ s['project'] for s in ref['subprojects'] ])
                versps = set([ s['project'] for s in verinfo['subprojects'] ])
                if refsps != versps:
                    raise ValueError('Reference blob %s has subprojects %s but blob %s has subprojects %s' %
                                     (refblob, refsps, blob, versps))
                # Check subproject keys match
                for refsubproject in ref['subprojects']:
                    # Find subproject from verinfo
                    for sp in verinfo['subprojects']:
                        if sp['project'] == refsubproject['project']:
                            subproject = sp
                    for ks, vs in refsubproject.iteritems():
                        if important(ks) and vs != subproject[ks]:
                            raise ValueError('Reference blob %s key %s.%s = %s does not match blob %s which has value %s' %
                                             (refblob, k, ks, v, blob, subproject[ks]))
    
def construct_combined_verinfo(verinfos, buildnumber):
    """Constructs verinfo for the combined blob based on all constituent verinfos.
    It is expected that the verinfos are already validated."""

    # Use the first verinfo as reference
    ref = verinfos[verinfos.keys()[0]]

    # Top-level artifact info
    newverinfo = {}
    newverinfo['project'] = ref['project']
    newverinfo['revision'] = ref['revision']
    newverinfo['version'] = ref['version']
    
    # Add subproject revisions
    newverinfo['subprojects'] = {}
    for s in ref['subprojects']:
        newverinfo['subprojects'][s['project']] = s['revision']

    # Add the timestamps, localtimes, and buildnumbers keys - these consist of all of the
    # values from the constituent verinfos
    def construct_combined_key(srckey):
        ckey = '%ss' % srckey
        # Create a dict for the combined keys to be stored
        subprojects = {}
        for s in ref['subprojects']:
            subprojects[s['project']] = {}
        newverinfo[ckey] = { 'subprojects': subprojects }
        # Populate the combined key's dict
        for blob, verinfo in verinfos.iteritems():
            platform = verinfo['platforms'][0]
            newverinfo[ckey][platform] = verinfo[srckey]
            for subproject in verinfo['subprojects']:
                newverinfo[ckey]['subprojects'][subproject['project']][platform] = subproject[srckey]

    construct_combined_key('buildnumber')
    construct_combined_key('timestamp')
    construct_combined_key('localtime')
    
    # Add the build number of the combined blob
    newverinfo['buildnumber'] = int(buildnumber)

    # Add the artifact names.
    basename = '%s-%s' % (newverinfo['project'].lower(), newverinfo['version'])
    artifacts = [ s % basename for s in ('%s.jar', '%s-javadoc.jar', '%s-sources.jar', '%s-tests.jar') ]
    newverinfo['artifacts'] = artifacts

    if DEBUG:
        print "\nNew verinfo:\n"
        pprint(newverinfo)

    return newverinfo

def create_combined_blob(newverinfo, verinfos, mainjar):
    """Creates a new blob and puts everything in it. The main jar's data is
    passed in mainjar, since its creation requires a lot of other steps to
    be taken, and it is kept separate from the blob creation."""
    blobname = '%s-%s-%s.zip' % (newverinfo['project'], newverinfo['version'], newverinfo['buildnumber'])
    blob = zipfile.ZipFile(blobname, 'w')
    # Open up the Linux blob to read the static artifacts from it
    k = [s for s in verinfos.keys() if 'lnx' in s][0]
    srcblob = zipfile.ZipFile(k, 'r')
    srcartifacts = verinfos[k]['artifacts']
    destartifacts = newverinfo['artifacts']
    for artifact in srcartifacts:
        data = srcblob.read(artifact)
        if 'javadoc' in artifact:
            destname = [s for s in destartifacts if 'javadoc' in s][0]
        elif 'sources' in artifact:
            destname = [s for s in destartifacts if 'sources' in s][0]
        elif 'tests' in artifact:
            destname = [s for s in destartifacts if 'tests' in s][0]
        else:
            # Don't write this one because it's the main jar
            continue
        if DEBUG:
            print "Adding %s to blob" % destname
        blob.writestr(destname, data)
    # Add the main jar to the blob
    mainjarname = [s for s in destartifacts if not ('javadoc' in s or 'sources' in s or 'tests' in s)][0]
    if DEBUG:
        print "Adding %s to blob" % mainjarname
    blob.writestr(mainjarname, mainjar)
    # Add verinfo to the blob
    if DEBUG:
        print "Adding verinfo.yaml to blob"
    blob.writestr('verinfo.yaml', dump(newverinfo, Dumper=Dumper))
    blob.close()
    return blobname

def create_main_jar(newverinfo, verinfos):
    """Combines the three platform jars to create a single main jar"""
    
    class MagicStringIO(StringIO):
        """A StringIO object whose buffer persists after having close()
        called - this is required so the ZipFile wrapping it can write
        out the central directory. In doing so the ZipFile calls close()
        on the StringIO object it is holding, freeing the data before we
        get a chance to get at it.

        After the ZipFile has been closed, call free() to actually free
        the memory buffer (as you normally would to close a file-like
        object)."""

        def close(self):
            """Does not free the memory buffer (c.f. StringIO.close()
            which does."""
            pass

        def free(self):
            """Frees the memory buffer."""
            if not self.closed:
                self.closed = True
                del self.buf, self.pos

    # jar is the new zip file we're building that will be the new main jar
    jardata = MagicStringIO()
    jar = zipfile.ZipFile(jardata, 'w', compression=zipfile.ZIP_DEFLATED)
    
    # Open the linux main jar
    k = [s for s in verinfos.keys() if 'lnx' in s][0]
    srcblob = zipfile.ZipFile(k, 'r')
    artifacts = verinfos[k]['artifacts']
    mainjarname = [s for s in artifacts if not ('javadoc' in s or 'sources' in s or 'tests' in s)][0]
    zfile = StringIO(srcblob.read(mainjarname))
    srcblob.close()
    mainjar = zipfile.ZipFile(zfile, 'r')

    # Copy the platform-independent files into the main jar from the linux main jar
    nl = mainjar.namelist()
    files = [ n for n in nl if (n[:3] == 'com' or n[:6] == 'config' or n[:8] == 'META-INF') and n[-1] != '/' ]
    for f in files:
        if DEBUG:
            print "Adding %s to main jar" % f
        data = mainjar.read(f)
        jar.writestr(f, data)

    # Copy the platform-specific libs into the main jar
    for p in ('lnx', 'osx', 'win'):
        copy_platform_libs_from_jar(p, verinfos, jar)
    
    # add verinfo to the new jar
    if DEBUG:
        print "Adding verinfo.yaml to main jar"
    jar.writestr('verinfo.yaml', dump(newverinfo, Dumper=Dumper))
    
    # close the new jar
    jar.close()
    
    # add the newly-written jar to the blob
    jarstr = jardata.getvalue()
    jardata.free()
    return jarstr

def copy_platform_libs_from_jar(platform, verinfos, destjar):
    """Copies the contents of the lib folder from the source jar to
    the dest jar. The verinfos are required to find the name of the
    source blob for the given platform's source jar."""
    k = [s for s in verinfos.keys() if platform in s][0]
    srcblob = zipfile.ZipFile(k, 'r')
    artifacts = verinfos[k]['artifacts']
    srcjarname = [s for s in artifacts if not ('javadoc' in s or 'sources' in s or 'tests' in s)][0]
    zfile = StringIO(srcblob.read(srcjarname))
    srcblob.close()
    srcjar = zipfile.ZipFile(zfile, 'r')
    nl = srcjar.namelist()
    files = [ n for n in nl if n[:3] == 'lib' and n[-1] != '/' ]
    for f in files:
        if DEBUG:
            print "Adding %s to main jar" % f
        data = srcjar.read(f)
        destjar.writestr(f, data)

def write_verinfo(verinfo):
    """Writes the verinfo.yaml file to the current dir."""
    if DEBUG:
        print "Writing verinfo.yaml to current dir"""
    with open('verinfo.yaml', 'w') as f:
        f.write(dump(verinfo, Dumper=Dumper))

if __name__ == '__main__':
    sys.exit(main())
