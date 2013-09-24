/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "visitor.hh"
#include "exceptions.hh"
#include "gtest/gtest.h"
#include "warningmacros.h"

using namespace std;
using namespace librdag;

/*
 * Check an array equals another array in value
 * @param expected the expected value
 * @param computed the value computed which is to be tested
 * @param length the length of the expected data
 */
template<typename T> bool ArrayEquals(T * expected, T * computed, size_t length)
{
    if(length<0){
      throw librdagException();
    }
    for(size_t i=0; i < length; i++)
    {
        if(expected[i]!=computed[i])
        {
            return false;
        }
    }
    return true;
}


/*
 * Check an array of arrays equals another in value
 * @param expected the expected value
 * @param computed the value computed which is to be tested
 * @param rows the number of rows in the expected data
 * @param cols the number of cols in the expected data
 */
template<typename T> bool ArrayOfArraysEquals(T ** expected, T ** computed, size_t rows, size_t cols)
{
    if(rows<0){
      throw librdagException();
    }
    for(size_t i=0; i < rows; i++)
    {
      if(!ArrayEquals<T>(expected[i],computed[i],cols))
      {
        return false;
      }
    }
    return true;
}

/**
 * To check that a class with accept Visitor is accepting correctly hasBeenVisited() returns true.
 */
class FakeVisitor: public librdag::Visitor
{
  public:
  void visit(OGExpr const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  void visit(OGArray<real16> const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  void visit(OGArray<complex16> const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  void visit(OGMatrix<real16> const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  void visit(OGMatrix<complex16> const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  void visit(OGScalar<real16> const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  void visit(OGScalar<complex16> const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  void visit(OGScalar<int> const SUPPRESS_UNUSED *thing){
    toggleHasBeenVisited();
  };
  bool hasBeenVisited(){
    return _hasBeenVisited;
  }
  private:
  bool _hasBeenVisited = false;
  void toggleHasBeenVisited()
  {
    _hasBeenVisited = !_hasBeenVisited;
  }
};

/*
 * Test OGRealScalar
 */
TEST(TerminalsTest, OGRealScalarTest) {
  // test ctor
  real16 value = 3.14e0;
  OGRealScalar * tmp;
  tmp = new OGRealScalar(value);
  // check ctor worked
  ASSERT_NE(tmp, nullptr); 
  
  // check getValue is ok
  ASSERT_EQ(tmp->getValue(), value);

  // wire up array for ArrOfArr test
  real16 expectedtmp[1] = {3.14e0};
  real16 ** expected = new real16 * [1];
  expected[0] = &(expectedtmp[0]);
  
  // check toArrayOfArrays()
  real16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real16>(expected,computed,1,1));
  delete [] computed[0];
  delete [] computed;

  // check toReal16ArrayOfArrays
  computed = tmp->toReal16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real16>(expected,computed,1,1));
  delete [] computed[0];
  delete [] computed;
   
  // check toComplex16ArrayOfArrays, should throw
  ASSERT_ANY_THROW(tmp->toComplex16ArrayOfArrays());
  
  // check visitor
  FakeVisitor * v = new FakeVisitor();
  tmp->accept(*v);
  ASSERT_TRUE(v->hasBeenVisited());
  delete v;
  
  
  // check copy and asOGRealScalar
  OGNumeric * copy = tmp->copy();
  ASSERT_EQ(tmp->getValue(),copy->asOGRealScalar()->getValue());
  ASSERT_EQ(copy,copy->asOGRealScalar());
  
  // clean up
  delete [] expected;
  delete copy;
  delete tmp;
}


/*
 * Test OGComplexScalar
 */
TEST(TerminalsTest, OGComplexScalarTest) {
  // test ctor
  complex16 value = {3.14e0, 0.00159e0};
  OGComplexScalar * tmp;
  tmp = new OGComplexScalar(value);
  // check ctor worked
  ASSERT_NE(tmp, nullptr); 
  
  // check getValue is ok
  ASSERT_EQ(tmp->getValue(), value);

  // wire up array for ArrOfArr test
  complex16 expectedtmp[1] = {{3.14e0, 0.00159e0}};
  complex16  ** expected = new complex16  * [1];
  expected[0] = &(expectedtmp[0]);
   
  // check toArrayOfArrays()
  complex16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,1,1));
  delete [] computed[0];
  delete [] computed;

  // check toComplex16ArrayOfArrays
  computed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,1,1));
  delete [] computed[0];
  delete [] computed;

  // check toReal16ArrayOfArrays, should throw
  ASSERT_ANY_THROW(tmp->toReal16ArrayOfArrays());
  
  // check visitor
  FakeVisitor * v = new FakeVisitor();
  tmp->accept(*v);
  ASSERT_TRUE(v->hasBeenVisited());
  delete v;  
  
  // check copy and asOGComplexScalar
  OGNumeric * copy = tmp->copy();
  ASSERT_EQ(tmp->getValue(),copy->asOGComplexScalar()->getValue());
  ASSERT_EQ(copy,copy->asOGComplexScalar());
  
  // clean up
  delete [] expected;
  delete copy;
  delete tmp;
}


/*
 * Test OGIntegerScalar
 */
TEST(TerminalsTest, OGIntegerScalarTest) {
  // test ctor
  int value = 3;
  OGIntegerScalar * tmp;
  tmp = new OGIntegerScalar(value);
  // check ctor worked
  ASSERT_NE(tmp, nullptr); 
  
  // check getValue is ok
  ASSERT_EQ(tmp->getValue(), value);

  // check toReal16ArrayOfArrays, should throw
  ASSERT_ANY_THROW(tmp->toReal16ArrayOfArrays());
   
  // check toComplex16ArrayOfArrays, should throw
  ASSERT_ANY_THROW(tmp->toComplex16ArrayOfArrays());
  
  // check copy and asOGIntegerScalar
  OGNumeric * copy = tmp->copy();
  ASSERT_EQ(tmp->getValue(),copy->asOGIntegerScalar()->getValue());
  ASSERT_EQ(copy,copy->asOGIntegerScalar());
  
  // clean up
  delete copy;
  delete tmp;
}

/*
 * Test OGRealMatrix
 */
TEST(TerminalsTest, OGRealMatrixTest) {
  // data
  real16 data [12] = {1,2,3,4,5,6,7,8,9,10,11,12};
  int rows = 3;
  int cols = 4;

  // attempt construct from nullptr, should throw
  real16 * null = nullptr;
  OGRealMatrix * tmp;
  ASSERT_ANY_THROW(tmp = new OGRealMatrix(null,rows,cols));
  
  // attempt construct from bad rows
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealMatrix(data,-1,cols));
  
