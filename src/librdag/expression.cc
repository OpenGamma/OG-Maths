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

/*
 * OGExpr
 */

OGExpr::OGExpr()
{
  this->_args = NULL;
}

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

OGExpr::OGExpr(std::vector<OGNumeric *> *args)
{
  this->_args = args;
}

OGExpr::~OGExpr()
{
  for (std::vector<OGNumeric *>::iterator it = this->_args->begin() ; it != this->_args->end(); it++)
  {
    delete *it;
  }
  delete this->_args;
}

//OGExpr&
//OGExpr::operator=(OGExpr& rhs)
//{
//  rhs.setArgs(this->getArgs());
//  return *this;
//}

std::vector<OGNumeric *> *
OGExpr::getArgs()
{
	return this->_args;
}

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

OGUnaryExpr::OGUnaryExpr(): OGExpr()
{
}

OGUnaryExpr::OGUnaryExpr(std::vector<OGNumeric*>* args)
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
  vector<OGNumeric*> *args = new vector<OGNumeric*>();
  args->push_back(arg);
  this->setArgs(args);
}

OGBinaryExpr::OGBinaryExpr() {}

OGBinaryExpr::OGBinaryExpr(std::vector<OGNumeric*>* args) : OGExpr() {
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
	vector<OGNumeric*> *args = new vector<OGNumeric*>();
	args->push_back(left);
	args->push_back(right);
	this->setArgs(args);
}

COPY::COPY() {}

COPY::COPY(OGNumeric *arg)
{
  vector<OGNumeric*> *args = new vector<OGNumeric*>();
  args->push_back(arg);
  this->setArgs(args);
}

COPY::COPY(std::vector<OGNumeric*>* args): OGUnaryExpr(args) {

}

void
COPY::debug_print()
{
	cout << "COPY base class" << endl;
}

PLUS::PLUS() {}

PLUS::PLUS(OGNumeric* left, OGNumeric* right) : OGBinaryExpr(left, right) {}

PLUS::PLUS(std::vector<OGNumeric*>* args): OGBinaryExpr(args) {}

void
PLUS::debug_print()
{
	cout << "PLUS base class" << endl;
}

MINUS::MINUS() {}

MINUS::MINUS(OGNumeric* left, OGNumeric* right) : OGBinaryExpr(left, right) {}

MINUS::MINUS(std::vector<OGNumeric*>* args): OGBinaryExpr(args) {
}

void
MINUS::debug_print()
{
	cout << "MINUS base class" << endl;
}

SVD::SVD() {}

SVD::SVD(std::vector<OGNumeric*>* args): OGUnaryExpr(args) {
}

void
SVD::debug_print()
{
	cout << "SVD base class" << endl;
}

SELECTRESULT::SELECTRESULT() {}

SELECTRESULT::SELECTRESULT(std::vector<OGNumeric*>* args): OGUnaryExpr(args) {
}

void
SELECTRESULT::debug_print()
{
	printf("SELECTRESULT base class\n");
}

}
