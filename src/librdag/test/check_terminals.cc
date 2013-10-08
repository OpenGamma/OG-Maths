/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "expression.hh"
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
      throw runtime_error("Negative array length specified");
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
    if(rows<0 || cols<0){
      throw runtime_error("Negative row or column count specified");
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
 * Check OGTerminal base class behaves
 */
TEST(TerminalsTest, OGTerminalTest) {
  OGNumeric * terminal_t = new OGRealScalar(3.14e0);
  ArgContainer * args = new ArgContainer();
  args->push_back(terminal_t);
  OGNumeric * expr_t = new NEGATE(args);
  const OGTerminal * term;
  term = terminal_t->asOGTerminal();
  ASSERT_NE(term,nullptr);
  term = expr_t->asOGTerminal();
  ASSERT_EQ(term,nullptr);
  delete expr_t;
  delete term;
}

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
  
  // Check debug string
  copy->debug_print();

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
  
  // Check debug string
  copy->debug_print();

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
  
  // Check debug string
  copy->debug_print();

  // clean up
  delete copy;
  delete tmp;
}

/*
 * Test OGRealMatrix
 */
TEST(TerminalsTest, OGRealMatrixTest) {
  // data
  real16 data [12] = {1e0,2e0,3e0,4e0,5e0,6e0,7e0,8e0,9e0,10e0,11e0,12e0};
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
  
  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));
  
  // wire up array for ArrOfArr test
  real16 expectedtmp[12] = {1e0,4e0,7e0,10e0,2e0,5e0,8e0,11e0,3e0,6e0,9e0,12e0};
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
  ASSERT_EQ(tmp->getData(),copy->asOGRealMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<real16>(tmp->getData(),copy->asOGRealMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGRealMatrix());
  
  // Check debug string
  copy->debug_print();

  // clean up
  delete copy;
  delete tmp;
}


/*
 * Test OGComplexMatrix
 */
TEST(TerminalsTest, OGComplexMatrixTest) {
  // data
  complex16 data [12] = {{1e0,10e0},{2e0,20e0},{3e0,30e0},{4e0,40e0},{5e0,50e0},{6e0,60e0},{7e0,70e0},{8e0,80e0},{9e0,90e0},{10e0,100e0},{11e0,110e0},{12e0,120e0}};
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
  
  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));
  
  // wire up array for ArrOfArr test
  complex16 expectedtmp[12] = {{1e0,10e0},{4e0,40e0},{7e0,70e0},{10e0,100e0},{2e0,20e0},{5e0,50e0},{8e0,80e0},{11e0,110e0},{3e0,30e0},{6e0,60e0},{9e0,90e0},{12e0,120e0}};
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
  ASSERT_EQ(tmp->getData(),copy->asOGComplexMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<complex16>(tmp->getData(),copy->asOGComplexMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGComplexMatrix());
  
  // Check debug string
  copy->debug_print();

  // clean up
  delete copy;
  delete tmp;
}


/*
 * Test OGRealDiagonalMatrix
 */
TEST(TerminalsTest, OGRealDiagonalMatrix) {
  // data
  real16 data [3] = {1e0,2e0,3e0};
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
  
  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));  
  
  // wire up array for ArrOfArr test
  real16 expectedtmp[12] = {1e0,0e0,0e0,0e0,0e0,2e0,0e0,0e0,0e0,0e0,3e0,0e0};
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
  ASSERT_EQ(tmp->getData(),copy->asOGRealDiagonalMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<real16>(tmp->getData(),copy->asOGRealDiagonalMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealDiagonalMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealDiagonalMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGRealDiagonalMatrix());
  
  // Check debug string
  copy->debug_print();

  // clean up
  delete copy;
  delete tmp;
}



/*
 * Test OGComplexDiagonalMatrix
 */
