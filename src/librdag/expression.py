#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

from exprtemplates import expression_hh, expr_class, expression_cc, expr_methods
from runners import InfixOp, PrefixOp

class Expressions(object):
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def nodes(self):
        return self._nodes

    def parentclass(self, node):
        if node.argcount == 1:
            return 'OGUnaryExpr'
        else:
            return 'OGBinaryExpr'

    @property
    def header(self):
        classes = ''
        for node in self.nodes:
            d = { 'classname': node.nodename, 'parentclass': self.parentclass(node) }
            classes += expr_class % d
        d = { 'expression_classes': classes }
        return expression_hh % d

    @property
    def source(self):
        methods = ''
        for node in self.nodes:
            d = { 'classname': node.nodename, 'parentclass': self.parentclass(node) }
            methods += expr_methods % d
        d = { 'expression_methods': methods }
        return expression_cc % d