  // attempt construct from bad cols
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealMatrix(data,rows,-1));
  
  // attempt construct from ok data
  tmp = nullptr;
  tmp = new OGRealMatrix(data,rows,cols);
  // check ctor worked
  ASSERT_NE(tmp, nullptr);
  
  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);
  
  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);
  
  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows*cols);
  
  // wire up array for ArrOfArr test
  real16 expectedtmp[12] = {1,4,7,10,2,5,8,11,3,6,9,12};
  real16 ** expected = new real16  * [rows];
  for(int i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }
  
  // check toArrayOfArrays()
  real16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  
  // check toReal16ArrayOfArrays()
  computed = tmp->toReal16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;  
   
  // check toComplex16ArrayOfArrays, expect throw
  ASSERT_ANY_THROW(tmp->toComplex16ArrayOfArrays());

  // check visitor
  FakeVisitor * v = new FakeVisitor();
  tmp->accept(*v);
  ASSERT_TRUE(v->hasBeenVisited());
  delete v;  
  
  // check copy and asOGRealMatrix
  OGNumeric * copy = tmp->copy();
  ASSERT_TRUE(ArrayEquals<real16>(tmp->getData(),copy->asOGRealMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGRealMatrix());
  
  // clean up
  delete copy;
  delete tmp;
}


/*
 * Test OGComplexMatrix
 */
TEST(TerminalsTest, OGComplexMatrixTest) {
  // data
  complex16 data [12] = {{1,10},{2,20},{3,30},{4,40},{5,50},{6,60},{7,70},{8,80},{9,90},{10,100},{11,110},{12,120}};
  int rows = 3;
  int cols = 4;

  // attempt construct from nullptr, should throw
  complex16 * null = nullptr;
  OGComplexMatrix * tmp;
  ASSERT_ANY_THROW(tmp = new OGComplexMatrix(null,rows,cols));
  
  // attempt construct from bad rows
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexMatrix(data,-1,cols));
  
  // attempt construct from bad cols
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexMatrix(data,rows,-1));
  
  // attempt construct from ok data
  tmp = nullptr;
  tmp = new OGComplexMatrix(data,rows,cols);
  // check ctor worked
  ASSERT_NE(tmp, nullptr);
  
  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);
  
  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);
  
  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows*cols);
  
  // wire up array for ArrOfArr test
  complex16 expectedtmp[12] = {{1,10},{4,40},{7,70},{10,100},{2,20},{5,50},{8,80},{11,110},{3,30},{6,60},{9,90},{12,120}};
  complex16 ** expected = new complex16  * [rows];
  for(int i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }
  
  // check toArrayOfArrays()
  complex16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  
  // check toComplex16ArrayOfArrays()
  computed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;  
   
  // check toReal16ArrayOfArrays, expect throw
  ASSERT_ANY_THROW(tmp->toReal16ArrayOfArrays());

  // check visitor
  FakeVisitor * v = new FakeVisitor();
  tmp->accept(*v);
  ASSERT_TRUE(v->hasBeenVisited());
  delete v;  
  
  // check copy and asOGComplexMatrix
  OGNumeric * copy = tmp->copy();
  ASSERT_TRUE(ArrayEquals<complex16>(tmp->getData(),copy->asOGComplexMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGComplexMatrix());
  
  // clean up
  delete copy;
  delete tmp;
}


