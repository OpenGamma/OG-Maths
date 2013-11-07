#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

from templates import dispatch_header, dispatcher_class, dispatcher_forward_decl, \
                      dispatcher_dispatch_prototype, dispatcher_private_member, \
                      dispatchunaryop_class, dispatchunaryop_run, \
                      dispatchbinaryop_class, dispatchbinaryop_run, \
                      dispatcher_methods, dispatch_cc, dispatcher_member_initialiser, \
                      dispatcher_member_deleter, dispatcher_constructor, \
                      dispatcher_destructor, dispatcher_case, dispatcher_dispatch, \
                      dispatcher_dispatch_numeric, dispatcher_unary_implementation, \
                      dispatcher_binary_implementation, dispatchunaryop_methods, \
                      dispatchunaryop_destructor, dispatchunaryop_eval, \
                      dispatchunaryop_eval_case, dispatchunaryop_terminal_method, \
                      dispatchbinaryop_methods, dispatchbinaryop_destructor, \
                      dispatchbinaryop_eval, dispatchbinaryop_eval_case_arg0, \
                      dispatchbinaryop_eval_case_arg1, dispatchbinaryop_terminal_method, \
                      dispatchop_class, dispatchbinaryop_conv_arg, dispatchop_methods, \
                      dispatchbinaryop_noconv_arg

class Dispatcher(object):
    """Generates the Dispatcher class definition and method implementations"""
    def __init__(self, terminals, nodes):
        self._terminals = terminals
        self._nodes = nodes

    @property
    def class_definition(self):
        dispatcher_terminal_dispatches = ""
        for t in self._terminals:
            d = { 'nodetype': t.name }
            dispatcher_terminal_dispatches += dispatcher_dispatch_prototype % d

        dispatcher_forward_decls = ""
        dispatcher_node_dispatches = ""
        dispatcher_private_members = ""
        for n in self._nodes:
            d = { 'nodetype': n.nodename }
            dispatcher_forward_decls += dispatcher_forward_decl % d
            dispatcher_node_dispatches += dispatcher_dispatch_prototype % d
            dispatcher_private_members += dispatcher_private_member % d

        d = { 'dispatcher_forward_decls': dispatcher_forward_decls,
              'dispatcher_terminal_dispatches': dispatcher_terminal_dispatches,
              'dispatcher_node_dispatches': dispatcher_node_dispatches,
              'dispatcher_private_members': dispatcher_private_members }
        return dispatcher_class % d

    @property
    def method_definitions(self):
        # Constructor/destructor
        initialisers = ''
        deleters = ''
        for n in self._nodes:
            d = { 'nodetype': n.nodename }
            initialisers += dispatcher_member_initialiser % d
            deleters += dispatcher_member_deleter % d
        d = { 'member_initialisers': initialisers }
        constructor = dispatcher_constructor % d
        d = { 'member_deleters': deleters }
        destructor = dispatcher_destructor % d
        # Dispatch method
        dispatch_terminal_cases = ''
        for t in self._terminals:
            d = { 'nodetype': t.name, 'nodeenumtype': t.enumname }
            dispatch_terminal_cases += dispatcher_case % d
        dispatch_expr_cases = ''
        for n in self._nodes:
            d = { 'nodetype': n.nodename, 'nodeenumtype': n.enumname }
            dispatch_expr_cases += dispatcher_case % d
        d = { 'dispatch_terminal_cases': dispatch_terminal_cases,
              'dispatch_expr_cases': dispatch_expr_cases }
        dispatch_numeric = dispatcher_dispatch_numeric % d
        # Dispatch terminal methods
        dispatch_terminals = ""
        for t in self._terminals:
            d = { 'nodetype': t.name, 'dispatch_implementation': '' }
            dispatch_terminals += dispatcher_dispatch % d
        # Dispatch node methods
        dispatch_exprs = ''
        for n in self._nodes:
            d = { 'nodetype': n.nodename }
            if n.argcount == 1:
                d['dispatch_implementation'] = dispatcher_unary_implementation % d
            elif n.argcount == 2:
                d['dispatch_implementation'] = dispatcher_binary_implementation % d
            dispatch_exprs += dispatcher_dispatch % d
        # Dispatcher methods
        d = { 'dispatcher_constructor':         constructor,
              'dispatcher_destructor':          destructor,
              'dispatcher_dispatch':            dispatch_numeric,
              'dispatcher_terminal_dispatches': dispatch_terminals,
              'dispatcher_node_dispatches':     dispatch_exprs }
        return dispatcher_methods % d

