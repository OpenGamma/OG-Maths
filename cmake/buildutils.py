#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

import platform, subprocess

def get_bits():
    process = subprocess.Popen(['gcc', '-dumpmachine'], stdout=subprocess.PIPE)
    machine, unused_err = process.communicate()
    retcode = process.poll()
    if retcode:
        raise RuntimeError('Error using gcc -dumpmachine')
    if machine[0:4] == 'i686':
        return 32
    else:
        return 64

def platform_code():
    """Returns the current platform code, following the usual convention
    for OG-Maths."""
    p = platform.system()
    if p == 'Linux':
        return 'lnx'
    elif p == 'Darwin':
        return 'osx'
    elif p =='Windows':
        if get_bits() == 32:
            return 'w32'
        else:
            return 'w64'
    else:
        raise NotImplementedError('This platform (%s) is not recognised.' % p)
