#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

dispatch_header = """\
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _DISPATCH_HH
#define _DISPATCH_HH

#include "numerictypes.hh"
#include "expression.hh"
#include "terminal.hh"
#include "warningmacros.h"
#include "jvmmanager.hh"
#include "exceptions.hh"
#include "debug.h"
#include "convertto.hh"

namespace librdag {

%(dispatcher_definition)s

%(dispatchop_definition)s

%(dispatchunary_definition)s

%(dispatchbinary_definition)s

// typedef the void dispatches
typedef DispatchUnaryOp<void*> DispatchVoidUnaryOp;
typedef DispatchBinaryOp<void*> DispatchVoidBinaryOp;

/**
 * Template instantiations
 */

extern template class DispatchUnaryOp<void*>;
extern template class DispatchBinaryOp<void*>;

} // end namespace librdag

# endif // _DISPATCH_HH
"""

dispatcher_class = """\
// Forward declarations
%(dispatcher_forward_decls)s

/**
 * The class for dispatching execution based on OGNumeric type
 */
class Dispatcher
{
  public:
    Dispatcher();
    virtual ~Dispatcher();
    void dispatch(OGNumeric const * thing) const;

    // Specific terminal dispatches
%(dispatcher_terminal_dispatches)s

    // Specific node dispatches
%(dispatcher_node_dispatches)s

  private:
%(dispatcher_private_members)s
};
"""

dispatcher_forward_decl = """\
class %(nodetype)sRunner;
"""

dispatcher_dispatch_prototype = """\
    virtual void dispatch(const %(nodetype)s* thing) const;
"""

dispatcher_private_member = """\
    %(nodetype)sRunner* _%(nodetype)sRunner;
"""

dispatchop_class = """\
class DispatchOp
{
  public:
    DispatchOp();
    virtual ~DispatchOp();
    const ConvertTo * getConvertTo() const;
  private:
    const ConvertTo * _convert;
};
"""

dispatchunaryop_class = """\
/**
 * For dispatching operations of the form "T foo(Register * register1, OGTerminal * arg)"
 */
template<typename T> class DispatchUnaryOp: public DispatchOp
{
  static_assert(is_pointer<T>::value, "Type T must be a pointer");
  public:
    using DispatchOp::getConvertTo;
    virtual ~DispatchUnaryOp();

    // will run the operation
    T eval(RegContainer* reg, const OGTerminal* arg) const;
    // Methods for specific terminals
%(dispatchunaryop_terminal_methods)s
    // Backstop methods for generic implementation
    virtual T run(RegContainer * reg, OGRealMatrix const * arg) const = 0;
    virtual T run(RegContainer * reg, OGComplexMatrix const * arg) const = 0;
};
"""

dispatchunaryop_run = """\
    virtual T run(RegContainer* reg, const %(nodetype)s* arg) const;
"""

dispatchbinaryop_class = """\
/**
 * For dispatching operations of the form "T foo(Register * register1, OGTerminal * arg0, OGTerminal * arg1)"
 */
template<typename T> class  DispatchBinaryOp: public DispatchOp
{
  static_assert(is_pointer<T>::value, "Type T must be a pointer");
  public:
    using DispatchOp::getConvertTo;
    virtual ~DispatchBinaryOp();
    // will run the operation
    T eval(RegContainer SUPPRESS_UNUSED * reg0, OGTerminal const *arg0, OGTerminal const *arg1) const;

    // Methods for specific terminals
%(dispatchbinaryop_terminal_methods)s
    // Required backstop impls
    virtual T run(RegContainer* reg0, const OGComplexMatrix* arg0, const OGComplexMatrix* arg1) const = 0;
    virtual T run(RegContainer* reg0, const OGRealMatrix* arg0, const OGRealMatrix* arg1) const = 0;
};
"""

dispatchbinaryop_run = """\
    virtual T run(RegContainer* reg0, const %(node0type)s* arg0, const %(node1type)s* arg1) const;
"""

# Dispatch cc file