class DispatchUnaryOp(object):
    """Generates the DispatchUnaryOp class definition and method implementations"""
    def __init__(self, terminals, nodes):
        self._terminals = terminals
        self._nodes = nodes
        self._backstop_terminals = [ 'OGRealMatrix', 'OGComplexMatrix' ]

    @property
    def class_definition(self):
        dispatchunaryop_terminal_methods = ""
        for t in self._terminals:
            if t.name in self._backstop_terminals:
                continue
            d = { 'nodetype': t.name }
            dispatchunaryop_terminal_methods += dispatchunaryop_run % d
        d = { 'dispatchunaryop_terminal_methods': dispatchunaryop_terminal_methods }
        return dispatchunaryop_class % d

    @property
    def method_definitions(self):
        eval_cases = ''
        terminal_methods = ''
        for t in self._terminals:
            d = { 'nodetype': t.name, 'nodeenumtype': t.enumname, \
                  'typetoconvertto': 'OGComplexMatrix' }
                  # FIXME: This is the dumbest possible choice.
            eval_cases += dispatchunaryop_eval_case % d
            if t.name not in self._backstop_terminals:
                terminal_methods += dispatchunaryop_terminal_method % d
        d = { 'eval_cases': eval_cases }
        eval_method = dispatchunaryop_eval % d
        # DispatchUnaryOp methods
        d = { 'dispatchunaryop_destructor': dispatchunaryop_destructor,
              'dispatchunaryop_eval':       eval_method,
              'dispatchunaryop_terminals':  terminal_methods }
        return dispatchunaryop_methods % d


class DispatchBinaryOp(object):
    """Generates the DispatchBinaryOp class definition and method implementations"""
    def __init__(self, terminals, nodes):
        self._terminals = terminals
        self._nodes = nodes
        self._backstop_terminals = [ 'OGRealMatrix', 'OGComplexMatrix' ]

    @property
    def class_definition(self):
        dispatchbinaryop_terminal_methods = ""
        for t0 in self._terminals:
            for t1 in self._terminals:
                if t0.name in self._backstop_terminals and t0 == t1:
                    continue
                d = { 'node0type': t0.name, 'node1type': t1.name }
                dispatchbinaryop_terminal_methods += dispatchbinaryop_run % d
        d = { 'dispatchbinaryop_terminal_methods': dispatchbinaryop_terminal_methods }
        return dispatchbinaryop_class %d

    @property
    def method_definitions(self):
        eval_cases = ''
        terminal_methods = ''
        for t0 in self._terminals:
            if t0.name == 'OGComplexMatrix':
                d = { 'argno': '0', 'nodetype': 'OGComplexMatrix' }
                conv0 = dispatchbinaryop_noconv_arg % d
            else:
                d = { 'argno': '0', 'typetoconvertto': 'OGComplexMatrix' }
                conv0 = dispatchbinaryop_conv_arg %d
            eval_arg1_cases = ''
            for t1 in self._terminals:
                if t1.name == 'OGComplexMatrix':
                    d = { 'argno': '1', 'nodetype': 'OGComplexMatrix' }
                    conv1 = dispatchbinaryop_noconv_arg % d
                else:
                    d = { 'argno': '1', 'typetoconvertto': 'OGComplexMatrix' }
                    conv1 = dispatchbinaryop_conv_arg %d
                d = { 'node0type': t0.name, 'node0enumtype': t0.enumname,
                      'node1type': t1.name, 'node1enumtype': t1.enumname,
                      'conv0': conv0,
                      'conv1': conv1 }
                      # FIXME: Those types are the dumbest possible choices.
                eval_arg1_cases += dispatchbinaryop_eval_case_arg1 % d
                if t0.name not in self._backstop_terminals or t0 != t1:
                    terminal_methods += dispatchbinaryop_terminal_method %d
            d = { 'node0type': t0.name, 'node0enumtype': t0.enumname,
                  'eval_arg1_cases': eval_arg1_cases }
            eval_cases += dispatchbinaryop_eval_case_arg0 % d
        d = { 'eval_cases': eval_cases }
        eval_method = dispatchbinaryop_eval % d
        # DispatchBinaryOp methods
        d = { 'dispatchbinaryop_destructor': dispatchbinaryop_destructor,
              'dispatchbinaryop_eval':       eval_method,
              'dispatchbinaryop_terminals':  terminal_methods }
        return dispatchbinaryop_methods % d

class Dispatch(object):
    """Generates the dispatch.hh and .cc files."""
    def __init__(self, terminals, nodes):
        self._terminals = terminals
        self._nodes = nodes
        self._dispatcher = Dispatcher(terminals, nodes)
        self._dispatchunaryop = DispatchUnaryOp(terminals, nodes)
        self._dispatchbinaryop = DispatchBinaryOp(terminals, nodes)

    @property
    def header(self):
        d = { 'dispatcher_definition': self._dispatcher.class_definition,
              'dispatchop_definition': dispatchop_class,
              'dispatchunary_definition': self._dispatchunaryop.class_definition,
              'dispatchbinary_definition': self._dispatchbinaryop.class_definition }
        return dispatch_header % d

    @property
    def source(self):
        d = { 'dispatcher_methods':       self._dispatcher.method_definitions,
              'dispatchop_methods':       dispatchop_methods,
              'dispatchunaryop_methods':  self._dispatchunaryop.method_definitions,
              'dispatchbinaryop_methods': self._dispatchbinaryop.method_definitions }
        return dispatch_cc % d
