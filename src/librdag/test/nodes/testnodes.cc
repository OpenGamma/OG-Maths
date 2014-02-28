/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */


#include "gtest/gtest.h"
#include "testnodes.hh"
#include "expressionbase.hh"
#include "execution.hh"
#include "dispatch.hh"
#include "containers.hh"

using namespace std;
using namespace librdag;

namespace testnodes {

// Check node impl
CheckNode::CheckNode(OGExpr * node, OGNumeric * expected, CompareMethod comparisonMethod)
{
  _node = node;
  _expected = expected;
  _comparisonMethod = comparisonMethod;
}

CheckNode::~CheckNode()
{
  delete this->_node;
  delete this->_expected;
}

OGExpr *
CheckNode::getNode() const
{
  return this->_node;
}

const OGNumeric *
CheckNode::getExpected() const
{
  return this->_expected;
}

bool CheckNode::resultCorrect() const
{
  if(this->getComparisonMethod() == MATHSEQUAL)
  {
    return this->comparesCorrectlyTypeInvariant();
  }
  else
  {
    return this->comparesCorrectly();
  }
}

CompareMethod
CheckNode::getComparisonMethod() const
{
    return this->_comparisonMethod;
}


// CheckUnary impls
CheckUnary::CheckUnary(OGExpr * node, OGNumeric * input, OGNumeric * expected, CompareMethod comparisonMethod):CheckNode(node, expected, comparisonMethod)
{
  _input = input;
}

CheckUnary::~CheckUnary()
{
  delete this->_resultPair;
}

ResultPair *
CheckUnary::getResultPair() const
{
  return this->_resultPair;
}



void
CheckUnary::execute()
{
  ArgContainer * args = new ArgContainer();
  args->push_back(_input);
  OGExpr * node = this->getNode();
  node->setArgContainer(args);
  RegContainer * regs = new RegContainer();
  node->setRegContainer(regs);
  ExecutionList * el1 = new ExecutionList(node);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    Dispatcher * v = new Dispatcher();
    v->dispatch(*it);
    delete v;
  }
  const OGNumeric * answer = (*regs)[0];
  this->_resultPair = new ResultPair(answer, this->getExpected());
  // regs and args cleaned up by CheckUnary base class dtor
  delete el1;
}

bool
CheckUnary::comparesCorrectlyTypeInvariant() const
{
  return (*(this->getResultPair()->first->asOGTerminal())%*(this->getResultPair()->second->asOGTerminal()));
}

bool
CheckUnary::comparesCorrectly() const
{
  return (*(this->getResultPair()->first->asOGTerminal())==~*(this->getResultPair()->second->asOGTerminal()));
}

// UnaryOpTest impls
UnaryOpTest::~UnaryOpTest()
{
  delete _checker;
}

void
UnaryOpTest::SetUp()
{
  _checker =  GetParam();
  _checker->execute();
}

void UnaryOpTest::TearDown()
{
    delete _checker;
    _checker = nullptr;
}

TEST_P(UnaryOpTest, SimpleAssertResultTrue) {
  CheckUnary * impl = GetParam();
  ASSERT_TRUE(impl->resultCorrect());
}

}