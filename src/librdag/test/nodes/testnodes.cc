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
  delete this->_resultPair->first; // this is the owning copy
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
  T * node = new T(_input);
  ExecutionList * el1 = new ExecutionList(node);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    Dispatcher * v = new Dispatcher();
    v->dispatch(*it);
    delete v;
  }
  const RegContainer * regs = node->getRegs();
  const OGNumeric * answer = (*regs)[0];
  this->_resultPair = new ResultPair(answer->asOGTerminal()->createOwningCopy(), this->getExpected());
  delete node;
  delete el1;
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
template<typename T> UnaryOpTest<T>::UnaryOpTest(): ::testing::TestWithParam<CheckUnary<T> *>::TestWithParam() {}

template<typename T> UnaryOpTest<T>::~UnaryOpTest() {}

template<typename T> void
UnaryOpTest<T>::SetUp() {}

template<typename T> void
UnaryOpTest<T>::TearDown() {}

// CheckBinary impls
template<typename T> CheckBinary<T>::CheckBinary(OGNumeric * first_input, OGNumeric * second_input, OGNumeric * expected, CompareMethod comparisonMethod):CheckNode<T>(expected, comparisonMethod)
{
  _first_input = first_input;
  _second_input = second_input;
}

template<typename T> CheckBinary<T>::~CheckBinary()
{
  delete this->_resultPair->first; // this is the owning copy
  delete this->_resultPair;
}

template<typename T> ResultPair *
CheckBinary<T>::getResultPair() const
{
  return this->_resultPair;
}

template<typename T> void
CheckBinary<T>::execute()
{
  T * node = new T(_first_input, _second_input);
  ExecutionList * el1 = new ExecutionList(node);
  for (auto it = el1->begin(); it != el1->end(); ++it)
  {
    Dispatcher * v = new Dispatcher();
    v->dispatch(*it);
    delete v;
  }
  const RegContainer * regs = node->getRegs();
  const OGNumeric * answer = (*regs)[0];
  this->_resultPair = new ResultPair(answer->asOGTerminal()->createOwningCopy(), this->getExpected());
  delete node;
  delete el1;
}

template<typename T> bool
CheckBinary<T>::comparesCorrectlyTypeInvariant() const
{
  return (*(this->getResultPair()->first->asOGTerminal())%*(this->getResultPair()->second->asOGTerminal()));
}

template<typename T> bool
CheckBinary<T>::comparesCorrectly() const
{
  return (*(this->getResultPair()->first->asOGTerminal())==~*(this->getResultPair()->second->asOGTerminal()));
}

template<typename T> void
CheckBinary<T>::debug_print()
{
  _first_input->debug_print();
  _second_input->debug_print();
}

// BinaryOpTest impls

template<typename T> BinaryOpTest<T>::BinaryOpTest(): ::testing::TestWithParam<CheckBinary<T> *>::TestWithParam() {}

template<typename T> BinaryOpTest<T>::~BinaryOpTest() {}

template<typename T> void
BinaryOpTest<T>::SetUp() {}

template<typename T> void
BinaryOpTest<T>::TearDown() {}

template class CheckUnary<NORM2>;
template class UnaryOpTest<NORM2>;

template class CheckBinary<MTIMES>;
template class BinaryOpTest<MTIMES>;

}
