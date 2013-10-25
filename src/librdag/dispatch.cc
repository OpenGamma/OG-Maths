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


template class DispatchUnaryOp<void *>;
template class DispatchBinaryOp<void *>;

Dispatcher::Dispatcher()
{
    _PlusRunner = new PlusRunner();
    _NegateRunner = new NegateRunner();
}


Dispatcher::~Dispatcher(){
  delete _PlusRunner;
  delete _NegateRunner;
}
  
void Dispatcher::dispatch(OGNumeric const *thing) const
{
    DEBUG_PRINT("Dispatching...\n");
    ExprType_t ID = thing->getType();
    DEBUG_PRINT("TYPE IS %d\n", ID);
    bool isTerminalType = false;
    if(ID&IS_NODE_MASK){
      DEBUG_PRINT("is node...\n");
      thing->asOGExpr();
    } else {
      isTerminalType = true;      
      DEBUG_PRINT("is terminal...\n");
    }

    // branch switch on isTerminalType?
    if(isTerminalType) {
      switch(ID)
      {
        case REAL_SCALAR_ENUM:
          dispatch(thing->asOGRealScalar());
          break;
        case COMPLEX_SCALAR_ENUM:
          dispatch(thing->asOGComplexScalar());
          break;
        case REAL_MATRIX_ENUM:
          dispatch(thing->asOGRealMatrix());
          break;
        case COMPLEX_MATRIX_ENUM:
          dispatch(thing->asOGComplexMatrix());
          break;
        case REAL_SPARSE_MATRIX_ENUM:
          dispatch(thing->asOGRealSparseMatrix());
          break;
        case COMPLEX_SPARSE_MATRIX_ENUM:
          dispatch(thing->asOGComplexSparseMatrix());
          break;
        case REAL_DIAGONAL_MATRIX_ENUM:
          dispatch(thing->asOGRealDiagonalMatrix());
          break;
        case COMPLEX_DIAGONAL_MATRIX_ENUM:
          dispatch(thing->asOGComplexDiagonalMatrix());
          break;
        case LOGICAL_MATRIX_ENUM:
          dispatch(thing->asOGRealMatrix()); // note dispatch type as base class T
          break;
        case INTEGER_SCALAR_ENUM:
          dispatch(thing->asOGIntegerScalar());
          break;          
        default:
          DEBUG_PRINT("NO SPECIFIC TYPE \n");
      }     
    }
    else
    {
      switch(ID)
      {
        case PLUS_ENUM:
          dispatch(thing->asPLUS());
          break;
        case NEGATE_ENUM:
          dispatch(thing->asNEGATE());
          break;
        case COPY_ENUM:
          dispatch(thing->asCOPY());
          break;
        case SVD_ENUM:
          dispatch(thing->asSVD());
          break;
        case SELECTRESULT_ENUM:
          dispatch(thing->asSELECTRESULT());
          break;
        default:
          DEBUG_PRINT("NO SPECIFIC TYPE \n");
      }
    }
}
  
// specific terminal dispatches
void Dispatcher::dispatch(OGRealScalar const SUPPRESS_UNUSED * thing) const {

}

void Dispatcher::dispatch(OGComplexScalar const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGIntegerScalar const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGRealMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGLogicalMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGComplexMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGRealDiagonalMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGComplexDiagonalMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGRealSparseMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGComplexSparseMatrix const SUPPRESS_UNUSED * thing) const {
  
}
  
void Dispatcher::dispatch(PLUS const *thing) const {
      DEBUG_PRINT("ABOUT TO DISPATCH A PLUS OP\n");
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
      this->_PlusRunner->eval(const_cast<RegContainer *>(regs), arg0t, arg1t);
}

void Dispatcher::dispatch(NEGATE const SUPPRESS_UNUSED *thing) const {
      DEBUG_PRINT("ABOUT TO DISPATCH A NEGATE OP\n");
      const ArgContainer* args = thing->getArgs();
      const RegContainer* regs = thing->getRegs();
      const OGNumeric *arg = (*args)[0];
      const OGTerminal *argt = arg->asOGTerminal();
      if (argt == nullptr)
      {
        argt = (*(arg->asOGExpr()->getRegs()))[0]->asOGTerminal();
      }
      _NegateRunner->eval(const_cast<RegContainer *>(regs), argt);
}

void Dispatcher::dispatch(COPY const SUPPRESS_UNUSED *thing) const {
      DEBUG_PRINT("ABOUT TO DISPATCH A COPY OP\n");
}

void Dispatcher::dispatch(SVD const SUPPRESS_UNUSED *thing) const {
      DEBUG_PRINT("ABOUT TO DISPATCH A SVD OP\n");
}

void Dispatcher::dispatch(SELECTRESULT const SUPPRESS_UNUSED *thing) const {
      DEBUG_PRINT("ABOUT TO DISPATCH A SELECTRESULT OP\n");
}
  
  

} // namespace librdag


