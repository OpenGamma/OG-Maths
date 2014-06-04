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

using namespace std;
using namespace librdag;

namespace testnodes {

// Check node impl
template<typename T> CheckNode<T>::CheckNode(OGNumeric::Ptr expected, CompareMethod comparisonMethod)
{
  _expected = expected;
  _comparisonMethod = comparisonMethod;
}

template<typename T>
CheckNode<T>::~CheckNode() {}

template<typename T>
OGNumeric::Ptr
CheckNode<T>::getExpected() const
{
  return _expected;
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
template<typename T> CheckUnary<T>::CheckUnary(OGNumeric::Ptr input, OGNumeric::Ptr expected, CompareMethod comparisonMethod):CheckNode<T>(expected, comparisonMethod)
{
  _input = input;
}

template<typename T> CheckUnary<T>::~CheckUnary()
{
  delete _resultPair;
}

template<typename T> ResultPair *
CheckUnary<T>::getResultPair() const
{
  return _resultPair;
}

template<typename T> void
CheckUnary<T>::execute()
{
  OGNumeric::Ptr node = T::create(_input);
  ExecutionList el = ExecutionList{node};
  Dispatcher v;
  for (auto it = el.begin(); it != el.end(); ++it)
  {
    v.dispatch(*it);
  }
  const RegContainer& regs = node->asOGExpr()->getRegs();
  OGNumeric::Ptr answer = regs[0];
  _resultPair = new ResultPair(answer, this->getExpected());
}

template<typename T> bool
CheckUnary<T>::comparesCorrectlyTypeInvariant() const
{
  return (*(this->getResultPair()->first->asOGTerminal())%(this->getResultPair()->second->asOGTerminal()));
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
template<typename T> CheckBinary<T>::CheckBinary(OGNumeric::Ptr first_input, OGNumeric::Ptr second_input, OGNumeric::Ptr expected, CompareMethod comparisonMethod):CheckNode<T>(expected, comparisonMethod)
{
  _first_input = first_input;
  _second_input = second_input;
}

template<typename T> CheckBinary<T>::~CheckBinary()
{
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
  OGNumeric::Ptr node = T::create(_first_input, _second_input);
  ExecutionList el = ExecutionList{node};
  Dispatcher v;
  for (auto it = el.begin(); it != el.end(); ++it)
  {
    v.dispatch(*it);
  }
  const RegContainer& regs = node->asOGExpr()->getRegs();
  OGNumeric::Ptr answer = regs[0];
  _resultPair = new ResultPair(answer, this->getExpected());
}

template<typename T> bool
CheckBinary<T>::comparesCorrectlyTypeInvariant() const
{
  return (*(this->getResultPair()->first->asOGTerminal())%(this->getResultPair()->second->asOGTerminal()));
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

template class CheckUnary<CTRANSPOSE>;
template class UnaryOpTest<CTRANSPOSE>;

template class CheckUnary<NORM2>;
template class UnaryOpTest<NORM2>;

template class CheckUnary<PINV>;
template class UnaryOpTest<PINV>;

template class CheckUnary<INV>;
template class UnaryOpTest<INV>;

template class CheckUnary<TRANSPOSE>;
template class UnaryOpTest<TRANSPOSE>;

template class CheckBinary<MTIMES>;
template class BinaryOpTest<MTIMES>;

}
