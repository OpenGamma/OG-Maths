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

using namespace std;
using namespace librdag;

/**
 * The namespace used for testing nodes
 */
namespace testnodes {

/**
 * Use to set up a parameterised test for a unary node.
 */
#define UNARY_NODE_TEST_SETUP(__NODE)\
class __NODE##TEST: public UnaryOpTest<__NODE>{};\
TEST_P(__NODE##TEST, SimpleAssertResultTrue) {\
  CheckUnary<__NODE> * impl = GetParam();\
  impl->execute();\
  bool result = impl->resultCorrect();\
  EXPECT_TRUE(result);\
  if(result==false)\
  {\
    cout << "_______________________________________" << std::endl;\
    cout << "Test failure:" << std::endl << "Expected:" << std::endl;\
    impl->getResultPair()->second->asOGTerminal()->debug_print();\
    cout << "Calculated:" << std::endl;\
    impl->getResultPair()->first->asOGTerminal()->debug_print();\
    cout << "_______________________________________" << std::endl;\
  }\
  delete impl;\
}

/**
 * Use to set up a parameterised test for a binary node.
 */
#define BINARY_NODE_TEST_SETUP(__NODE)\
class __NODE##TEST: public BinaryOpTest<__NODE>{};\
TEST_P(__NODE##TEST, SimpleAssertResultTrue) {\
  CheckBinary<__NODE> * impl = GetParam();\
  impl->execute();\
  bool result = impl->resultCorrect();\
  EXPECT_TRUE(result);\
  if(result==false)\
  {\
    cout << "_______________________________________" << std::endl;\
    cout << "Test failure:" << std::endl << "Expected:" << std::endl;\
    impl->getResultPair()->second->asOGTerminal()->debug_print();\
    cout << "Calculated:" << std::endl;\
    impl->getResultPair()->first->asOGTerminal()->debug_print();\
    cout << "_______________________________________" << std::endl;\
  }\
  delete impl;\
}


/**
 * Instantiate a set of parameterised nodes tests (set up using the *_NODE_TEST_SETUP macro)
 */
#define INSTANTIATE_NODE_TEST_CASE_P(prefix, test_case_name, generator) INSTANTIATE_TEST_CASE_P(prefix, test_case_name##TEST, generator)

/**
 * Container for results
 */
typedef pair<OGNumeric::Ptr, OGNumeric::Ptr> ResultPair;

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
      CheckNode(OGNumeric::Ptr expected, CompareMethod comparisonMethod);
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
       * Gets the expected value.
       */
      virtual OGNumeric::Ptr getExpected() const;
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
    OGNumeric::Ptr _expected;
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
    CheckUnary(OGNumeric::Ptr input, OGNumeric::Ptr expected, CompareMethod comparisonMethod);
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
    OGNumeric::Ptr _input;
};

/**
 * Unary Operation test class
 */
template <typename T> class UnaryOpTest : public ::testing::TestWithParam<CheckUnary<T> *>
{
 public:
  UnaryOpTest();
  virtual ~UnaryOpTest();
  virtual void SetUp();
  virtual void TearDown();
};


/**
 * Class used for checking binary nodes execute as expected.
 * @param T the node to be checked.
 */
template <typename T> class CheckBinary: public CheckNode<T>
{
  public:
    /**
     * Construct a binary node check.
     * @param first_input the first input value.
     * @param second_input the second input value.
     * @param expected the expected value.
     * @param comparisonMethod how the results should be compared.
     */
    CheckBinary(OGNumeric::Ptr first_input, OGNumeric::Ptr second_input, OGNumeric::Ptr expected, CompareMethod comparisonMethod);
    ~CheckBinary();
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
    OGNumeric::Ptr _first_input;
    OGNumeric::Ptr _second_input;
};


/**
 * Binary Operation test class
 */
template <typename T> class BinaryOpTest : public ::testing::TestWithParam<CheckBinary<T> *>
{
 public:
  BinaryOpTest();
  virtual ~BinaryOpTest();
  virtual void SetUp();
  virtual void TearDown();
};

}

#endif // _TESTNODES_HH
