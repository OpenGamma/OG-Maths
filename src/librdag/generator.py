#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""Generates the implementation of the runners."""

from templates import *

class Function(object):
    """A Function is used to generate the code for a single node."""
    def __init__(self, nodename):
        self._nodename = nodename

    @property
    def nodename(self):
        return self._nodename

class UnaryFunction(Function):
    """A UnaryFunction is for a node that takes a single argument."""
    def __init__(self, nodename):
        super(UnaryFunction, self).__init__(nodename)
        self._class_definition_template = unary_runner_class_definition

    @property
    def class_definition(self):
        return self._class_definition_template % { 'nodename': self._nodename }

    @property
    def scalar_runner_function(self):
        implementation = self.scalar_implementation
        d = { 'implementation': implementation,
              'nodename': self.nodename,
              'argtype': 'OGRealScalar',
              'returntype': 'OGRealScalar' }
        return unary_runner_function % d

    @property
    def real_matrix_runner_function(self):
        implementation = self.real_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.nodename,
              'argtype': 'OGRealMatrix',
              'returntype': 'OGRealMatrix' }
        return unary_runner_function % d

    @property
    def complex_matrix_runner_function(self):
        implementation = self.complex_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.nodename,
              'argtype': 'OGComplexMatrix',
              'returntype': 'OGComplexMatrix' }
        return unary_runner_function % d

class BinaryFunction(Function):
    """A BinaryFunction is for a node that takes two arguments."""
    def __init__(self, nodename):
        super(BinaryFunction, self).__init__(nodename)
        self._class_definition_template = binary_runner_class_definition

    @property
    def class_definition(self):
        return self._class_definition_template % { 'nodename': self._nodename }

    @property
    def scalar_runner_function(self):
        implementation = self.scalar_implementation
        d = { 'implementation': implementation,
              'nodename': self.nodename,
              'arg0type': 'OGRealScalar',
              'arg1type': 'OGRealScalar',
              'returntype': 'OGRealScalar' }
        return binary_runner_function % d

    @property
    def real_matrix_runner_function(self):
        implementation = self.real_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.nodename,
              'arg0type': 'OGRealMatrix',
              'arg1type': 'OGRealMatrix',
              'returntype': 'OGRealMatrix' }
        return binary_runner_function % d

    @property
    def complex_matrix_runner_function(self):
        implementation = self.complex_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.nodename,
              'arg0type': 'OGComplexMatrix',
              'arg1type': 'OGComplexMatrix',
              'returntype': 'OGComplexMatrix' }
        return binary_runner_function % d

class InfixOp(BinaryFunction):
    """An InfixOp is a BinaryFunction that has a particular symbol that is
    placed infix in its two arguments in the generated code."""
    def __init__(self, nodename, symbol):
        super(InfixOp, self).__init__(nodename)
        self._symbol = symbol

    @property
    def symbol(self):
        return self._symbol

    @property
    def scalar_implementation(self):
        d = { 'symbol':     self.symbol,
              'arg0type':   'OGRealScalar',
              'arg1type':   'OGRealScalar',
              'returntype': 'OGRealScalar' }
        return infix_scalar_runner_implementation % d

    @property
    def real_matrix_implementation(self):
        return "  arg0->debug_print(); arg1->debug_print(); ret = arg0; // TBC"

    @property
    def complex_matrix_implementation(self):
        return "  arg0->debug_print(); arg1->debug_print(); ret = arg0; // TBC"

class PrefixOp(UnaryFunction):
    """A PrefixOp is a UnaryFunction whose symbol is placed just before its
    argument in the code."""
    def __init__(self, nodename, symbol):
        super(PrefixOp, self).__init__(nodename)
        self._symbol = symbol

    @property
    def symbol(self):
        return self._symbol

    @property
    def scalar_implementation(self):
        d = { 'symbol':     self.symbol,
              'returntype': 'OGRealScalar' }
        return prefix_scalar_runner_implementation % d

    @property
    def real_matrix_implementation(self):
        d = { 'symbol':     self.symbol,
              'datatype':   'real16',
              'returntype': 'OGRealMatrix' }
        return prefix_matrix_runner_implementation %d

    @property
    def complex_matrix_implementation(self):
        d = { 'symbol':     self.symbol,
              'datatype':   'complex16',
              'returntype': 'OGComplexMatrix' }
        return prefix_matrix_runner_implementation %d

class RunnersHeader(object):
    """Generates the runners.hh file for a set of nodes."""
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def implementation(self):
        class_definitions = ""
        for node in nodes:
            class_definitions += node.class_definition
        d = { 'class_definitions': class_definitions }
        return runners_header % d

class Runners(object):
    """Generates the runners.cc file for a set of nodes."""
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def implementation(self):
        function_definitions = ""
        for node in nodes:
            function_definitions += node.scalar_runner_function
            function_definitions += node.real_matrix_runner_function
            function_definitions += node.complex_matrix_runner_function
        d = { 'function_definitions': function_definitions }
        return runners_cc % d

# The list of nodes to generate
nodes = [ InfixOp('Plus', '+'), PrefixOp('Negate', '-') ]

def get_parser():
    """Creates a suitable parser for the options to the generator."""
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-o', '--output', help='Output file name', required=True)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument('--runners-hh', action='store_true', help='Generate runners.hh')
    action.add_argument('--runners-cc', action='store_true', help='Generate runners.cc')
    return parser

def main(args):
    """Entry point if run on the command line"""
    with open(args.output, 'w') as f:
        if args.runners_hh:
            code = RunnersHeader(nodes).implementation
        elif args.runners_cc:
            code = Runners(nodes).implementation
        f.writelines(code)

if __name__ == '__main__':
    main(get_parser().parse_args())