TEST(TerminalsTest, OGComplexDiagonalMatrix) {
  // data
  complex16 data [3] = {{1e0,10e0},{2e0,20e0},{3e0,30e0}};
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

  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));  
  
  // wire up array for ArrOfArr test
  complex16 expectedtmp[12] = {{1e0,10e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{2e0,20e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{3e0,30e0},{0e0,0e0}};
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
  ASSERT_EQ(tmp->getData(),copy->asOGComplexDiagonalMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<complex16>(tmp->getData(),copy->asOGComplexDiagonalMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexDiagonalMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexDiagonalMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGComplexDiagonalMatrix());
  
  // Check debug string
  copy->debug_print();

  // clean up
  delete copy;
  delete tmp;
}


/*
 * Test OGRealSparseMatrix
 */
TEST(TerminalsTest, OGRealSparseMatrix) {
  // data
  // double[][] data = { { 1, 2, 0, 0 }, { 3, 0, 4, 0 }, { 0, 5, 6, 0 }, { 0, 0, 7, 0 }, {0, 0, 0, 0} };
  real16 data [7] = { 1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  int colPtr [6] = { 0, 2, 4, 7, 7, 7 };
  int rowIdx [7] = { 0, 1, 0, 2, 1, 2, 3 };
  int rows = 5;
  int cols = 4;

  OGRealSparseMatrix * tmp;
  int * nullintptr = nullptr;
  
  // attempt construct from colptr as null, should throw
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealSparseMatrix(nullintptr,rowIdx,data,rows,cols));

  // attempt construct from rowidx as null, should throw
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealSparseMatrix(colPtr,nullintptr,data,rows,cols));

  // attempt construct from data nullptr, should throw
  real16 * nulldata = nullptr;
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealSparseMatrix(colPtr,rowIdx,nulldata,rows,cols));
  
  // attempt construct from bad rows
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealSparseMatrix(colPtr,rowIdx,data,-1,cols));

  // attempt construct from bad cols
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGRealSparseMatrix(colPtr,rowIdx,data,rows,-1));
  
  // attempt construct from ok data
  tmp = nullptr;
  tmp = new OGRealSparseMatrix(colPtr,rowIdx,data,rows,cols);
  // check ctor worked
  ASSERT_NE(tmp, nullptr);
  
  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);
  
  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);

  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), colPtr[cols]);

  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));
  
  // check getColPtr
  ASSERT_TRUE(ArrayEquals(tmp->getColPtr(),colPtr,6));

  // check getRowIdx
  ASSERT_TRUE(ArrayEquals(tmp->getRowIdx(),rowIdx,7));
  
  // wire up array for ArrOfArr test
  real16 expectedtmp[20] = { 1e0, 2e0, 0e0, 0e0 ,  3e0, 0e0, 4e0, 0e0 ,  0e0, 5e0, 6e0, 0e0 ,  0e0, 0e0, 7e0, 0e0 , 0e0, 0e0, 0e0, 0e0};
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
  
  // check copy and asOGRealSparseMatrix
  OGNumeric * copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGRealSparseMatrix()->getData());
  ASSERT_TRUE(ArrayEquals(tmp->getData(),copy->asOGRealSparseMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealSparseMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealSparseMatrix()->getCols());
  ASSERT_EQ(tmp->getRowIdx(),copy->asOGRealSparseMatrix()->getRowIdx());
  ASSERT_TRUE(ArrayEquals(tmp->getRowIdx(),copy->asOGRealSparseMatrix()->getRowIdx(),7));
  ASSERT_EQ(tmp->getColPtr(),copy->asOGRealSparseMatrix()->getColPtr());
  ASSERT_TRUE(ArrayEquals(tmp->getColPtr(),copy->asOGRealSparseMatrix()->getColPtr(),6));
  ASSERT_EQ(copy,copy->asOGRealSparseMatrix());
  
  // Check debug string
  copy->debug_print();

  // clean up
  delete copy;
  delete tmp;
}


/*
 * Test OGComplexSparseMatrix
 */
TEST(TerminalsTest, OGComplexSparseMatrix) {
  // data
//   double[][] realData = { { 1, 2, 0, 0 }, { 5, 0, 7, 0 }, { 0, 10, 11, 0 }, { 0, 0, 15, 0 } };
//   double[][] imagData = { { 10, 20, 30, 0 }, { 0, 60, 70, 0 }, { 90, 100, 0, 120 }, { 0, 0, 0, 160 } };
  complex16 data [12] = { {1, 10}, {5, 0}, {0, 90}, {2, 20}, {0, 60}, {10, 100}, {0, 30}, {7, 70}, {11, 0}, {15, 0}, {0, 120}, {0, 160} };
  int colPtr [6] = { 0, 3, 6, 10, 12, 12 };
  int rowIdx [12] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 3, 2, 3 };
  int rows = 5;
  int cols = 4;

  OGComplexSparseMatrix * tmp;
  int * nullintptr = nullptr;
  
  // attempt construct from colptr as null, should throw
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexSparseMatrix(nullintptr,rowIdx,data,rows,cols));

  // attempt construct from rowidx as null, should throw
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexSparseMatrix(colPtr,nullintptr,data,rows,cols));

  // attempt construct from data nullptr, should throw
  complex16 * nulldata = nullptr;
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexSparseMatrix(colPtr,rowIdx,nulldata,rows,cols));
  
  // attempt construct from bad rows
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexSparseMatrix(colPtr,rowIdx,data,-1,cols));

  // attempt construct from bad cols
  tmp = nullptr;
  ASSERT_ANY_THROW(tmp = new OGComplexSparseMatrix(colPtr,rowIdx,data,rows,-1));
  
  // attempt construct from ok data
  tmp = nullptr;
  tmp = new OGComplexSparseMatrix(colPtr,rowIdx,data,rows,cols);
  // check ctor worked
  ASSERT_NE(tmp, nullptr);
  
  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);
  
  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);

  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), colPtr[cols]);

  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));
  
  // check getColPtr
  ASSERT_TRUE(ArrayEquals(tmp->getColPtr(),colPtr,6));

  // check getRowIdx
  ASSERT_TRUE(ArrayEquals(tmp->getRowIdx(),rowIdx,12));  
  
  // wire up array for ArrOfArr test
  complex16 expectedtmp[20] = {
    {1,10},{2,20},{0,30},{0,0},
    {5,0},{0,60},{7,70},{0,0},
    {0,90},{10,100},{11,0},{0,120},
    {0,0},{0,0},{15,0},{0,160},
    {0,0},{0,0},{0,0},{0,0}
  };
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
  
  // check copy and asOGComplexSparseMatrix
  OGNumeric * copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGComplexSparseMatrix()->getData());
  ASSERT_TRUE(ArrayEquals(tmp->getData(),copy->asOGComplexSparseMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexSparseMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexSparseMatrix()->getCols());
  ASSERT_EQ(tmp->getRowIdx(),copy->asOGComplexSparseMatrix()->getRowIdx());
  ASSERT_TRUE(ArrayEquals(tmp->getRowIdx(),copy->asOGComplexSparseMatrix()->getRowIdx(),12));
  ASSERT_EQ(tmp->getColPtr(),copy->asOGComplexSparseMatrix()->getColPtr());
  ASSERT_TRUE(ArrayEquals(tmp->getColPtr(),copy->asOGComplexSparseMatrix()->getColPtr(),6));
  ASSERT_EQ(copy,copy->asOGComplexSparseMatrix());
  
  // Check debug string
  copy->debug_print();

  // clean up
  delete copy;
  delete tmp;
}
