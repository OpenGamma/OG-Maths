/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _TESTNODES_HH
#define _TESTNODES_HH

#include "terminal.hh"
#include "expressionbase.hh"
#include "execution.hh"
#include "dispatch.hh"
#include "containers.hh"

using namespace std;
using namespace librdag;

namespace testnodes {

typedef pair<const OGNumeric* , const OGNumeric* > ResultPair;

/**
 * Indicate the method used for comparison
 */
enum CompareMethod
{
  /**
   * Compare ensuring the types and the values are the same.
   */
  TYPEEQUAL,
  /**
   * Compare ensuring the values are the same (type invariant).
   */
  MATHSEQUAL
};

/**
 * Class used to compare node results, the class is just a view on memory.
 * @param T the node to be checked.
 */
template <typename T> class CheckNode
{
  public:
      /**
       * Construct with test node.
       * @param expected the expected result.
       * @param comparisonMethod how the results should be compared.
       */
      CheckNode(OGNumeric * expected, CompareMethod comparisonMethod);
      /**
       * Standard destructor
       */
      virtual ~CheckNode();
      /**
       * Checks if the result of the node execution compares with the expected
       * value correctly (type invariant, so result conversion is allowed).
       */
      virtual bool comparesCorrectlyTypeInvariant() const = 0;
      /**
       * Checks if the result of the node execution compares with the expected
       * value correctly.
       */
      virtual bool comparesCorrectly() const = 0;
      /**
       * Checks if the result is correct using the method as specified in \a comparisonMethod.
       */
      virtual bool resultCorrect() const;
      /**
       * Gets the node.
       */
      virtual T * getNode() const;
      /**
       * Gets the expected value.
       */
      virtual const OGNumeric * getExpected() const;
      /**
       * Gets the comparison method.
       */
      virtual CompareMethod getComparisonMethod() const;

  protected:
    /**
     * Method called to do the actual execution of the operation.
     */
    virtual void execute() = 0;
  private:
    T * _node;
    const OGNumeric * _expected;
    CompareMethod _comparisonMethod;
};

/**
 * Class used for checking unary nodes execute as expected.
 * @param T the node to be checked.
 */
template <typename T> class CheckUnary: public CheckNode<T>
{
  public:
    /**
     * Construct a unary node check.
     * @param input the input value.
     * @param expected the expected value.
     * @param comparisonMethod how the results should be compared.
     */
    CheckUnary(OGNumeric * input, OGNumeric * expected, CompareMethod comparisonMethod);
    ~CheckUnary();
    virtual bool comparesCorrectlyTypeInvariant() const override;
    virtual bool comparesCorrectly() const override;

    /**
     * Get the ResultPair
     */
    ResultPair * getResultPair() const;
    virtual void execute() override;

    virtual void debug_print();

  private:
    ResultPair * _resultPair;
    OGNumeric * _input;
};

/**
 * Unary Operation test class
 */
template <typename T> class UnaryOpTest : public ::testing::TestWithParam<CheckUnary<T> *>
{
 public:
  virtual ~UnaryOpTest();
  virtual void SetUp();
  virtual void TearDown();
 protected:
  CheckUnary<T> * _checker;
};

}

#endif // _TESTNODES_HH
