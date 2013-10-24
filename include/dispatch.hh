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

class DispatchOp
{
  public:
    DispatchOp();
    virtual ~DispatchOp();
    const ConvertTo * getConvertTo() const;
  private:
    const ConvertTo * _convert;
};

/**
 * For dispatching operations of the form "T foo(Register * register1, OGTerminal * arg)"
 */
template<typename T> class  DispatchUnaryOp: public DispatchOp
{
static_assert(is_pointer<T>::value, "Type T must be a pointer");

public:
  virtual ~DispatchUnaryOp(){};
  using DispatchOp::getConvertTo;
  // will run the operation
  T eval(RegContainer SUPPRESS_UNUSED * reg, OGTerminal const * arg) const
  {
    ExprType_t argID = arg->getType();
    T ret = nullptr;
    switch(argID)
    {
      case REAL_SPARSE_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealSparseMatrix());\n");
        ret = run(reg, arg->asOGRealSparseMatrix());
        break;
      case REAL_SCALAR_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealScalar());\n");
        ret = run(reg, arg->asOGRealScalar());
        break;
      case REAL_DIAGONAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealDiagonalMatrix());\n");
        ret = run(reg, arg->asOGRealDiagonalMatrix());
        break;
      case INTEGER_SCALAR_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGIntegerScalar());\n");
        ret = run(reg, arg->asOGIntegerScalar());
        break;
      case COMPLEX_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexMatrix());\n");
        ret = run(reg, arg->asOGComplexMatrix());
        break;
      case COMPLEX_DIAGONAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexDiagonalMatrix());\n");
        ret = run(reg, arg->asOGComplexDiagonalMatrix());
        break;
      case COMPLEX_SCALAR_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexScalar());\n");
        ret = run(reg, arg->asOGComplexScalar());
        break;
      case LOGICAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGLogicalMatrix());\n");
        ret = run(reg, arg->asOGLogicalMatrix());
        break;
      case REAL_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGRealMatrix());\n");
        ret = run(reg, arg->asOGRealMatrix());
        break;
      case COMPLEX_SPARSE_MATRIX_ENUM:
        DEBUG_PRINT("running with run(reg, arg->asOGComplexSparseMatrix());\n");
        ret = run(reg, arg->asOGComplexSparseMatrix());
        break;
      default:
          throw rdag_error("Unknown type in dispatch on arg");
    }
    return ret;
  }

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGRealScalar const SUPPRESS_UNUSED * arg) const
  {
    OGOwningRealMatrix * conv = this->getConvertTo()->convertToOGRealMatrix(arg);
    T ret = run(reg, conv);
    delete conv;
    return ret;
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGComplexScalar const SUPPRESS_UNUSED * arg) const
  {
//     return run(reg, this->getConvertTo()->convertToOGComplexMatrix(arg));
    return nullptr;
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGIntegerScalar const SUPPRESS_UNUSED * arg) const
  {
//     return run(reg, this->getConvertTo()->convertToOGRealMatrix(arg));
    return nullptr;
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGLogicalMatrix const SUPPRESS_UNUSED * arg) const
  {
//     return run(reg, this->getConvertTo()->convertToOGRealMatrix(arg));
    return nullptr;
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg) const
  {
//     return run(reg, this->getConvertTo()->convertToOGRealMatrix(arg));
    return nullptr;
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg) const
  {
//     return run(reg, this->getConvertTo()->convertToOGComplexMatrix(arg));
    return nullptr;
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGRealSparseMatrix const SUPPRESS_UNUSED * arg) const
  {
//     return run(reg, this->getConvertTo()->convertToOGRealMatrix(arg));
    return nullptr;
  };

  virtual T run(RegContainer SUPPRESS_UNUSED * reg, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg) const
  {
//     return run(reg, this->getConvertTo()->convertToOGComplexMatrix(arg));
    return nullptr;
  };

  // Backstop methods for generic implementation
  virtual T run(RegContainer * reg, OGRealMatrix const * arg) const = 0;
  virtual T run(RegContainer * reg, OGComplexMatrix const * arg) const = 0;
};


/**
 * For dispatching operations of the form "T foo(Register * register1, OGTerminal * arg0, OGTerminal * arg1)"
 */
