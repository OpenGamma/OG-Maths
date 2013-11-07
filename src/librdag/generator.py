#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""Generates the implementation of the dispatcher and runners."""

from dispatch import Dispatch
from runners import Runners, Runners, InfixOp, PrefixOp
from terminals import Terminal
from expression import Expressions

# The list of nodes to generate
nodes = [ InfixOp('PLUS', '+', 'PLUS_ENUM'), PrefixOp('NEGATE', '-', 'NEGATE_ENUM') ]

# The list of terminals
terminals = [ Terminal('OGRealScalar', 'REAL_SCALAR_ENUM'),
              Terminal('OGComplexScalar', 'COMPLEX_SCALAR_ENUM'),
              Terminal('OGIntegerScalar', 'INTEGER_SCALAR_ENUM'),
              Terminal('OGRealMatrix', 'REAL_MATRIX_ENUM'),
              Terminal('OGLogicalMatrix', 'LOGICAL_MATRIX_ENUM'),
              Terminal('OGComplexMatrix', 'COMPLEX_MATRIX_ENUM'),
              Terminal('OGRealDiagonalMatrix', 'REAL_DIAGONAL_MATRIX_ENUM'),
              Terminal('OGComplexDiagonalMatrix', 'COMPLEX_DIAGONAL_MATRIX_ENUM'),
              Terminal('OGRealSparseMatrix', 'REAL_SPARSE_MATRIX_ENUM'),
              Terminal('OGComplexSparseMatrix', 'COMPLEX_SPARSE_MATRIX_ENUM') ]

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
        f.writelines(code)

if __name__ == '__main__':
    main(get_parser().parse_args())
