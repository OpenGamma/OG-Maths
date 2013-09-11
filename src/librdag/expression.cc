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

OGNumeric::OGNumeric() {}

OGNumeric::~OGNumeric()
{
}

void OGNumeric::debug_print()
{
  cout << "Abstract OGNumeric type" << endl;
}

const OGRealScalar* OGNumeric::asOGRealScalar() const
{
  return nullptr;
}

const OGComplexScalar* OGNumeric::asOGComplexScalar() const
{
  return nullptr;
}

const OGIntegerScalar* OGNumeric::asOGIntegerScalar() const
{
  return nullptr;
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

ArgContainer*
OGExpr::getArgs()
{
  return this->_args;
}

size_t
OGExpr::getNArgs()
{
  return this->_args->size();
}

void
OGExpr::debug_print()
{
	cout << "OGExpr base class" << endl;
}

void
OGExpr::accept(Visitor &v)
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

void
COPY::debug_print()
{
	cout << "COPY base class" << endl;
}

PLUS::PLUS(ArgContainer* args): OGBinaryExpr(args) {}

void
PLUS::debug_print()
{
	cout << "PLUS base class" << endl;
}

MINUS::MINUS(ArgContainer* args): OGBinaryExpr(args) {
}

void
MINUS::debug_print()
{
	cout << "MINUS base class" << endl;
}

SVD::SVD(ArgContainer* args): OGUnaryExpr(args) {}

void
SVD::debug_print()
{
	cout << "SVD base class" << endl;
}

SELECTRESULT::SELECTRESULT(ArgContainer* args): OGBinaryExpr(args) {
  // Check that the second argument is an integer
  const OGIntegerScalar* i = (*args)[1]->asOGIntegerScalar();
  if (i == nullptr)
  {
    // FIXME: Throw exception when exceptions set up. die for now.
    exit(1);
  }
}

void
SELECTRESULT::debug_print()
{
	printf("SELECTRESULT base class\n");
}

} // namespace librdag
