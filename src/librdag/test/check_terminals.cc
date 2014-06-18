/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "expression.hh"
#include "exceptions.hh"
#include "gtest/gtest.h"
#include "warningmacros.h"
#include "exprtypeenum.h"

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

/*
 * Check OGTerminal base class behaves
 */
TEST(TerminalsTest, OGTerminalTest) {
  OGTerminal::Ptr terminal = OGRealScalar::create(3.14e0);
  OGExpr::Ptr expr = NEGATE::create(terminal);
  ASSERT_NE(nullptr, terminal->asOGTerminal());
  ASSERT_EQ(nullptr, expr->asOGTerminal());

  // Check throwing when toReal8Array and toComplex16Array are not implemented
  EXPECT_THROW(terminal->toReal8Array(), rdag_error);
  EXPECT_THROW(terminal->toComplex16Array(), rdag_error);
}

/*
 * Test OGScalar<T>
 */
TEST(TerminalsTest, OGScalarTest) {

  OGScalar<real8>::Ptr tmp = OGScalar<real8>::create(10);

  ASSERT_THROW(tmp->copy(), rdag_error);
  ASSERT_THROW((tmp->asFullOGRealMatrix()), rdag_error);
  ASSERT_THROW((tmp->asFullOGComplexMatrix()), rdag_error);
  ASSERT_THROW((tmp->createOwningCopy()), rdag_error);
  ASSERT_THROW((tmp->createComplexOwningCopy()), rdag_error);

  // print for test completeness
  tmp->debug_print();
}

/*
 * Test OGRealScalar
 */
TEST(TerminalsTest, OGRealScalarTest) {
  // test ctor
  real8 value = 3.14e0;
  OGRealScalar::Ptr tmp = OGRealScalar::create(value);
  // check ctor worked
  ASSERT_NE(tmp, OGRealScalar::Ptr{}); 

  // check getRows is ok
  ASSERT_EQ(tmp->getRows(), 1);

  // check getCols is ok
  ASSERT_EQ(tmp->getCols(), 1);

  // check getDatalen is ok
  ASSERT_EQ(tmp->getDatalen(), 1);

  // check getValue is ok
  ASSERT_EQ(tmp->getValue(), value);

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), REAL_SCALAR_ENUM);

  // wire up array for ArrOfArr test
  real8 expectedtmp[1] = {3.14e0};
  real8 ** expected = new real8 * [1];
  expected[0] = &(expectedtmp[0]);

  // check toArrayOfArrays()
  real8 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,1,1));
  delete [] computed[0];
  delete [] computed;

  // check toReal8ArrayOfArrays
  computed = tmp->toReal8ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,1,1));
  delete [] computed[0];
  delete [] computed;

  // check toComplex16ArrayOfArrays
  // wire up array for ArrOfArr test
  complex16 cexpectedtmp[1] = {3.14e0};
  complex16 ** cexpected = new complex16 * [1];
  cexpected[0] = &(cexpectedtmp[0]);
  complex16 ** cmplxcomputed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(cexpected,cmplxcomputed,1,1));
  delete [] cmplxcomputed[0];
  delete[] cmplxcomputed;

  // check copy and asOGRealScalar
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getValue(),copy->asOGRealScalar()->getValue());
  ASSERT_EQ(copy,copy->asOGRealScalar());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  OGComplexScalar::Ptr cmplx_tmp = OGComplexScalar::create(value);
  ASSERT_TRUE(*cmplx_tmp->asOGTerminal()==~*owningComplexCopy);

  // Check debug string
  copy->debug_print();

  // clean up
  delete [] expected;
  delete [] cexpected;
}



/*
 * Test OGComplexScalar
 */
