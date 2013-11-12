#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

import re
from util import camel2underscore

class Numeric(object):
    """Numeric is the base class for all expression tree nodes.

    All expression tree nodes have a typename and and enum name."""
    def __init__(self, typename, enumname):
        self._typename = typename
        self._enumname = enumname

    @property
    def typename(self):
        return self._typename

    @property
    def enumname(self):
        return self._enumname

    @property
    def is_terminal(self):
        return False

    @property
    def is_expr(self):
        return False

    def __eq__(self, other):
        """Two Numerics are equal if they have the same type."""
        return self.typename == other.typename

class Terminal(Numeric):
    """A terminal is a numeric. It also has a datatype and a storage type."""
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

    @property
    def is_terminal(self):
        return True

class Expression(Numeric):
    """A Function is used to generate the code for a single node."""

    @property
    def argcount(self):
        raise NotImplementedError("argcount must be implemented by Expression subclasses.")

    @property
    def parentclass(self):
        raise NotImplementedError("parentclass must be implemented by Expression subclasses.")

    @property
    def is_expr(self):
        return True

class UnaryExpression(Expression):
    """A UnaryExpression is an Expression that takes one argument."""

    @property
    def argcount(self):
        return 1

    @property
    def parentclass(self):
        return 'OGUnaryExpr'

class BinaryExpression(Expression):
    """A BinaryExpression is an expression that takes two arguments."""

    @property
    def argcount(self):
        return 2

    @property
    def parentclass(self):
        return 'OGBinaryExpr'