template<typename T> class  DispatchBinaryOp: public DispatchOp
{
static_assert(is_pointer<T>::value, "Type T must be a pointer");
public:
virtual ~DispatchBinaryOp(){};
using DispatchOp::getConvertTo;

// will run the operation
T eval(RegContainer SUPPRESS_UNUSED * reg0, OGTerminal const *arg0, OGTerminal const *arg1) const
{
  ExprType_t arg0ID = arg0->getType();
  ExprType_t arg1ID = arg1->getType();
  T ret = nullptr;
  // massive switch table
  switch(arg0ID)
  {
    case REAL_SCALAR_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealScalar(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_SCALAR_ENUM
    case COMPLEX_SCALAR_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexScalar(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_SCALAR_ENUM
    case INTEGER_SCALAR_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGIntegerScalar(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == INTEGER_SCALAR_ENUM
    case REAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_MATRIX_ENUM
    case LOGICAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGLogicalMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == LOGICAL_MATRIX_ENUM
    case COMPLEX_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_MATRIX_ENUM
    case REAL_SPARSE_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealSparseMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_SPARSE_MATRIX_ENUM
    case COMPLEX_SPARSE_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexSparseMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_SPARSE_MATRIX_ENUM
    case REAL_DIAGONAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGRealDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == REAL_DIAGONAL_MATRIX_ENUM
    case COMPLEX_DIAGONAL_MATRIX_ENUM:
        switch(arg1ID)
        {
          case REAL_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealScalar());
              break;
          case COMPLEX_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexScalar());
              break;
          case INTEGER_SCALAR_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGIntegerScalar());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGIntegerScalar());
              break;
          case REAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealMatrix());
              break;
          case LOGICAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGLogicalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGLogicalMatrix());
              break;
          case COMPLEX_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexMatrix());
              break;
          case REAL_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealSparseMatrix());
              break;
          case COMPLEX_SPARSE_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexSparseMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexSparseMatrix());
              break;
          case REAL_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGRealDiagonalMatrix());
              break;
          case COMPLEX_DIAGONAL_MATRIX_ENUM:
              cout << "running with run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());" << std::endl;
              ret = run(reg0, arg0->asOGComplexDiagonalMatrix(), arg1->asOGComplexDiagonalMatrix());
              break;
          default:
              throw rdag_error("Unknown type in dispatch on arg1");
        }
        break; // end case: arg0ID == COMPLEX_DIAGONAL_MATRIX_ENUM
    default:
        throw rdag_error("Unknown type in dispatch on arg0");
  }
  return ret;
};

// specific methods

   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGRealScalar
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGComplexScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGRealMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGComplexMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGRealSparseMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGComplexSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGRealDiagonalMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealScalar and OGComplexDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGRealScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGComplexScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGRealMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGComplexMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGRealSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGComplexSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGRealDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexScalar and OGComplexDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGRealScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGComplexScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGRealMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGComplexMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGRealSparseMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGComplexSparseMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGRealDiagonalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGIntegerScalar and OGComplexDiagonalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGRealScalar
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,arg0,conv1);

         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGComplexScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGComplexMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGRealSparseMatrix
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,arg0,conv1);

         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGComplexSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGRealDiagonalMatrix
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,arg0,conv1);

         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealMatrix and OGComplexDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGRealScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGComplexScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGRealMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGComplexMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGRealSparseMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGComplexSparseMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGRealDiagonalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGLogicalMatrix and OGComplexDiagonalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGRealScalar
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,arg0,conv1);
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGComplexScalar
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,arg0,conv1);
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGRealMatrix
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,arg0,conv1);
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGRealSparseMatrix
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,arg0,conv1);
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGComplexSparseMatrix
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,arg0,conv1);
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGRealDiagonalMatrix
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,arg0,conv1);
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexMatrix and OGComplexDiagonalMatrix
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,arg0,conv1);
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGRealScalar
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGComplexScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGRealMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGComplexMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGRealSparseMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGComplexSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGRealDiagonalMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealSparseMatrix and OGComplexDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGRealScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGComplexScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGRealMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGComplexMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGRealSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGComplexSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGRealDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexSparseMatrix and OGComplexDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGRealScalar
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGComplexScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGRealMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGComplexMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGRealSparseMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGComplexSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGRealDiagonalMatrix
         OGRealMatrix * conv0 = this->getConvertTo()->convertToOGRealMatrix(arg0);
         OGRealMatrix * conv1 = this->getConvertTo()->convertToOGRealMatrix(arg1);
         T ret = run(reg0,conv0,conv1);

         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGRealDiagonalMatrix and OGComplexDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGRealScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGComplexScalar
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGIntegerScalar
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGRealMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGLogicalMatrix
   return nullptr;

   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGComplexMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         T ret = run(reg0,conv0,arg1);
         delete conv0;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGRealSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGComplexSparseMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGRealDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };


   virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const
   {
      // impl convert and run for types OGComplexDiagonalMatrix and OGComplexDiagonalMatrix
         OGComplexMatrix * conv0 = this->getConvertTo()->convertToOGComplexMatrix(arg0);
         OGComplexMatrix * conv1 = this->getConvertTo()->convertToOGComplexMatrix(arg1);
         T ret = run(reg0,conv0,conv1);
         delete conv0;
         delete conv1;

         return ret;
   };



// Required backstop impls
virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const * arg0, OGComplexMatrix const * arg1) const = 0;

virtual T run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const * arg0, OGRealMatrix const * arg1) const = 0;

};

// typedef the void dispatch of a binary operation
typedef DispatchBinaryOp<void *> DispatchVoidBinaryOp;

// typedef the void dispatch of a unary operation
typedef DispatchUnaryOp<void *> DispatchVoidUnaryOp;

} // namespace librdag

#endif // _DISPATCH_HH

