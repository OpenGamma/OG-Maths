#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

import re
from util import camel2underscore

class Numeric(object):
    def __init__(self, typename, enumname):
        self._typename = typename
        self._enumname = enumname

    @property
    def typename(self):
        return self._typename

    @property
    def enumname(self):
        return self._enumname

    def __eq__(self, other):
        return self.typename == other.typename

class Terminal(Numeric):
    def __init__(self, datatype, storagetype):
        typename = "OG%s%s" % (datatype, storagetype)
        enumname = '%s_%s_ENUM' % (datatype.upper(), camel2underscore(storagetype).upper())
        super(Terminal, self).__init__(typename, enumname)
        self._datatype = datatype
        self._storagetype = storagetype

    @property
    def datatype(self):
        return self._datatype

    @property
    def storagetype(self):
        return self._storagetype