TEST(TerminalsTest, OGComplexScalarTest) {
  // test ctor
  complex16 value = {3.14e0, 0.00159e0};
  OGComplexScalar::Ptr tmp = OGComplexScalar::create(value);
  // check ctor worked
  ASSERT_NE(tmp, nullptr); 

  // check getRows is ok
  ASSERT_EQ(tmp->getRows(), 1);

  // check getCols is ok
  ASSERT_EQ(tmp->getCols(), 1);

  // check getDatalen is ok
  ASSERT_EQ(tmp->getDatalen(), 1);

  // check getValue is ok
  ASSERT_EQ(tmp->getValue(), value);

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), COMPLEX_SCALAR_ENUM);

  // check can't promote as real
  ASSERT_THROW((tmp->asFullOGRealMatrix()), rdag_error);

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

  // check toReal8ArrayOfArrays, should throw
  ASSERT_THROW((tmp->toReal8ArrayOfArrays()), rdag_error);

  // check copy and asOGComplexScalar
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getValue(),copy->asOGComplexScalar()->getValue());
  ASSERT_EQ(copy,copy->asOGComplexScalar());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningComplexCopy);

  // Check debug string
  copy->debug_print();

  // clean up
  delete [] expected;
}


/*
 * Test OGIntegerScalar
 */
TEST(TerminalsTest, OGIntegerScalarTest) {
  // test ctor
  int4 value = 3;
  OGIntegerScalar::Ptr tmp = OGIntegerScalar::create(value);
  // check ctor worked
  ASSERT_NE(tmp, nullptr); 

  // check getRows is ok
  ASSERT_EQ(tmp->getRows(), 1);

  // check getCols is ok
  ASSERT_EQ(tmp->getCols(), 1);

  // check getDatalen is ok
  ASSERT_EQ(tmp->getDatalen(), 1);

  // check getValue is ok
  ASSERT_EQ(tmp->getValue(), value);

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), INTEGER_SCALAR_ENUM);  

  // check toReal8ArrayOfArrays, should throw
  ASSERT_THROW((tmp->toReal8ArrayOfArrays()), rdag_error);

  // check toComplex16ArrayOfArrays, should throw
  ASSERT_THROW((tmp->toComplex16ArrayOfArrays()), rdag_error);

  // check copy and asOGIntegerScalar
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getValue(),copy->asOGIntegerScalar()->getValue());
  ASSERT_EQ(copy,copy->asOGIntegerScalar());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  OGComplexScalar::Ptr cmplx_tmp = OGComplexScalar::create(value);
  ASSERT_TRUE(*cmplx_tmp->asOGTerminal()==~*owningComplexCopy);

  // Check debug string
  copy->debug_print();
}

/*
 * Test OGArray
 */
TEST(TerminalsTest, OGArrayTest) {

  shared_ptr<OGArray<real8>> tmp = shared_ptr<OGArray<real8>>{new OGArray<real8>{}};

  ASSERT_THROW((tmp->copy()), rdag_error);
  ASSERT_THROW((tmp->debug_print()), rdag_error);
  ASSERT_THROW((tmp->asFullOGRealMatrix()), rdag_error);
  ASSERT_THROW((tmp->asFullOGComplexMatrix()), rdag_error);
  ASSERT_THROW((tmp->createOwningCopy()), rdag_error);
  ASSERT_THROW((tmp->createComplexOwningCopy()), rdag_error);
}

TEST(TerminalsTest, OGMatrix_T_real8) {
  real8 data [12] = {1e0,2e0,3e0,4e0,5e0,6e0,7e0,8e0,9e0,10e0,11e0,12e0};
  size_t rows = 3;
  size_t cols = 4;
  OGMatrix<real8>::Ptr tmp = OGMatrix<real8>::create(data,rows,cols);
  OGNumeric::Ptr copy = tmp->copy();
  // can't do much here as at present we can't cast via RTTI asFOO() methods to an OGMatrix<T>
  ASSERT_TRUE(*tmp==~*(copy->asOGTerminal()));
}

TEST(TerminalsTest, OGMatrix_T_complex16) {
  complex16 data [12] = {{1e0,10e0},{2e0,20e0},{3e0,30e0},{4e0,40e0},{5e0,50e0},{6e0,60e0},{7e0,70e0},{8e0,80e0},{9e0,90e0},{10e0,100e0},{11e0,110e0},{12e0,120e0}};
  size_t rows = 3;
  size_t cols = 4;
  OGMatrix<complex16>::Ptr tmp = OGMatrix<complex16>::create(data,rows,cols);
  OGNumeric::Ptr copy = tmp->copy();
  // can't do much here as at present we can't cast via RTTI asFOO() methods to an OGMatrix<T>
  ASSERT_TRUE(*tmp==~*(copy->asOGTerminal()));
}

