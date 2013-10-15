/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <iostream>
#include "expression.hh"
#include "terminal.hh"
#include "exceptions.hh"
#include "exprtypeenum.h"

using namespace std;

namespace librdag
{

/*
 * OGExpr
 */

OGExpr::OGExpr(ArgContainer *args)
{
  if (args == nullptr)
  {
    throw rdag_error("Null args passed to Expr constructor");
  }
  this->_args = args;
  this->_regs = new RegContainer();
}

OGExpr::~OGExpr()
{
  delete this->_args;
  delete this->_regs;
}

const ArgContainer*
OGExpr::getArgs() const
{
  return this->_args;
}

size_t
OGExpr::getNArgs() const
{
  return this->_args->size();
}

void
OGExpr::accept(Visitor &v) const
{
  v.visit(this);
}

const OGExpr*
OGExpr::asOGExpr() const
{
  return this;
}

const RegContainer * 
OGExpr::getRegs() const
{
  return this->_regs;
}

void OGExpr::debug_print() const
{
  cout << "OGExpr::debug_print()" << std::endl;
}


/**
 * Things that extend OGExpr
 */

OGUnaryExpr::OGUnaryExpr(ArgContainer* args): OGExpr(args)
{
  if (args->size() != 1)
  {
    throw rdag_error("ArgContainer passed to UnaryExpr does not have exactly one argument");
  }
}

OGBinaryExpr::OGBinaryExpr(ArgContainer* args): OGExpr(args)
{
  if (args->size() != 2)
  {
    throw rdag_error("ArgContainer passed to BinaryExpr does not have exactly two arguments");
  }
}

/**
 * COPY node
 */
COPY::COPY(ArgContainer* args): OGUnaryExpr(args) {}

OGNumeric*
COPY::copy() const
{
  return new COPY(this->getArgs()->copy());
}

const COPY*
COPY::asCOPY() const
{
  return this;
}

void
COPY::debug_print() const
{
	cout << "COPY base class" << endl;
}

ExprType_t
COPY::getType() const
{
  return COPY_ENUM;
}

/**
 * PLUS node
 */
PLUS::PLUS(ArgContainer* args): OGBinaryExpr(args) {}

OGNumeric*
PLUS::copy() const
{
  return new PLUS(this->getArgs()->copy());
}

const PLUS*
PLUS::asPLUS() const
{
  return this;
}

void
PLUS::debug_print() const
{
	cout << "PLUS base class" << endl;
}

ExprType_t
PLUS::getType() const
{
  return PLUS_ENUM;
}

/**
 * NEGATE node
 */
NEGATE::NEGATE(ArgContainer* args): OGUnaryExpr(args) {
}

OGNumeric*
NEGATE::copy() const
{
  return new NEGATE(this->getArgs()->copy());
}

const NEGATE*
NEGATE::asNEGATE() const
{
  return this;
}

void
NEGATE::debug_print() const
{
  cout << "NEGATE base class" << endl;
}

ExprType_t
NEGATE::getType() const
{
  return NEGATE_ENUM;
}

/**
 * SVD node
 */
SVD::SVD(ArgContainer* args): OGUnaryExpr(args) {}

OGNumeric*
SVD::copy() const
{
  return new SVD(this->getArgs()->copy());
}

const SVD*
SVD::asSVD() const
{
  return this;
}

void
SVD::debug_print() const
{
	cout << "SVD base class" << endl;
}

ExprType_t
SVD::getType() const
{
  return SVD_ENUM;
}

/**
 * SELECTRESULT node
 */
SELECTRESULT::SELECTRESULT(ArgContainer* args): OGBinaryExpr(args) {
  // Check that the second argument is an integer
  const OGIntegerScalar* i = (*args)[1]->asOGIntegerScalar();
  if (i == nullptr)
  {
    throw rdag_error("Second argument of SelectResult is not an integer");
  }
}

OGNumeric*
SELECTRESULT::copy() const
{
  return new SELECTRESULT(this->getArgs()->copy());
}

const SELECTRESULT*
SELECTRESULT::asSELECTRESULT() const
{
  return this;
}

void
SELECTRESULT::debug_print() const
{
	printf("SELECTRESULT base class\n");
}

ExprType_t
SELECTRESULT::getType() const
{
  return SELECTRESULT_ENUM;
}

} // namespace librdag
