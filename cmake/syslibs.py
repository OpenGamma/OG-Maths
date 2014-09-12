#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""Copies the GCC runtime libraries into the jar libraries build folder.

Subsequently this program fixes the linkage of libraries in the JAR folder to
use an appropriate rpath on Linux and OS X."""

import argparse, os, shutil, subprocess, sys, re
from buildutils import platform_code

libs = {}
libs['lnx'] = [ 'libgcc_s.so.1', 'libgfortran.so.3', 'libquadmath.so.0' ]
libs['osx'] = [ 'libgcc_s.1.dylib', 'libgfortran.3.dylib', 'libquadmath.0.dylib' ]
libs['w64'] = [ 'libgcc_s_seh-1.dll', 'libgfortran-3.dll', 'libquadmath-0.dll' ]
libs['w32'] = [ 'libgcc_s_sjlj-1.dll', 'libgfortran-3.dll', 'libquadmath-0.dll' ]

default_gcc_lib_folder = {}
default_gcc_lib_folder['lnx'] = '/opt/gcc/4.9.1/lib64'
default_gcc_lib_folder['osx'] = '/opt/gcc/4.9.1/lib'
default_gcc_lib_folder['w64'] = 'C:\\buildsystem\\mingw-w64\\x86_64\\4.9.1\\mingw64\\bin'
default_gcc_lib_folder['w32'] = 'C:\\buildsystem\\mingw-w64\\x86\\4.9.1\\mingw32\\bin'

# The pattern for spotting a linux lib is GNU_PRELINKED as found in the DSO dynamic section
pat_gnu_prelinked = re.compile('GNU_PRELINKED');

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
        # Check the system lib to see if it is "prelinked", dynamic section of DSO will
        # contain string "GNU_PRELINKED" if so.
        process = subprocess.Popen(['readelf', '-d', f], cwd=args.output, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        retcode = process.poll()
        if retcode:
            raise RuntimeError('readelf of %s failed. Retcode: %s, stdout: %s, stderr: %s' % (f, retcode, out, err))

        # Check if there's a match in stdout of 'readelf -d' for the "GNU_PRELINKED" line
        if pat_gnu_prelinked.search(out):
            # invoke un-prelink command on library
            print 'Un-prelinking library: %s' % f
            process = subprocess.Popen(['prelink', '-u', f], cwd=args.output, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            out, err = process.communicate()
            retcode = process.poll()
            if retcode:
                raise RuntimeError('prelink -u (un-prelink) of %s failed. Retcode: %s, stdout: %s, stderr: %s' % (f, retcode, out, err))

        # Patch the RUNPATH to $ORIGIN post potential un-prelink else the un-prelink won't work
        print 'Patching RUNPATH to $ORIGIN for: %s' % f
        process = subprocess.Popen(['patchelf', '--set-rpath', '$ORIGIN', f], cwd=args.output)
        out, err = process.communicate()
        retcode = process.poll()
        if retcode:
            raise RuntimeError('patchelf of %s failed. Retcode: %s, output: %s' % (f, retcode, out))

def fix_osx_linkage(args):
    libdir = args.output
    # Get the install name of each library
    install_names = {}
    for name in os.listdir(libdir):
        install_names[name] = subprocess.check_output(['otool','-D',name], cwd=libdir).split()[1]

    # Compute the new install name of each library
    new_install_names = {}
    for name, old_install_name in install_names.iteritems():
        install_name = '@loader_path/%s' % old_install_name.split('/')[-1]
        new_install_names[name] = (old_install_name, install_name)

    # Change the install name of each library
    for name in new_install_names.keys():
        old, new = new_install_names[name]
        print 'Changing install name of %s from %s to %s' % (name, old, new)
        subprocess.check_output(['install_name_tool','-id','%s' % new, name], cwd=libdir)

    # Change the linkage of each library to the new install names
    for lib in install_names.keys():
        linkage = subprocess.check_output(['otool','-L',lib], cwd=libdir).split('\n')
        dylines = [ l.strip().split()[0] for l in linkage if l.find('compatibility') != -1 ]
        dymap = {}
        for l in dylines:
            k = l.split('/')[-1]
            dymap[k] = l
        for name, old in dymap.iteritems():
            try:
                new = new_install_names[name][1]
            except KeyError:
                # Because we're not changing system lib names, not every lib the library links
                # to will be found
                continue
            print 'Changing linkage of %s from %s to %s' % (lib, old, new)
            subprocess.check_output(['install_name_tool','-change','%s' % old, '%s' % new, lib], cwd=libdir)

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