/*
 * Test OGRealMatrix
 */
TEST(TerminalsTest, OGRealMatrixTest) {
  // data
  real8 data [12] = {1e0,2e0,3e0,4e0,5e0,6e0,7e0,8e0,9e0,10e0,11e0,12e0};
  size_t rows = 3;
  size_t cols = 4;

  // attempt construct from nullptr, should throw
  real8 * null = nullptr;
  OGRealMatrix::Ptr tmp;
  ASSERT_THROW(OGRealMatrix::create(null,rows,cols), rdag_error);

  // attempt construct from ok data, own the data and delete it
  tmp = OGRealMatrix::create(new real8[2]{10,20},1,2, OWNER);
  ASSERT_NE(tmp, OGRealMatrix::Ptr{});
  ASSERT_TRUE(tmp->getDataAccess()==OWNER);

  // attempt construct from ok data
  tmp = OGRealMatrix::create(data,rows,cols);

  // check ctor worked
  ASSERT_NE(tmp, OGRealMatrix::Ptr{});

  // check it's a view context
  ASSERT_TRUE(tmp->getDataAccess()==VIEWER);

  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);

  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);

  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows*cols);

  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), REAL_MATRIX_ENUM);  

  // wire up array for ArrOfArr test
  real8 expectedtmp[12] = {1e0,4e0,7e0,10e0,2e0,5e0,8e0,11e0,3e0,6e0,9e0,12e0};
  real8 ** expected = new real8  * [rows];
  for(size_t i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }

  // check toArrayOfArrays()
  real8 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;

  // check toReal8ArrayOfArrays()
  computed = tmp->toReal8ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;

  // check toComplex16ArrayOfArrays()
  complex16 c_expectedtmp[12] = {{1e0,0},{4e0,0},{7e0,0},{10e0,0},{2e0,0},{5e0,0},{8e0,0},{11e0,0},{3e0,0},{6e0,0},{9e0,0},{12e0,0}};
  complex16 ** c_expected = new complex16  * [rows];
  for(size_t i = 0; i < rows; i++){
    c_expected[i] = &(c_expectedtmp[i*cols]);
  }
  complex16 ** c_computed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(c_expected,c_computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] c_computed[i];
  }
  delete [] c_computed;
  delete [] c_expected;

  // check copy and asOGRealMatrix
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGRealMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<real8>(tmp->getData(),copy->asOGRealMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGRealMatrix());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);
  ASSERT_FALSE(tmp->getData()==owningCopy->asOGRealMatrix()->getData()); // make sure the data is unique

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  complex16 * cdata = new complex16[rows*cols]();
  std::copy(data,data+(rows*cols),cdata);
  OGComplexMatrix::Ptr cmplx_tmp = OGComplexMatrix::create(cdata, rows, cols, OWNER);
  ASSERT_TRUE(*cmplx_tmp->asOGTerminal()==~*owningComplexCopy);
  ASSERT_FALSE(tmp->getData()==reinterpret_cast<real8 *>(owningComplexCopy->asOGComplexMatrix()->getData())); // make sure the data is unique

  // Check debug string
  copy->debug_print();

}


/*
 * Test OGComplexMatrix
 */
