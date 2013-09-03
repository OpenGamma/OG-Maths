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
  cout << "in OGNumeric destructor" << endl;
}

void OGNumeric::debug_print()
{
  cout << "Abstract OGNumeric type" << endl;
}

/*
 * OGExpr
 */

OGExpr::OGExpr() {}

OGExpr::OGExpr(OGExpr& copy)
{
  this->_args = new std::vector<OGNumeric *>(*copy.getArgs());
}

OGExpr::OGExpr(const librdag::OGNumeric * const args, const int nargs)
{
	this->_args = new std::vector<OGNumeric *>;
	for(int i = 0; i < nargs; i++)
	{
		this->_args->push_back(const_cast<OGNumeric *> (&args[i]));
	}
}

OGExpr::~OGExpr()
{
	for (std::vector<OGNumeric *>::iterator it = this->_args->begin() ; it != this->_args->end(); it++)
	{
		delete &it;
	}
	delete this->_args;
}

OGExpr&
OGExpr::operator=(OGExpr& rhs)
{
  rhs.setArgs(this->getArgs());
  return *this;
}

std::vector<OGNumeric *> *
OGExpr::getArgs()
{
	return this->_args;
}

// FIXME: Should replace this with construct from a vector
void
OGExpr::setArgs(std::vector<OGNumeric *> * args)
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

OGBinaryExpr::OGBinaryExpr() {}

OGBinaryExpr::OGBinaryExpr(OGNumeric* left, OGNumeric* right)
{
	vector<OGNumeric*> *args = new vector<OGNumeric*>();
	args->push_back(left);
	args->push_back(right);
	this->setArgs(args);
}

void
COPY::debug_print()
{
	cout << "COPY base class" << endl;
}

PLUS::PLUS() {}

PLUS::PLUS(OGNumeric* left, OGNumeric* right) : OGBinaryExpr(left, right) {}

void
PLUS::debug_print()
{
	cout << "PLUS base class" << endl;
}

MINUS::MINUS() {}

MINUS::MINUS(OGNumeric* left, OGNumeric* right) : OGBinaryExpr(left, right) {}

void
MINUS::debug_print()
{
	cout << "MINUS base class" << endl;
}

SVD::SVD() {}

void
SVD::debug_print()
{
	cout << "SVD base class" << endl;
}

SELECTRESULT::SELECTRESULT() {}

void
SELECTRESULT::debug_print()
{
	printf("SELECTRESULT base class\n");
}

}
