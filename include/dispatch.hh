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
#include "register.hh"
#include "warningmacros.h"
#include "jvmmanager.hh"



namespace librdag {

// fwd decl
class PlusRunner;
  
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
  
};


/**
 * For dispatching operations of the form "T foo(Register * register1, OGTerminal * arg0, OGTerminal * arg1)"
 */
template<typename T> class  DispatchBinaryOp
{
public:
virtual ~DispatchBinaryOp(){};
 
// will run the operation
T run(Register const SUPPRESS_UNUSED * register1, OGTerminal const SUPPRESS_UNUSED * arg0, OGTerminal const SUPPRESS_UNUSED * arg1) const
{
  
  // massive switch table
  
};

// specific methods
//  MAT="OGRealScalar OGComplexScalar OGIntegerScalar OGRealMatrix OGLogicalMatrix OGComplexMatrix OGRealDiagonalMatrix OGComplexDiagonalMatrix OGRealSparseMatrix OGComplexSparseMatrix"; for i in $MAT; do for j in $MAT; do printf "virtual T run(Register const SUPPRESS_UNUSED * reg0, ${i} const SUPPRESS_UNUSED * arg0, ${j} const SUPPRESS_UNUSED * arg1) const \n{\n  // impl convert and run for types ${i} and ${j} \n};\n\n";done;done
virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealScalar and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexScalar and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGIntegerScalar const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGIntegerScalar and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealMatrix and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGLogicalMatrix and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexMatrix and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealDiagonalMatrix and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexDiagonalMatrix and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGRealSparseMatrix and OGComplexSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGIntegerScalar const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGIntegerScalar 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGLogicalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGLogicalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexDiagonalMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexDiagonalMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGRealSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGRealSparseMatrix 
};

virtual T run(Register const SUPPRESS_UNUSED * reg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg0, OGComplexSparseMatrix const SUPPRESS_UNUSED * arg1) const 
{
  // impl convert and run for types OGComplexSparseMatrix and OGComplexSparseMatrix 
};



virtual T run(Register const * reg0, OGComplexMatrix const * arg0, OGComplexMatrix const * arg1) const = 0;

virtual T run(Register const * reg0, OGRealMatrix const * arg0, OGRealMatrix const * arg1) const = 0;

};

template class DispatchBinaryOp<void>;

// typedef the void dispatch of a binary operation
typedef DispatchBinaryOp<void> DispatchVoidBinaryOp;


class PlusRunner: public DispatchVoidBinaryOp, Uncopyable
{
  virtual void run(Register const SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const override;
  virtual void run(Register const SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const override;
};

} // namespace librdag

namespace convert {

using namespace librdag;

jobjectArray convertCreal16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, real16 ** inputData, int rows, int cols);
jobjectArray extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols);
jobjectArray extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols);

class DispatchToReal16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToReal16ArrayOfArrays();
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED *thing);
    void setData(real16 ** data);
    void setRows(int rows);
    void setCols(int cols);
    real16 ** getData();
    int getRows();
    int getCols();
  private:
    real16 ** _data = nullptr;
    int rows;
    int cols;
};



class DispatchToComplex16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToComplex16ArrayOfArrays();
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED *thing);
    void setData(complex16 ** data);
    void setRows(int rows);
    void setCols(int cols);
    complex16 ** getData();
    int getRows();
    int getCols();
  private:
    complex16 ** _data = nullptr;
    int rows;
    int cols;
};

class DispatchToOGTerminal: public librdag::Visitor
{
  public:
    DispatchToOGTerminal(JNIEnv* env);
    virtual ~DispatchToOGTerminal();
    virtual void visit(librdag::OGExpr const *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const *thing);
    virtual void visit(librdag::OGScalar<int> const *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const *thing);
    jobject getObject();
  private:
    void setObject(jobject obj);
    jobject _obj = nullptr;
    JNIEnv* _env;
};


} // namespace convert

#endif // _DISPATCH_HH