TEST(TerminalsTest, OGComplexMatrixTest) {
  // data
  complex16 data [12] = {{1e0,10e0},{2e0,20e0},{3e0,30e0},{4e0,40e0},{5e0,50e0},{6e0,60e0},{7e0,70e0},{8e0,80e0},{9e0,90e0},{10e0,100e0},{11e0,110e0},{12e0,120e0}};
  size_t rows = 3;
  size_t cols = 4;

  // attempt construct from nullptr, should throw
  complex16 * null = nullptr;
  OGComplexMatrix::Ptr tmp;
  ASSERT_THROW(OGComplexMatrix::create(null,rows,cols), rdag_error);

  // attempt construct from ok data, own the data and delete it
  tmp = OGComplexMatrix::create(new complex16[2]{{10,20},{30,40}},1,2, OWNER);
  ASSERT_NE(tmp, OGComplexMatrix::Ptr{});
  ASSERT_TRUE(tmp->getDataAccess()==OWNER);

  // attempt construct from ok data
  tmp = OGComplexMatrix::create(data,rows,cols);
  // check ctor worked
  ASSERT_NE(tmp, OGComplexMatrix::Ptr{});

  // check it's a view context
  ASSERT_TRUE(tmp->getDataAccess()==VIEWER);

  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);

  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);

  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows*cols);

  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), COMPLEX_MATRIX_ENUM);    

  // check can't promote as real
  ASSERT_THROW((tmp->asFullOGRealMatrix()), rdag_error);

  // wire up array for ArrOfArr test
  complex16 expectedtmp[12] = {{1e0,10e0},{4e0,40e0},{7e0,70e0},{10e0,100e0},{2e0,20e0},{5e0,50e0},{8e0,80e0},{11e0,110e0},{3e0,30e0},{6e0,60e0},{9e0,90e0},{12e0,120e0}};
  complex16 ** expected = new complex16  * [rows];
  for(size_t i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }

  // check toArrayOfArrays()
  complex16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;

  // check toComplex16ArrayOfArrays()
  computed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;  

  // check toReal8ArrayOfArrays, expect throw
  ASSERT_THROW((tmp->toReal8ArrayOfArrays()), rdag_error);

  // check copy and asOGComplexMatrix
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGComplexMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<complex16>(tmp->getData(),copy->asOGComplexMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGComplexMatrix());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);
  ASSERT_FALSE(tmp->getData()==owningCopy->asOGComplexMatrix()->getData()); // make sure the data is unique

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningComplexCopy);
  ASSERT_FALSE(tmp->getData()==owningComplexCopy->asOGComplexMatrix()->getData());

  // Check debug string
  copy->debug_print();
}


/*
 * Test OGRealDiagonalMatrix
 */
TEST(TerminalsTest, OGRealDiagonalMatrix) {
  // data
  real8 data [3] = {1e0,2e0,3e0};
  size_t rows = 3;
  size_t cols = 4;

  // attempt construct from nullptr, should throw
  real8 * null = nullptr;
  OGRealDiagonalMatrix::Ptr tmp;
  ASSERT_THROW(OGRealDiagonalMatrix::create(null,rows,cols), rdag_error);

  // attempt construct from ok data, own the data and delete it
  tmp = OGRealDiagonalMatrix::create(new real8[2]{10,20},2,2, OWNER);
  ASSERT_NE(tmp, OGRealDiagonalMatrix::Ptr{});
  ASSERT_TRUE(tmp->getDataAccess()==OWNER);

  // attempt construct from ok data
  tmp = OGRealDiagonalMatrix::create(data,rows,cols);

  // check ctor worked
  ASSERT_NE(tmp, OGRealDiagonalMatrix::Ptr{});

  // check it's a view context
  ASSERT_TRUE(tmp->getDataAccess()==VIEWER);

  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);

  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);

  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows>cols?cols:rows);

  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));  

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), REAL_DIAGONAL_MATRIX_ENUM);

  // wire up array for ArrOfArr test
  real8 expectedtmp[12] = {1e0,0e0,0e0,0e0,0e0,2e0,0e0,0e0,0e0,0e0,3e0,0e0};
  real8 expectedarr[3] = {1e0, 2e0, 3e0};
  real8 ** expected = new real8  * [rows];
  for(size_t i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }

  // check toArray()
  real8* arr = tmp->toArray();
  for (size_t i = 0; i < 3; i++)
    EXPECT_TRUE(arr[i] == expectedarr[i]);
  delete[] arr;

  // check toReal8Array()
  arr = tmp->toReal8Array();
  for (size_t i = 0; i < 3; i++)
    EXPECT_TRUE(arr[i] == expectedarr[i]);
  delete[] arr;

  // check toArrayOfArrays()
  real8 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;

  // check toReal8ArrayOfArrays()
  computed = tmp->toReal8ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;  

  // check toComplex16ArrayOfArrays, expect throw
  ASSERT_THROW((tmp->toComplex16ArrayOfArrays()), rdag_error);

  // check copy and asOGRealMatrix
  OGNumeric::Ptr  copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGRealDiagonalMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<real8>(tmp->getData(),copy->asOGRealDiagonalMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealDiagonalMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealDiagonalMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGRealDiagonalMatrix());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);
  ASSERT_FALSE(tmp->getData()==owningCopy->asOGRealDiagonalMatrix()->getData()); // make sure the data is unique

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  complex16 * cmplx_data = new complex16[tmp->getDatalen()];
  std::copy(data, data+tmp->getDatalen(), cmplx_data);
  OGComplexDiagonalMatrix::Ptr cmplx_tmp = OGComplexDiagonalMatrix::create(cmplx_data, rows, cols, OWNER);
  ASSERT_TRUE(*cmplx_tmp->asOGTerminal()==~*owningComplexCopy);
  ASSERT_FALSE(tmp->getData()==reinterpret_cast<real8 *>(owningComplexCopy->asOGComplexDiagonalMatrix()->getData())); // make sure the data is unique

  // Check debug string
  copy->debug_print();
  // Check debug string for matrix with more rows than cols (swapped rows/cols)
  tmp = OGRealDiagonalMatrix::create(data,cols,rows);
  tmp->debug_print();
}


