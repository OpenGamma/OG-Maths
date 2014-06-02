#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

from exprtemplates import expression_hh, expr_class, expression_cc, expr_methods, \
                          numeric_hh, numeric_fwd_decl, numeric_cast_method, \
                          numeric_cc, numeric_method, unary_constructor, \
                          binary_constructor, unary_copy_method, binary_copy_method, \
                          unary_ctor_method, binary_ctor_method, unary_factory, \
                          binary_factory, unary_factory_method, binary_factory_method

class Expressions(object):
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def nodes(self):
        return self._nodes

    @property
    def header(self):
        classes = ''
        for node in self.nodes:
            d = { 'classname': node.typename, 'parentclass': node.parentclass }
            if node.argcount == 1:
                constructor = unary_constructor % d
                factory = unary_factory % d
            else: # 2 or -1 (selectresult) - is binary in either case
                constructor = binary_constructor % d
                factory = binary_factory % d
            d['constructor'] = constructor
            d['factory'] = factory
            classes += expr_class % d
        d = { 'expression_classes': classes }
        return expression_hh % d

    @property
    def source(self):
        methods = ''
        for node in self.nodes:
            d = { 'classname': node.typename, 'parentclass': node.parentclass }
            if node.argcount == 1:
                copy_method = unary_copy_method % d
                ctor_method = unary_ctor_method % d
                factory_method = unary_factory_method % d
            else: # 2 or -1 (selectresult) - is binary in either case
                copy_method = binary_copy_method % d
                ctor_method = binary_ctor_method % d
                factory_method = binary_factory_method % d
            d['copy_method'] = copy_method
            d['ctor_method'] = ctor_method
            d['factory_method'] = factory_method
            methods += expr_methods % d
        d = { 'expression_methods': methods }
        return expression_cc % d

class Numeric(object):
    def __init__(self, nodes):
        self._nodes = nodes

    @property
    def nodes(self):
        return self._nodes

    @property
    def header(self):
        fwd_decls = ''
        cast_methods = ''
        for node in self.nodes:
            d = { 'classname': node.typename }
            fwd_decls += numeric_fwd_decl % d
            cast_methods += numeric_cast_method % d
        d = { 'fwd_decls': fwd_decls, 'cast_methods': cast_methods }
        return numeric_hh %d

    @property
    def source(self):
        methods = ''
        for node in self.nodes:
            d = { 'classname': node.typename }
            methods += numeric_method % d
        d = { 'numeric_methods': methods }
        return numeric_cc % d
