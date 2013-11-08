#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

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
    def __init__(self, typename, enumname):
        super(Terminal, self).__init__(typename, enumname)