/*
 * Test OGComplexDiagonalMatrix
 */
TEST(TerminalsTest, OGComplexDiagonalMatrix) {
  // data
  complex16 data [3] = {{1e0,10e0},{2e0,20e0},{3e0,30e0}};
  size_t rows = 3;
  size_t cols = 4;

  // attempt construct from nullptr, should throw
  complex16 * null = nullptr;
  OGComplexDiagonalMatrix::Ptr tmp;
  ASSERT_THROW(OGComplexDiagonalMatrix::create(null,rows,cols), rdag_error);

  // attempt construct from ok data, own the data and delete it
  tmp = OGComplexDiagonalMatrix::create(new complex16[2]{{10,20},{30,40}},2,2, OWNER);
  ASSERT_NE(tmp, OGComplexDiagonalMatrix::Ptr{});
  ASSERT_TRUE(tmp->getDataAccess()==OWNER);

  // attempt construct from ok data
  tmp = OGComplexDiagonalMatrix::create(data,rows,cols);

  // check ctor worked
  ASSERT_NE(tmp, OGComplexDiagonalMatrix::Ptr{});

  // check it's a view context
  ASSERT_TRUE(tmp->getDataAccess()==VIEWER);

  // check getRows
  ASSERT_EQ(tmp->getRows(), rows);

  // check getCols
  ASSERT_EQ(tmp->getCols(), cols);

  // check getDatalen
  ASSERT_EQ(tmp->getDatalen(), rows>cols?cols:rows);

  // check getData
  ASSERT_TRUE(ArrayEquals(tmp->getData(),data,tmp->getDatalen()));  

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), COMPLEX_DIAGONAL_MATRIX_ENUM);  

  // check can't promote as real
  ASSERT_THROW((tmp->asFullOGRealMatrix()), rdag_error);

  // wire up array for ArrOfArr test
  complex16 expectedtmp[12] = {{1e0,10e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{2e0,20e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{0e0,0e0},{3e0,30e0},{0e0,0e0}};
  complex16 expectedarr[3] = {{1e0,10e0}, {2e0,20e0}, {3e0,30e0}};
  complex16 ** expected = new complex16  * [rows];
  for(size_t i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }

  // check toArray()
  complex16* arr = tmp->toArray();
  for (size_t i = 0; i < 3; i++)
    EXPECT_TRUE(arr[i] == expectedarr[i]);
  delete[] arr;

  // check toComplex16Array()
  arr = tmp->toComplex16Array();
  for (size_t i = 0; i < 3; i++)
    EXPECT_TRUE(arr[i] == expectedarr[i]);
  delete[] arr;

  // check toArrayOfArrays()
  complex16 ** computed = tmp->toArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;

  // check toComplex16ArrayOfArrays()
  computed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;  

  // check toReal8ArrayOfArrays, expect throw
  ASSERT_THROW((tmp->toReal8ArrayOfArrays()), rdag_error);

  // check copy and asOGRealMatrix
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGComplexDiagonalMatrix()->getData());
  ASSERT_TRUE(ArrayEquals<complex16>(tmp->getData(),copy->asOGComplexDiagonalMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexDiagonalMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexDiagonalMatrix()->getCols());
  ASSERT_EQ(copy,copy->asOGComplexDiagonalMatrix());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);
  ASSERT_FALSE(tmp->getData()==owningCopy->asOGComplexDiagonalMatrix()->getData()); // make sure the data is unique

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningComplexCopy);
  ASSERT_FALSE(tmp->getData()==owningComplexCopy->asOGComplexDiagonalMatrix()->getData());

  // Check debug string
  copy->debug_print();
  // Check debug string for matrix with more rows than cols (swapped rows/cols)
  tmp = OGComplexDiagonalMatrix::create(data,cols,rows);
  tmp->debug_print();
}