dispatch_cc = """\
/*
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * See distribution for licence.
 *
 */

#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "terminal.hh"
#include "warningmacros.h"
#include "uncopyable.hh"
#include <iostream>

namespace librdag {

/**
 *  Dispatcher
 */

%(dispatcher_methods)s

/**
 * DispatchOp
 */

%(dispatchop_methods)s

/**
 * DispatchUnaryOp
 */

%(dispatchunaryop_methods)s

/**
 * DispatchBinaryOp
 */

%(dispatchbinaryop_methods)s

/**
 * Template instantiations
 */

template class DispatchUnaryOp<void*>;
template class DispatchBinaryOp<void*>;

} // namespace librdag
"""
# Dispatcher methods

dispatcher_methods = """\
%(dispatcher_constructor)s

%(dispatcher_destructor)s

%(dispatcher_dispatch)s

// Specific terminal dispatches

%(dispatcher_terminal_dispatches)s

// Specific node dispatches

%(dispatcher_node_dispatches)s
"""

dispatcher_constructor = """\
Dispatcher::Dispatcher()
{
%(member_initialisers)s\
}
"""

dispatcher_member_initialiser = """\
  _%(nodetype)sRunner = new %(nodetype)sRunner();
"""

dispatcher_member_deleter = """\
  delete _%(nodetype)sRunner;
"""

dispatcher_destructor = """\
Dispatcher::~Dispatcher(){
%(member_deleters)s\
}
"""

dispatcher_dispatch_numeric = """\
void
Dispatcher::dispatch(OGNumeric const *thing) const
{
  DEBUG_PRINT("Dispatching...\\n");
  ExprType_t ID = thing->getType();
  DEBUG_PRINT("TYPE IS %%d\\n", ID);
  bool isTerminalType = false;
  if (ID & IS_NODE_MASK)
  {
    DEBUG_PRINT("is node...\\n");
  }
  else
  {
    isTerminalType = true;
    DEBUG_PRINT("is terminal...\\n");
  }

  // branch switch on isTerminalType?
  if(isTerminalType) {
    switch(ID)
    {
%(dispatch_terminal_cases)s
      default:
        throw rdag_error("Unknown terminal type in dispatch");
    }
  }
  else
  {
    switch(ID)
    {
%(dispatch_expr_cases)s
      default:
        throw rdag_error("Unknown expression type in dispatch");
    }
  }
}
"""

dispatcher_case = """\
      case %(nodeenumtype)s:
        dispatch(thing->as%(nodetype)s());
        break;
"""

dispatcher_dispatch = """\
void
Dispatcher::dispatch(%(nodetype)s const SUPPRESS_UNUSED * thing) const
{
%(dispatch_implementation)s
}
"""

dispatcher_binary_implementation = """\
  const ArgContainer * args = thing->getArgs();
  const RegContainer * regs = thing->getRegs();
  const OGNumeric * arg0 = (*args)[0];
  const OGNumeric * arg1 = (*args)[1];
  const OGTerminal* arg0t = arg0->asOGTerminal();
  const OGTerminal* arg1t = arg1->asOGTerminal();
  if (arg0t == nullptr)
  {
    arg0t = (*(arg0->asOGExpr()->getRegs()))[0]->asOGTerminal();
  }
  if (arg1t == nullptr)
  {
    arg1t = (*(arg1->asOGExpr()->getRegs()))[0]->asOGTerminal();
  }
  this->_%(nodetype)sRunner->eval(const_cast<RegContainer *>(regs), arg0t, arg1t);
"""

dispatcher_unary_implementation = """\
  const ArgContainer* args = thing->getArgs();
  const RegContainer* regs = thing->getRegs();
  const OGNumeric *arg = (*args)[0];
  const OGTerminal *argt = arg->asOGTerminal();
  if (argt == nullptr)
  {
    argt = (*(arg->asOGExpr()->getRegs()))[0]->asOGTerminal();
  }
  _%(nodetype)sRunner->eval(const_cast<RegContainer *>(regs), argt);
"""

