#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

from pyprimes import primes_above

class ExprEnums(object):
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def nodes(self):
        return self._nodes

    @property
    def code(self):
        enums = []
        # Start numbers at 0x175 to give some headroom
        g = primes_above(0x174)
        for node in self.nodes:
            code = "%s = 0X%04XL" % (node.enumname, next(g))
            enums.append(code)
        return ',\n'.join(enums);