/*
 * Test OGRealDiagonalMatrix
 */
TEST(TerminalsTest, OGRealDiagonalMatrix) {
  // data
  real16 data [3] = {1,2,3};
  int rows = 3;
  int cols = 4;

  // attempt construct from nullptr, should throw
  real16 * null = nullptr;
  OGRealDiagonalMatrix * tmp;
  ASSERT_ANY_THROW(tmp = new OGRealDiagonalMatrix(null,rows,cols));
  
  // attempt construct from bad rows
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealDiagonalMatrix(data,-1,cols));
  
  // attempt construct from bad cols
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealDiagonalMatrix(data,rows,-1));
  
  // attempt construct from ok data
  tmp = nullptr;
  tmp = new OGRealDiagonalMatrix(data,rows,cols);
  // check ctor worked
  ASSERT_NE(tmp, nullptr);
  
  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);
  
  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);
  
  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows>cols?cols:rows);
  
  // wire up array for ArrOfArr test
  real16 expectedtmp[12] = {1,0,0,0,0,2,0,0,0,0,3,0};
  real16 ** expected = new real16  * [rows];
  for(int i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }
  
  // check toArrayOfArrays()
  real16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  
  // check toReal16ArrayOfArrays()
  computed = tmp->toReal16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;  
   
  // check toComplex16ArrayOfArrays, expect throw
  ASSERT_ANY_THROW(tmp->toComplex16ArrayOfArrays());

  // check visitor
  FakeVisitor * v = new FakeVisitor();
  tmp->accept(*v);
  ASSERT_TRUE(v->hasBeenVisited());
  delete v;  
  
  // check copy and asOGRealMatrix
  OGNumeric * copy = tmp->copy();
  ASSERT_TRUE(ArrayEquals<real16>(tmp->getData(),copy->asOGRealDiagonalMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealDiagonalMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealDiagonalMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGRealDiagonalMatrix());
  
  // clean up
  delete copy;
  delete tmp;
}



/*
 * Test OGComplexDiagonalMatrix
 */
TEST(TerminalsTest, OGComplexDiagonalMatrix) {
  // data
  complex16 data [3] = {{1,10},{2,20},{3,30}};
  int rows = 3;
  int cols = 4;

  // attempt construct from nullptr, should throw
  complex16 * null = nullptr;
  OGComplexDiagonalMatrix * tmp;
  ASSERT_ANY_THROW(tmp = new OGComplexDiagonalMatrix(null,rows,cols));
  
  // attempt construct from bad rows
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexDiagonalMatrix(data,-1,cols));
  
  // attempt construct from bad cols
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexDiagonalMatrix(data,rows,-1));
  
  // attempt construct from ok data
  tmp = nullptr;
  tmp = new OGComplexDiagonalMatrix(data,rows,cols);
  // check ctor worked
  ASSERT_NE(tmp, nullptr);
  
  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);
  
  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);
  
  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows>cols?cols:rows);
  
  // wire up array for ArrOfArr test
  complex16 expectedtmp[12] = {{1,10},{0,0},{0,0},{0,0},{0,0},{2,20},{0,0},{0,0},{0,0},{0,0},{3,30},{0,0}};
  complex16 ** expected = new complex16  * [rows];
  for(int i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }
  
  // check toArrayOfArrays()
  complex16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  
  // check toComplex16ArrayOfArrays()
  computed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(int i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;  
   
  // check toReal16ArrayOfArrays, expect throw
  ASSERT_ANY_THROW(tmp->toReal16ArrayOfArrays());

  // check visitor
  FakeVisitor * v = new FakeVisitor();
  tmp->accept(*v);
  ASSERT_TRUE(v->hasBeenVisited());
  delete v;  
  
  // check copy and asOGRealMatrix
  OGNumeric * copy = tmp->copy();
  ASSERT_TRUE(ArrayEquals<complex16>(tmp->getData(),copy->asOGComplexDiagonalMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexDiagonalMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexDiagonalMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGComplexDiagonalMatrix());
  
  // clean up
  delete copy;
  delete tmp;
}
