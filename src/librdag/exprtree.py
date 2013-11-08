#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

class Terminal(object):
    def __init__(self, name, enumname):
        self._name = name
        self._enumname = enumname

    @property
    def name(self):
        return self._name

    @property
    def enumname(self):
        return self._enumname

    def __eq__(self, other):
        return self.name == other.name