# DispatchOp methods

dispatchop_methods = """\
DispatchOp::DispatchOp()
{
  _convert = new ConvertTo();
}

const ConvertTo *
DispatchOp::getConvertTo() const
{
  return _convert;
}

DispatchOp::~DispatchOp()
{
  delete _convert;
}
"""

# DispatchUnaryOp methods

dispatchunaryop_methods = """\
%(dispatchunaryop_destructor)s

%(dispatchunaryop_eval)s

%(dispatchunaryop_terminals)s
"""

dispatchunaryop_destructor = """\
template <typename T>
DispatchUnaryOp<T>::~DispatchUnaryOp() {}
"""

dispatchunaryop_eval = """\
template<typename T>
T
DispatchUnaryOp<T>::eval(RegContainer* reg, const OGTerminal* arg) const
{
  ExprType_t argID = arg->getType();
  T ret = nullptr;
  switch(argID)
  {
%(eval_cases)s
    default:
        throw rdag_error("Unknown type in dispatch on arg");
  }
  return ret;
}
"""

dispatchunaryop_eval_case = """\
    case %(nodeenumtype)s:
      DEBUG_PRINT("running with run(reg, arg->as%(nodetype)s);\\n");
      ret = run(reg, arg->as%(nodetype)s());
      break;
"""

dispatchunaryop_terminal_method = """\
template<typename T>
T
DispatchUnaryOp<T>::run(RegContainer* reg, const %(nodetype)s* arg) const
{
  %(typetoconvertto)s* conv = this->getConvertTo()->convertTo%(typetoconvertto)s(arg);
  T ret = run(reg, conv);
  delete conv;
  return ret;
}
"""

# DispatchBinaryOp methods

dispatchbinaryop_methods = """\
%(dispatchbinaryop_destructor)s

%(dispatchbinaryop_eval)s

%(dispatchbinaryop_terminals)s
"""

dispatchbinaryop_destructor = """\
template <typename T>
DispatchBinaryOp<T>::~DispatchBinaryOp(){}
"""

dispatchbinaryop_eval = """\
template <typename T>
T
DispatchBinaryOp<T>::eval(RegContainer* reg0, OGTerminal const *arg0, OGTerminal const *arg1) const
{
  ExprType_t arg0ID = arg0->getType();
  ExprType_t arg1ID = arg1->getType();
  T ret = nullptr;
  // MASSIVE SWITCH TABLE
  switch(arg0ID)
  {
%(eval_cases)s
    default:
      throw rdag_error("Unknown type in dispatch on arg0");
  }
  return ret;
}
"""

dispatchbinaryop_eval_case_arg0 = """\
    case %(node0enumtype)s:
      switch(arg1ID)
      {
%(eval_arg1_cases)s
        default:
          throw rdag_error("Unknown type in dispatch on arg1");
      }
      break;
"""

dispatchbinaryop_eval_case_arg1 = """\
        case %(node1enumtype)s:
          DEBUG_PRINT("running with run(reg0, arg0->as%(node0type)s(), arg1->as%(node1type)s());\\n");
          ret = run(reg0, arg0->as%(node0type)s(), arg1->as%(node1type)s());
          break;
"""

dispatchbinaryop_terminal_method = """\
template<typename T>
T
DispatchBinaryOp<T>::run(RegContainer* reg0,
                         const %(node0type)s* arg0,
                         const %(node1type)s* arg1) const
{
%(conv0)s
%(conv1)s
  T ret = run(reg0, conv0, conv1);
%(deletions)s
  return ret;
}
"""

dispatchbinaryop_conv_arg = """\
  %(typetoconvertto)s* conv%(argno)s = this->getConvertTo()->convertTo%(typetoconvertto)s(arg%(argno)s);
"""

dispatchbinaryop_noconv_arg = """\
  const %(nodetype)s* conv%(argno)s = arg%(argno)s;
"""

dispatchbinaryop_deletion = """\
  delete conv%(argno)s;
"""
