#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

from createexprtemplates import createexpr_cc, terminal_case, expr_case, \
                                unary_args, binary_args

class CreateExpressions(object):
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def nodes(self):
        return self._nodes

    @property
    def source(self):
        cases = ''
        for node in self.nodes:
            d = { 'typename': node.typename, 'enumname': node.enumname }
            if node.is_terminal:
                cases += terminal_case % d
            else:
                if node.argcount == 1:
                    d['args'] = unary_args
                else: # 2 or -1 (selectresult) - either way its binary
                    d['args'] = binary_args
                cases += expr_case % d
        d = { 'switch_cases': cases }
        return createexpr_cc % d
