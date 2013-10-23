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


namespace librdag {

// fwd decl
class PlusRunner;
class NegateRunner;

/**
 * The class for dispatching execution based on OGNumeric type
 */
class Dispatcher
{
public:
  Dispatcher();  
  virtual ~Dispatcher();
  void dispatch(OGNumeric const * thing) const;
  
  // specific terminal dispatches
  virtual void dispatch(OGRealScalar const * thing) const;
  virtual void dispatch(OGComplexScalar const * thing) const;
  virtual void dispatch(OGIntegerScalar const * thing) const;
  virtual void dispatch(OGRealMatrix const * thing) const;
  virtual void dispatch(OGLogicalMatrix const * thing) const;
  virtual void dispatch(OGComplexMatrix const * thing) const;
  virtual void dispatch(OGRealDiagonalMatrix const * thing) const;
  virtual void dispatch(OGComplexDiagonalMatrix const * thing) const;
  virtual void dispatch(OGRealSparseMatrix const * thing) const;
  virtual void dispatch(OGComplexSparseMatrix const * thing) const;
  
  // specific node dispatches
  virtual void dispatch(PLUS const * thing) const;
  virtual void dispatch(NEGATE const * thing) const;
  virtual void dispatch(COPY const * thing) const;
  virtual void dispatch(SVD const * thing) const;
  virtual void dispatch(SELECTRESULT const * thing) const;
  
private:
  PlusRunner * _PlusRunner;
  NegateRunner* _NegateRunner;
  
};

/**
 * For dispatching operations of the form "T foo(Register * register1, OGTerminal * arg)"
 */
template<typename T> class  DispatchUnaryOp
{
public:
  virtual ~DispatchUnaryOp(){};

  // will run the operation
  T eval(RegContainer SUPPRESS_UNUSED * reg, OGTerminal const * arg) const
  {
    ExprType_t argID = arg->getType();
    switch(argID)
    {
      case REAL_SPARSE_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealSparseMatrix());\n");
        run(reg, arg->asOGRealSparseMatrix());
        break;
      case REAL_SCALAR_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealScalar());\n");
        run(reg, arg->asOGRealScalar());
        break;
      case REAL_DIAGONAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealDiagonalMatrix());\n");
        run(reg, arg->asOGRealDiagonalMatrix());
        break;
      case INTEGER_SCALAR_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGIntegerScalar());\n");
        run(reg, arg->asOGIntegerScalar());
        break;
      case COMPLEX_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexMatrix());\n");
        run(reg, arg->asOGComplexMatrix());
        break;
      case COMPLEX_DIAGONAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexDiagonalMatrix());\n");
        run(reg, arg->asOGComplexDiagonalMatrix());
        break;
      case COMPLEX_SCALAR_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexScalar());\n");
        run(reg, arg->asOGComplexScalar());
        break;
      case LOGICAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGLogicalMatrix());\n");
        run(reg, arg->asOGLogicalMatrix());
        break;
      case REAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealMatrix());\n");
        run(reg, arg->asOGRealMatrix());
        break;
      case COMPLEX_SPARSE_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexSparseMatrix());\n");
        run(reg, arg->asOGComplexSparseMatrix());
        break;
      default:
          throw rdag_error("Unknown type in dispatch on arg");
    }
  }

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGRealScalar const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGRealScalar
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGComplexScalar const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGComplexScalar
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGIntegerScalar const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGIntegerScalar
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGLogicalMatrix const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGLogicalMatrix
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGRealDiagonalMatrix
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGComplexDiagonalMatrix
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGRealSparseMatrix const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGRealSparseMatrix
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg) const
  {
    // impl convert and run for type OGComplexSparseMatrix
  };

  // Backstop methods for generic implementation
  virtual T run(RegContainer * reg, OGRealMatrix const * arg) const = 0;
  virtual T run(RegContainer * reg, OGComplexMatrix const * arg) const = 0;
};


/**
 * For dispatching operations of the form "T foo(Register * register1, OGTerminal * arg0, OGTerminal * arg1)"
 */
template<typename T> class  DispatchBinaryOp
{
public:
virtual ~DispatchBinaryOp(){};
 
// will run the operation
T eval(RegContainer SUPPRESS_UNUSED * reg0, OGTerminal const *arg0, OGTerminal const *arg1) const
{
  ExprType_t arg0ID = arg0->getType();
  ExprType_t arg1ID = arg1->getType();
  // massive switch table
  switch(arg0ID)
  {
    case REAL_SPARSE_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_SPARSE_MATRIX_ENUM
    case REAL_SCALAR_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_SCALAR_ENUM
    case REAL_DIAGONAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_DIAGONAL_MATRIX_ENUM
    case INTEGER_SCALAR_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == INTEGER_SCALAR_ENUM
    case COMPLEX_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_MATRIX_ENUM
    case COMPLEX_DIAGONAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_DIAGONAL_MATRIX_ENUM
    case COMPLEX_SCALAR_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_SCALAR_ENUM
    case LOGICAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == LOGICAL_MATRIX_ENUM
    case REAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_MATRIX_ENUM
    case COMPLEX_SPARSE_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealSparseMatrix());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case REAL_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealScalar());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealScalar());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case INTEGER_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGIntegerScalar());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGIntegerScalar());
              break;
          case COMPLEX_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexMatrix());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexDiagonalMatrix());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          case COMPLEX_SCALAR_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexScalar());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexScalar());
              break;
          case LOGICAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGLogicalMatrix());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGLogicalMatrix());
              break;
          case REAL_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealMatrix());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              DEBUG_PRINT("running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexSparseMatrix());\n");
              run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_SPARSE_MATRIX_ENUM
    default:
        throw rdag_error("Unknown type in dispatch on arg0");
  }

  
};

// specific methods
//  MAT="OGRealScalar OGComplexScalar OGIntegerScalar OGRealMatrix OGLogicalMatrix OGComplexMatrix OGRealDiagonalMatrix OGComplexDiagonalMatrix OGRealSparseMatrix OGComplexSparseMatrix"; for i in $MAT; do for j in $MAT; do printf "virtual T run(RegContainer SUPPRESS_UNUSED * reg0, ${i} const SUPPRESS_UNUSED * arg0, ${j} const SUPPRESS_UNUSED * arg1) const \n{\n  // impl convert and run for types ${i} and ${j} \n};\n\n";done;done
virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGIntegerScalar 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGLogicalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexDiagonalMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealSparseMatrix 
};

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexSparseMatrix 
};


// Required backstop impls
virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const * arg0, OGComplexMatrix const * arg1) const = 0;

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const * arg0, OGRealMatrix const * arg1) const = 0;

};

// typedef the void dispatch of a binary operation
typedef DispatchBinaryOp<void> DispatchVoidBinaryOp;

// typedef the void dispatch of a unary operation
typedef DispatchUnaryOp<void> DispatchVoidUnaryOp;

} // namespace librdag

#endif // _DISPATCH_HH

