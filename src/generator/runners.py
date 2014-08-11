from runnertemplates import runners_header, runners_cc, binary_runner_class_definition, \
                            binary_runner_function, infix_scalar_runner_implementation, \
                            infix_matrix_runner_implementation,                         \
                            unary_runner_class_definition, unary_runner_function,       \
                            prefix_scalar_runner_implementation, \
                            prefix_matrix_runner_implementation, \
                            unaryfunction_scalar_runner_implementation, \
                            unaryfunction_matrix_runner_implementation, \
                            unimplementedunary_runner_function, \
                            unimplementedbinary_runner_function, \
                            integer_parameter_runner_class_definition
from exprtree import UnaryExpression, BinaryExpression
from jinja2 import Environment, DictLoader

class UnaryExpressionRunner(UnaryExpression):
    """For generating the runner code for a UnaryExpression."""
    def __init__(self, nodename, enumname):
        super(UnaryExpressionRunner, self).__init__(nodename, enumname)
        self._class_definition_template = unary_runner_class_definition

    @property
    def class_definition(self):
        return self._class_definition_template % { 'nodename': self.typename }

    @property
    def scalar_runner_function(self):
        implementation = self.scalar_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'argtype': 'OGRealScalar',
              'returntype': 'OGRealScalar' }
        return unary_runner_function % d

    @property
    def real_matrix_runner_function(self):
        implementation = self.real_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'argtype': 'OGRealDenseMatrix',
              'returntype': 'OGRealDenseMatrix' }
        return unary_runner_function % d

    @property
    def complex_matrix_runner_function(self):
        implementation = self.complex_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'argtype': 'OGComplexDenseMatrix',
              'returntype': 'OGComplexDenseMatrix' }
        return unary_runner_function % d

class BinaryExpressionRunner(BinaryExpression):
    """A BinaryFunction is for a node that takes two arguments."""
    def __init__(self, nodename, enumname):
        super(BinaryExpressionRunner, self).__init__(nodename, enumname)
        self._class_definition_template = binary_runner_class_definition

    @property
    def class_definition(self):
        return self._class_definition_template % { 'nodename': self.typename }

    @property
    def scalar_runner_function(self):
        implementation = self.scalar_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'arg0type': 'OGRealScalar',
              'arg1type': 'OGRealScalar',
              'returntype': 'OGRealScalar' }
        return binary_runner_function % d

    @property
    def real_matrix_runner_function(self):
        implementation = self.real_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'arg0type': 'OGRealDenseMatrix',
              'arg1type': 'OGRealDenseMatrix',
              'returntype': 'OGRealDenseMatrix' }
        return binary_runner_function % d

    @property
    def complex_matrix_runner_function(self):
        implementation = self.complex_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'arg0type': 'OGComplexDenseMatrix',
              'arg1type': 'OGComplexDenseMatrix',
              'returntype': 'OGComplexDenseMatrix' }
        return binary_runner_function % d

    @property
    def argcount(self):
        return 2

class SelectResultRunner(BinaryExpression):
    """This is a little bit special because its second
    argument is always an integer scalar. Used for SELECTRESULT, and may be
    useful for generalising into something for SELECT/ASSIGN nodes in future."""
    def __init__(self, nodename, enumname):
        super(SelectResultRunner, self).__init__(nodename, enumname)
        self._class_definition_template = integer_parameter_runner_class_definition

    @property
    def class_definition(self):
        return self._class_definition_template % { 'nodename': self.typename }

    @property
    def scalar_runner_function(self):
        implementation = self.scalar_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'arg0type': 'OGRealScalar',
              'arg1type': 'OGIntegerScalar',
              'returntype': 'OGTerminal' }
        return binary_runner_function % d

    @property
    def real_matrix_runner_function(self):
        implementation = self.real_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'arg0type': 'OGRealDenseMatrix',
              'arg1type': 'OGIntegerScalar',
              'returntype': 'OGTerminal' }
        return binary_runner_function % d

    @property
    def complex_matrix_runner_function(self):
        implementation = self.complex_matrix_implementation
        d = { 'implementation': implementation,
              'nodename': self.typename,
              'arg0type': 'OGComplexDenseMatrix',
              'arg1type': 'OGIntegerScalar',
              'returntype': 'OGTerminal' }
        return binary_runner_function % d

    @property
    def argcount(self):
        """The returned value (-1) is a special flag used to indicate the
        SelectResultRunner to the dispatch code generator."""
        # This is not the best design, and should be rethought once we have more
        # custom nodes so we can do something more generic.
        return -1