/*
 * Test OGRealSparseMatrix
 */
TEST(TerminalsTest, OGRealSparseMatrix) {
  // data
  // real8[][] data = { { 1, 2, 0, 0 }, { 3, 0, 4, 0 }, { 0, 5, 6, 0 }, { 0, 0, 7, 0 }, {0, 0, 0, 0} };
  real8 data [7] = { 1.0e0, 3.0e0, 2.0e0, 5.0e0, 4.0e0, 6.0e0, 7.0e0 };
  int4 colPtr [6] = { 0, 2, 4, 7, 7, 7 };
  int4 rowIdx [7] = { 0, 1, 0, 2, 1, 2, 3 };
  size_t rows = 5;
  size_t cols = 4;

  OGRealSparseMatrix::Ptr tmp;
  int4 * nullintptr = nullptr;

  // attempt construct from colptr as null, should throw
  ASSERT_THROW(OGRealSparseMatrix::create(nullintptr,rowIdx,data,rows,cols), rdag_error);

  // attempt construct from rowidx as null, should throw
  ASSERT_THROW(OGRealSparseMatrix::create(colPtr,nullintptr,data,rows,cols), rdag_error);

  // attempt construct from data nullptr, should throw
  real8 * nulldata = nullptr;
  ASSERT_THROW(OGRealSparseMatrix::create(colPtr,rowIdx,nulldata,rows,cols), rdag_error);

    // attempt construct from ok data, own the data and delete it
  tmp = OGRealSparseMatrix::create(new int4[3]{0,2,2}, new int4[2]{0,1},new real8[2]{10,20},2,2, OWNER);
  ASSERT_NE(tmp, OGRealSparseMatrix::Ptr{});
  ASSERT_TRUE(tmp->getDataAccess()==OWNER);

  // attempt construct from ok data
  tmp = OGRealSparseMatrix::create(colPtr,rowIdx,data,rows,cols);

  // check ctor worked
  ASSERT_NE(tmp, OGRealSparseMatrix::Ptr{});

  // check it's a view context
  ASSERT_TRUE(tmp->getDataAccess()==VIEWER);

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

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), REAL_SPARSE_MATRIX_ENUM);

  // wire up array for ArrOfArr test
  real8 expectedtmp[20] = { 1e0, 2e0, 0e0, 0e0 ,  3e0, 0e0, 4e0, 0e0 ,  0e0, 5e0, 6e0, 0e0 ,  0e0, 0e0, 7e0, 0e0 , 0e0, 0e0, 0e0, 0e0};
  real8 ** expected = new real8  * [rows];
  for(size_t i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }

  // check toArray()
  real8* arr = tmp->toArray();
  for (size_t i = 0; i < 7; i++)
    EXPECT_EQ(arr[i], data[i]);
  delete[] arr;

  // check toReal8Array()
  arr = tmp->toReal8Array();
  for (size_t i = 0; i < 7; i++)
    EXPECT_EQ(arr[i], data[i]);
  delete[] arr;

  // check toArrayOfArrays()
  real8 ** computed = tmp->toArrayOfArrays(); 
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;

  // check toReal8ArrayOfArrays()
  computed = tmp->toReal8ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<real8>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;

  // check toComplex16ArrayOfArrays, expect throw
  ASSERT_THROW((tmp->toComplex16ArrayOfArrays()), rdag_error);

  // check copy and asOGRealSparseMatrix
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGRealSparseMatrix()->getData());
  ASSERT_TRUE(ArrayEquals(tmp->getData(),copy->asOGRealSparseMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGRealSparseMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGRealSparseMatrix()->getCols());
  ASSERT_EQ(tmp->getRowIdx(),copy->asOGRealSparseMatrix()->getRowIdx());
  ASSERT_TRUE(ArrayEquals(tmp->getRowIdx(),copy->asOGRealSparseMatrix()->getRowIdx(),7));
  ASSERT_EQ(tmp->getColPtr(),copy->asOGRealSparseMatrix()->getColPtr());
  ASSERT_TRUE(ArrayEquals(tmp->getColPtr(),copy->asOGRealSparseMatrix()->getColPtr(),6));
  ASSERT_EQ(copy,copy->asOGRealSparseMatrix());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);
  ASSERT_FALSE(tmp->getData()==owningCopy->asOGRealSparseMatrix()->getData()); // make sure the data is unique
  ASSERT_FALSE(tmp->asOGRealSparseMatrix()->getColPtr()==owningCopy->asOGRealSparseMatrix()->getColPtr()); // make sure the colptr data is unique
  ASSERT_FALSE(tmp->asOGRealSparseMatrix()->getRowIdx()==owningCopy->asOGRealSparseMatrix()->getRowIdx()); // make sure the rowidx data is unique

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()%owningComplexCopy);
  ASSERT_TRUE(owningComplexCopy->asOGComplexSparseMatrix()!=nullptr); // check type is correct as % was used to compare
  ASSERT_FALSE(tmp->getData()==reinterpret_cast<real8 *>(owningComplexCopy->asOGComplexSparseMatrix()->getData())); // make sure the data is unique
  ASSERT_FALSE(tmp->asOGRealSparseMatrix()->getColPtr()==owningComplexCopy->asOGComplexSparseMatrix()->getColPtr()); // make sure the colptr data is unique
  ASSERT_FALSE(tmp->asOGRealSparseMatrix()->getRowIdx()==owningComplexCopy->asOGComplexSparseMatrix()->getRowIdx()); // make sure the rowidx data is unique

  // Check debug string
  copy->debug_print();
}


