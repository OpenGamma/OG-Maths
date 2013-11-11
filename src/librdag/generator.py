#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""Generates the implementation of the dispatcher and runners."""

from dispatch import Dispatch
from runners import Runners, Runners, InfixOpRunner, PrefixOpRunner, UnaryFunctionRunner
from exprtree import Terminal
from expression import Expressions, Numeric

# The list of nodes to generate
nodes = [ InfixOpRunner('PLUS', 'PLUS_ENUM', '+'),
          InfixOpRunner('TIMES', 'TIMES_ENUM', '*'),
          PrefixOpRunner('NEGATE', 'NEGATE_ENUM', '-'),
          UnaryFunctionRunner('SIN', 'SIN_ENUM', 'sin') ]

# The list of terminals
terminals = [ Terminal('Real', 'Scalar'),
              Terminal('Complex', 'Scalar'),
              Terminal('Integer', 'Scalar'),
              Terminal('Real', 'Matrix'),
              Terminal('Logical', 'Matrix'),
              Terminal('Complex', 'Matrix'),
              Terminal('Real', 'DiagonalMatrix'),
              Terminal('Complex', 'DiagonalMatrix'),
              Terminal('Real', 'SparseMatrix'),
              Terminal('Complex', 'SparseMatrix') ]

def get_parser():
    """Creates a suitable parser for the options to the generator."""
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-o', '--output', help='Output file name', required=True)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument('--runners-hh', action='store_true', help='Generate runners.hh')
    action.add_argument('--runners-cc', action='store_true', help='Generate runners.cc')
    action.add_argument('--dispatch-hh', action='store_true', help='Generate dispatch.hh')
    action.add_argument('--dispatch-cc', action='store_true', help='Generate dispatch.cc')
    action.add_argument('--expression-hh', action='store_true', help='Generate expression.hh')
    action.add_argument('--expression-cc', action='store_true', help='Generate expression.cc')
    action.add_argument('--numeric-hh', action='store_true', help='Generate numeric.hh')
    action.add_argument('--numeric-cc', action='store_true', help='Generate numeric.cc')
    return parser

def main(args):
    """Entry point if run on the command line"""
    with open(args.output, 'w') as f:
        if args.runners_hh:
            code = Runners(nodes).header
        elif args.runners_cc:
            code = Runners(nodes).source
        elif args.dispatch_hh:
            code = Dispatch(terminals, nodes).header
        elif args.dispatch_cc:
            code = Dispatch(terminals, nodes).source
        elif args.expression_hh:
            code = Expressions(nodes).header
        elif args.expression_cc:
            code = Expressions(nodes).source
        elif args.numeric_hh:
            code = Numeric(nodes).header
        elif args.numeric_cc:
            code = Numeric(nodes).source
        f.writelines(code)

if __name__ == '__main__':
    main(get_parser().parse_args())