class InfixOpRunner(BinaryExpressionRunner):
    """An InfixOp is a BinaryExpression that has a particular symbol that is
    placed infix in its two arguments in the generated code."""
    def __init__(self, nodename, enumname, symbol, izysymbol_vv, izysymbol_vs, izysymbol_sv):
        super(InfixOpRunner, self).__init__(nodename, enumname)
        self._symbol = symbol
        self._izysymbol_vv = izysymbol_vv
        self._izysymbol_vs = izysymbol_vs
        self._izysymbol_sv = izysymbol_sv
        self._env = Environment(loader=DictLoader({'infix_matrix_runner_implementation': infix_matrix_runner_implementation}))

    @property
    def env(self):
        return self._env

    @property
    def symbol(self):
        return self._symbol

    @property
    def izysymbol_vv(self):
        return self._izysymbol_vv

    @property
    def izysymbol_vs(self):
        return self._izysymbol_vs

    @property
    def izysymbol_sv(self):
        return self._izysymbol_sv

    @property
    def scalar_implementation(self):
        d = { 'symbol':     self.symbol,
              'returntype': 'OGRealScalar' }
        return infix_scalar_runner_implementation % d

    @property
    def real_matrix_implementation(self):
        d = { 'symbol':     self.symbol,
              'izysymbol_vv':     self.izysymbol_vv,
              'izysymbol_vs':     self.izysymbol_vs,
              'izysymbol_sv':     self.izysymbol_sv,
              'datatype':   'real8',
              'returntype': 'OGRealDenseMatrix' }
        template = self.env.get_template('infix_matrix_runner_implementation');
        return template.render(d);

    @property
    def complex_matrix_implementation(self):
        d = { 'symbol':     self.symbol,
              'izysymbol_vv':     self.izysymbol_vv,
              'izysymbol_vs':     self.izysymbol_vs,
              'izysymbol_sv':     self.izysymbol_sv,
              'datatype':   'complex16',
              'returntype': 'OGComplexDenseMatrix' }
        template = self.env.get_template('infix_matrix_runner_implementation');
        return template.render(d);


class PrefixOpRunner(UnaryExpressionRunner):
    """A PrefixOp is a UnaryFunction whose symbol is placed just before its
    argument in the code."""
    def __init__(self, nodename, enumname, symbol):
        super(PrefixOpRunner, self).__init__(nodename, enumname)
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
              'datatype':   'real8',
              'returntype': 'OGRealDenseMatrix' }
        return prefix_matrix_runner_implementation % d

    @property
    def complex_matrix_implementation(self):
        d = { 'symbol':     self.symbol,
              'datatype':   'complex16',
              'returntype': 'OGComplexDenseMatrix' }
        return prefix_matrix_runner_implementation % d

class UnaryFunctionRunner(UnaryExpressionRunner):
    """A UnaryFunction is one that is implemented with a call to a function
    that takes a single argument (e.g. cos, sin, etc)."""

    def __init__(self, nodename, enumname, function):
        super(UnaryFunctionRunner, self).__init__(nodename, enumname)
        self._function = function

    @property
    def function(self):
        return self._function

    @property
    def scalar_implementation(self):
        d = { 'function': self.function,
              'returntype': 'OGRealScalar' }
        return unaryfunction_scalar_runner_implementation % d

    @property
    def real_matrix_implementation(self):
        d = { 'function':    self.function,
              'datatype':   'real8',
              'returntype': 'OGRealDenseMatrix' }
        return unaryfunction_matrix_runner_implementation % d

    @property
    def complex_matrix_implementation(self):
        d = { 'function':   self.function,
              'datatype':   'complex16',
              'returntype': 'OGComplexDenseMatrix' }
        return unaryfunction_matrix_runner_implementation % d

class UnimplementedUnary(UnaryExpressionRunner):
    """A unary expression node that we want to generate code for, but haven't yet
    implemented."""
    def __init__(self, nodename):
        enumname = '%s_ENUM' % nodename
        super(UnimplementedUnary, self).__init__(nodename, enumname)

    @property
    def scalar_runner_function(self):
        d = { 'nodename': self.typename,
              'argtype': 'OGRealScalar',
              'returntype': 'OGRealScalar' }
        return unimplementedunary_runner_function % d

    @property
    def real_matrix_runner_function(self):
        d = { 'nodename': self.typename,
              'argtype': 'OGRealDenseMatrix',
              'returntype': 'OGRealDenseMatrix' }
        return unimplementedunary_runner_function % d

    @property
    def complex_matrix_runner_function(self):
        d = { 'nodename': self.typename,
              'argtype': 'OGComplexDenseMatrix',
              'returntype': 'OGComplexDenseMatrix' }
        return unimplementedunary_runner_function % d

class UnimplementedBinary(BinaryExpressionRunner):
    """A binary expression node that we want to generate code for, but haven't yet
    implemented."""
    def __init__(self, nodename):
        enumname = '%s_ENUM' % nodename
        super(UnimplementedBinary, self).__init__(nodename, enumname)

    @property
    def scalar_runner_function(self):
        d = { 'nodename': self.typename,
              'arg0type': 'OGRealScalar',
              'arg1type': 'OGRealScalar',
              'returntype': 'OGRealScalar' }
        return unimplementedbinary_runner_function % d

    @property
    def real_matrix_runner_function(self):
        d = { 'nodename': self.typename,
              'arg0type': 'OGRealDenseMatrix',
              'arg1type': 'OGRealDenseMatrix',
              'returntype': 'OGRealDenseMatrix' }
        return unimplementedbinary_runner_function % d

    @property
    def complex_matrix_runner_function(self):
        d = { 'nodename': self.typename,
              'arg0type': 'OGComplexDenseMatrix',
              'arg1type': 'OGComplexDenseMatrix',
              'returntype': 'OGComplexDenseMatrix' }
        return unimplementedbinary_runner_function % d

class Runners(object):
    """Generates the runners.hh and .cc files for a set of nodes."""
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def header(self):
        class_definitions = ""
        for node in self._nodes:
            class_definitions += node.class_definition
        d = { 'class_definitions': class_definitions }
        return runners_header % d

    @property
    def source(self):
        function_definitions = ""
        for node in self._nodes:
            function_definitions += node.scalar_runner_function
            function_definitions += node.real_matrix_runner_function
            function_definitions += node.complex_matrix_runner_function
        d = { 'function_definitions': function_definitions }
        return runners_cc % d
