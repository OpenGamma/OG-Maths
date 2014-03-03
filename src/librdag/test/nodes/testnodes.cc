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
template<typename T> CheckNode<T>::CheckNode(OGNumeric * expected, CompareMethod comparisonMethod)
{
  _expected = expected;
  _comparisonMethod = comparisonMethod;
}

template<typename T> CheckNode<T>::~CheckNode()
{
  delete this->_expected;
}

template<typename T> T *
CheckNode<T>::getNode() const
{
  return this->_node;
}

template<typename T> const OGNumeric *
CheckNode<T>::getExpected() const
{
  return this->_expected;
}

template<typename T> bool CheckNode<T>::resultCorrect() const
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

template<typename T> CompareMethod
CheckNode<T>::getComparisonMethod() const
{
    return this->_comparisonMethod;
}


// CheckUnary impls
template<typename T> CheckUnary<T>::CheckUnary(OGNumeric * input, OGNumeric * expected, CompareMethod comparisonMethod):CheckNode<T>(expected, comparisonMethod)
{
  _input = input;
}

template<typename T> CheckUnary<T>::~CheckUnary()
{
  delete this->_resultPair->first; // the first element is a copy of the "answer"
  delete this->_resultPair;
}

template<typename T> ResultPair *
CheckUnary<T>::getResultPair() const
{
  return this->_resultPair;
}

template<typename T> void
CheckUnary<T>::execute()
{
  ArgContainer * args = new ArgContainer();
  args->push_back(_input);
  T * node = new T(args);
  ExecutionList * el1 = new ExecutionList(node);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    Dispatcher * v = new Dispatcher();
    v->dispatch(*it);
    delete v;
  }
  const RegContainer * regs = node->getRegs();
  const OGNumeric * answer = (*regs)[0];
  // we take a copy of answer so node can be deleted
  this->_resultPair = new ResultPair(answer->copy(), this->getExpected());
  delete el1;
  delete node;
}

template<typename T> bool
CheckUnary<T>::comparesCorrectlyTypeInvariant() const
{
  return (*(this->getResultPair()->first->asOGTerminal())%*(this->getResultPair()->second->asOGTerminal()));
}

template<typename T> bool
CheckUnary<T>::comparesCorrectly() const
{
  return (*(this->getResultPair()->first->asOGTerminal())==~*(this->getResultPair()->second->asOGTerminal()));
}

template<typename T> void
CheckUnary<T>::debug_print()
{
  _input->debug_print();
}

// UnaryOpTest impls
template<typename T> UnaryOpTest<T>::~UnaryOpTest()
{
  delete _checker;
}

template<typename T> void
UnaryOpTest<T>::SetUp()
{
  _checker = this->::testing::TestWithParam<CheckUnary<T> *>::GetParam();
  _checker->execute();
}

template<typename T> void
UnaryOpTest<T>::TearDown()
{
    delete _checker;
    _checker = nullptr;
}

template class CheckUnary<NORM2>;
template class UnaryOpTest<NORM2>;

}