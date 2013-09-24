/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <iostream>
#include "expression.hh"
#include "visitor.hh"

using namespace std;

namespace librdag
{

/*
 * OGNumeric
 */

OGNumeric::~OGNumeric()
{
}

void
OGNumeric::debug_print() const
{
  cout << "Abstract OGNumeric type" << endl;
}

const COPY*
OGNumeric::asCOPY() const
{
  return nullptr;
}

const PLUS*
OGNumeric::asPLUS() const
{
  return nullptr;
}

const NEGATE*
OGNumeric::asNEGATE() const
{
  return nullptr;
}

const SVD*
OGNumeric::asSVD() const
{
  return nullptr;
}

const SELECTRESULT*
OGNumeric::asSELECTRESULT() const
{
  return nullptr;
}

const OGTerminal*
OGNumeric::asOGTerminal() const
{
  return nullptr;
}

const OGRealScalar*
OGNumeric::asOGRealScalar() const
{
  return nullptr;
}

const OGComplexScalar*
OGNumeric::asOGComplexScalar() const
{
  return nullptr;
}

const OGIntegerScalar*
OGNumeric::asOGIntegerScalar() const
{
  return nullptr;
}

const OGRealMatrix*
OGNumeric::asOGRealMatrix() const
{
  return nullptr;
}

const OGComplexMatrix*
OGNumeric::asOGComplexMatrix() const
{
  return nullptr;
}

const OGRealDiagonalMatrix*
OGNumeric::asOGRealDiagonalMatrix() const
{
  return nullptr;
}

const OGComplexDiagonalMatrix*
OGNumeric::asOGComplexDiagonalMatrix() const
{
  return nullptr;
}

const OGRealSparseMatrix*
OGNumeric::asOGRealSparseMatrix() const
{
  return nullptr;
}

const OGComplexSparseMatrix*
OGNumeric::asOGComplexSparseMatrix() const
{
  return nullptr;
}

/*
 * OGTerminal
 */

const OGTerminal*
OGTerminal::asOGTerminal() const
{
  return this;
}

/*
 * OGExpr
 */

OGExpr::OGExpr(ArgContainer *args)
{
  if (args == nullptr)
  {
    throw new librdagException();
  }
  this->_args = args;
}

OGExpr::~OGExpr()
{
  delete this->_args;
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
OGExpr::debug_print() const
{
	cout << "OGExpr base class" << endl;
}

void
OGExpr::accept(Visitor &v) const
{
  v.visit(this);
}

/**
 * Things that extend OGExpr
 */

OGUnaryExpr::OGUnaryExpr(ArgContainer* args): OGExpr(args)
{
  if (args->size() != 1)
  {
    throw new librdagException();
  }
}

OGBinaryExpr::OGBinaryExpr(ArgContainer* args): OGExpr(args)
{
  if (args->size() != 2)
  {
    throw new librdagException();
  }
}

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

SELECTRESULT::SELECTRESULT(ArgContainer* args): OGBinaryExpr(args) {
  // Check that the second argument is an integer
  const OGIntegerScalar* i = (*args)[1]->asOGIntegerScalar();
  if (i == nullptr)
  {
    throw new librdagException();
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

} // namespace librdag