/*
 * Test OGComplexSparseMatrix
 */
TEST(TerminalsTest, OGComplexSparseMatrix) {
  // data
  complex16 data [12] = { {1, 10}, {5, 0}, {0, 90}, {2, 20}, {0, 60}, {10, 100}, {0, 30}, {7, 70}, {11, 0}, {15, 0}, {0, 120}, {0, 160} };
  int4 colPtr [6] = { 0, 3, 6, 10, 12, 12 };
  int4 rowIdx [12] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 3, 2, 3 };
  size_t rows = 5;
  size_t cols = 4;

  OGComplexSparseMatrix::Ptr tmp;
  int4 * nullintptr = nullptr;

  // attempt construct from colptr as null, should throw
  ASSERT_THROW(OGComplexSparseMatrix::create(nullintptr,rowIdx,data,rows,cols), rdag_error);

  // attempt construct from rowidx as null, should throw
  ASSERT_THROW(OGComplexSparseMatrix::create(colPtr,nullintptr,data,rows,cols), rdag_error);

  // attempt construct from data nullptr, should throw
  complex16 * nulldata = nullptr;
  ASSERT_THROW(OGComplexSparseMatrix::create(colPtr,rowIdx,nulldata,rows,cols), rdag_error);

  // attempt construct from ok data, own the data and delete it
  tmp = OGComplexSparseMatrix::create(new int4[3]{0,2,2}, new int4[2]{0,1},new complex16[2]{{10,20},{30,40}},2,2, OWNER);
  ASSERT_NE(tmp, OGComplexSparseMatrix::Ptr{});
  ASSERT_TRUE(tmp->getDataAccess()==OWNER);

  // attempt construct from ok data
  tmp = OGComplexSparseMatrix::create(colPtr,rowIdx,data,rows,cols);

  // check ctor worked
  ASSERT_NE(tmp, OGComplexSparseMatrix::Ptr{});

  // check it's a view context
  ASSERT_TRUE(tmp->getDataAccess()==VIEWER);

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

  // check getType() is ok
  ASSERT_EQ(tmp->getType(), COMPLEX_SPARSE_MATRIX_ENUM);  

  // check can't promote as real
  ASSERT_THROW((tmp->asFullOGRealMatrix()), rdag_error);

  // wire up array for ArrOfArr test
  complex16 expectedtmp[20] = {
    {1,10},{2,20},{0,30},{0,0},
    {5,0},{0,60},{7,70},{0,0},
    {0,90},{10,100},{11,0},{0,120},
    {0,0},{0,0},{15,0},{0,160},
    {0,0},{0,0},{0,0},{0,0}
  };
  complex16 ** expected = new complex16  * [rows];
  for(size_t i = 0; i < rows; i++){
    expected[i] = &(expectedtmp[i*cols]);
  }

  // check toArray()
  complex16* arr = tmp->toArray();
  for (size_t i = 0; i < 12; i++)
    EXPECT_EQ(arr[i], data[i]);
  delete[] arr;

  // check toComplex16Array()
  arr = tmp->toComplex16Array();
  for (size_t i = 0; i < 12; i++)
    EXPECT_EQ(arr[i], data[i]);
  delete[] arr;

  // check toArrayOfArrays()
  complex16 ** computed = tmp->toArrayOfArrays(); 
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;

  // check toComplex16ArrayOfArrays()
  computed = tmp->toComplex16ArrayOfArrays();
  ASSERT_TRUE(ArrayOfArraysEquals<complex16>(expected,computed,rows,cols));
  for(size_t i = 0; i < rows; i++){
    delete [] computed[i];
  }
  delete [] computed;
  delete [] expected;

  // check toReal8ArrayOfArrays, expect throw
  ASSERT_THROW((tmp->toReal8ArrayOfArrays()), rdag_error);

  // check copy and asOGComplexSparseMatrix
  OGNumeric::Ptr copy = tmp->copy();
  ASSERT_EQ(tmp->getData(),copy->asOGComplexSparseMatrix()->getData());
  ASSERT_TRUE(ArrayEquals(tmp->getData(),copy->asOGComplexSparseMatrix()->getData(),tmp->getDatalen()));
  ASSERT_EQ(tmp->getRows(),copy->asOGComplexSparseMatrix()->getRows());
  ASSERT_EQ(tmp->getCols(),copy->asOGComplexSparseMatrix()->getCols());
  ASSERT_EQ(tmp->getRowIdx(),copy->asOGComplexSparseMatrix()->getRowIdx());
  ASSERT_TRUE(ArrayEquals(tmp->getRowIdx(),copy->asOGComplexSparseMatrix()->getRowIdx(),12));
  ASSERT_EQ(tmp->getColPtr(),copy->asOGComplexSparseMatrix()->getColPtr());
  ASSERT_TRUE(ArrayEquals(tmp->getColPtr(),copy->asOGComplexSparseMatrix()->getColPtr(),6));
  ASSERT_EQ(copy,copy->asOGComplexSparseMatrix());

  // check createOwningCopy
  OGTerminal::Ptr owningCopy{tmp->createOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()==~*owningCopy);
  ASSERT_FALSE(tmp->getData()==owningCopy->asOGComplexSparseMatrix()->getData()); // make sure the data is unique
  ASSERT_FALSE(tmp->asOGComplexSparseMatrix()->getColPtr()==owningCopy->asOGComplexSparseMatrix()->getColPtr()); // make sure the colptr data is unique
  ASSERT_FALSE(tmp->asOGComplexSparseMatrix()->getRowIdx()==owningCopy->asOGComplexSparseMatrix()->getRowIdx()); // make sure the rowidx data is unique

  // check createComplexOwningCopy
  OGTerminal::Ptr owningComplexCopy{tmp->createComplexOwningCopy()};
  ASSERT_TRUE(*tmp->asOGTerminal()%owningComplexCopy);
  ASSERT_TRUE(owningComplexCopy->asOGComplexSparseMatrix()!=nullptr); // check type is correct as % was used to compare
  ASSERT_FALSE(tmp->getData()==(owningComplexCopy->asOGComplexSparseMatrix()->getData())); // make sure the data is unique
  ASSERT_FALSE(tmp->asOGComplexSparseMatrix()->getColPtr()==owningComplexCopy->asOGComplexSparseMatrix()->getColPtr()); // make sure the colptr data is unique
  ASSERT_FALSE(tmp->asOGComplexSparseMatrix()->getRowIdx()==owningComplexCopy->asOGComplexSparseMatrix()->getRowIdx()); // make sure the rowidx data is unique

  // Check debug string
  copy->debug_print();
}
