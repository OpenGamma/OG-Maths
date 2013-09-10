/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
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

OGExpr::OGExpr()
{
  this->_args = nullptr;
}

OGExpr::OGExpr(OGExpr& copy)
{
  this->_args = new ArgContainer(*copy.getArgs());
}

OGExpr::OGExpr(ArgContainer *args)
{
  this->_args = args;
}

OGExpr::~OGExpr()
{
  delete this->_args;
}

ArgContainer *
OGExpr::getArgs()
{
	return this->_args;
}

void
OGExpr::setArgs(ArgContainer * args)
{
	this->_args = args;
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

OGUnaryExpr::OGUnaryExpr() : OGExpr() {}

OGUnaryExpr::OGUnaryExpr(ArgContainer* args)
{
  if (args->size() != 1)
  {
    //FIXME: Replace with exception when implemented.
    // For now just die
    exit(1);
  }
  this->setArgs(args);
}

OGUnaryExpr::OGUnaryExpr(OGNumeric* arg)
{
  ArgContainer* args = new ArgContainer();
  args->push_back(arg);
  this->setArgs(args);
}

OGBinaryExpr::OGBinaryExpr() : OGExpr() {}

OGBinaryExpr::OGBinaryExpr(ArgContainer* args) {
  if (args->size() != 2)
  {
    //FIXME: Replace with exception when implemented.
    // For now just die
    exit(1);
  }
  this->setArgs(args);
}

OGBinaryExpr::OGBinaryExpr(OGNumeric* left, OGNumeric* right)
{
    ArgContainer* args = new ArgContainer();
    args->push_back(left);
    args->push_back(right);
    this->setArgs(args);
}

COPY::COPY() : OGUnaryExpr() {}

COPY::COPY(OGNumeric *arg) : OGUnaryExpr(arg) {}

COPY::COPY(ArgContainer* args): OGUnaryExpr(args) {}

void
COPY::debug_print()
{
	cout << "COPY base class" << endl;
}

PLUS::PLUS() : OGBinaryExpr() {}

PLUS::PLUS(OGNumeric* left, OGNumeric* right) : OGBinaryExpr(left, right) {}

PLUS::PLUS(ArgContainer* args): OGBinaryExpr(args) {}

void
PLUS::debug_print()
{
	cout << "PLUS base class" << endl;
}

MINUS::MINUS() : OGBinaryExpr() {}

MINUS::MINUS(OGNumeric* left, OGNumeric* right) : OGBinaryExpr(left, right) {}

MINUS::MINUS(ArgContainer* args): OGBinaryExpr(args) {
}

void
MINUS::debug_print()
{
	cout << "MINUS base class" << endl;
}

SVD::SVD() : OGUnaryExpr() {}

SVD::SVD(ArgContainer* args): OGUnaryExpr(args) {}

SVD::SVD(OGNumeric* arg): OGUnaryExpr(arg) {}

void
SVD::debug_print()
{
	cout << "SVD base class" << endl;
}

SELECTRESULT::SELECTRESULT() : OGBinaryExpr() {}

SELECTRESULT::SELECTRESULT(ArgContainer* args): OGBinaryExpr(args) {
  // Check that the second argument is an integer
  const OGIntegerScalar* i = (*args)[1]->asOGIntegerScalar();
  if (i == nullptr)
  {
    // FIXME: Throw exception when exceptions set up. die for now.
    exit(1);
  }
}

SELECTRESULT::SELECTRESULT(OGNumeric* result, OGNumeric* index): OGBinaryExpr(result, index)
{
  const OGIntegerScalar* i = (*(this->getArgs()))[1]->asOGIntegerScalar();
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
