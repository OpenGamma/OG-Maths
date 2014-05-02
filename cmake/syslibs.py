#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""Copies the GCC runtime libraries into the jar libraries build folder.

Subsequently this program fixes the linkage of libraries in the JAR folder to
use an appropriate rpath on Linux and OS X."""

import argparse, os, shutil, subprocess, sys
from buildutils import platform_code

libs = {}
libs['lnx'] = [ 'libstdc++.so.6', 'libgcc_s.so.1', 'libgfortran.so.3', 'libquadmath.so.0' ]
libs['osx'] = [ 'libstdc++.6.dylib', 'libgcc_s.1.dylib', 'libgfortran.3.dylib', 'libquadmath.0.dylib' ]
libs['win'] = [ 'libstdc++-6.dll', 'libgcc_s_seh-1.dll', 'libgfortran-3.dll', 'libquadmath-0.dll' ]

default_gcc_lib_folder = {}
default_gcc_lib_folder['lnx'] = '/opt/gcc/4.8.2/lib64'
default_gcc_lib_folder['osx'] = '/opt/local/lib/gcc48'
default_gcc_lib_folder['win'] = 'C:\\BuildSystem\\mingw-builds\\x64-4.8.1-win32-seh-rev2\\mingw64\\bin'

def get_parser():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-l', '--lib-location', help='Location of GCC runtime libraries')
    parser.add_argument('-o', '--output', help='Output folder')
    return parser

def get_lib_folder(args):
    p = platform_code()
    return args.lib_location or os.environ.get('GCC_LIB_FOLDER') or default_gcc_lib_folder[p]

def fix_linux_linkage(args):
    files = os.listdir(args.output)
    for f in files:
        print 'Patching RUNPATH to $ORIGIN for %s' % f
        process = subprocess.Popen(['patchelf', '--set-rpath', '$ORIGIN', f], cwd=args.output)
        out, err = process.communicate()
        retcode = process.poll()
        if retcode:
            raise RuntimeError('patchelf of %s failed. Retcode: %s, output: %s' % (f, retcode, out))

def fix_osx_linkage(args):
    raise NotImplementedError('OS X linkage fixing is not yet implemented.')

def main(args):
    gcc_lib_folder = get_lib_folder(args)
    print "GCC libraries are at %s" % gcc_lib_folder
    p = platform_code()
    for lib in libs[p]:
        src = os.path.join(gcc_lib_folder, lib)
        dest = os.path.join(args.output, lib)
        print "Copying %s to %s" % (src, dest)
        shutil.copyfile(src, dest)
    if p == 'lnx':
        fix_linux_linkage(args)
    elif p == 'osx':
        fix_osx_linkage(args)

if __name__ == '__main__':
    sys.exit(main(get_parser().parse_args()))
