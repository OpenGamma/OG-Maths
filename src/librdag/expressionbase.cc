/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <iostream>
#include "expressionbase.hh"
#include "terminal.hh"
#include "exceptions.hh"

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

} // namespace librdag
