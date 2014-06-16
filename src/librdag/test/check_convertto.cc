/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "exceptions.hh"
#include "gtest/gtest.h"
#include "warningmacros.h"
#include "convertto.hh"
#include "dispatch.hh"
#include <iostream>
using namespace std;
using namespace librdag;

TEST(ConvertToTest, OGRealScalarConvertToOGRealMatrix) {

  real8 value = 13.e0;
  OGRealScalar::Ptr scalar = OGRealScalar::create(value);
  real8 data[1] = { value };
  data[0]=value;
  OGRealMatrix::Ptr expected = OGRealMatrix::create(data,1,1);
  ConvertTo c;
  OGRealMatrix::Ptr answer = c.convertToOGRealMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGIntegerScalarConvertToOGRealMatrix) {

  int4 value = 13;
  OGIntegerScalar::Ptr scalar = OGIntegerScalar::create(value);
  real8 data[1];
  data[0] = value;
  OGRealMatrix::Ptr expected = OGRealMatrix::create(data,1,1);
  ConvertTo c;
  OGRealMatrix::Ptr answer = c.convertToOGRealMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGLogicalMatrixConvertToOGRealMatrix) {

  real8 input_values[12] = {1,0,1,0,1,0,1,0,1,0,1,0};
  OGLogicalMatrix::Ptr input = OGLogicalMatrix::create(input_values,3,4);
  real8 expected_values[12] = {1,0,1,0,1,0,1,0,1,0,1,0};
  OGRealMatrix::Ptr expected = OGRealMatrix::create(expected_values,3,4);
  ConvertTo c;
  OGRealMatrix::Ptr answer = c.convertToOGRealMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGRealDiagonalMatrixConvertToOGRealMatrix) {

  real8 input_values[4] = {1,2,3,4};
  real8 expected_values[24] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4,0,0,0,0,0,0,0,0};
  OGRealDiagonalMatrix::Ptr input = OGRealDiagonalMatrix::create(input_values,4,6);
  OGRealMatrix::Ptr expected = OGRealMatrix::create(expected_values,4,6);
  ConvertTo c;
  OGRealMatrix::Ptr answer = c.convertToOGRealMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGRealSparseMatrixConvertToOGRealMatrix) {

  real8 input_values[7] = {1,4,2,8,11,6,12};
  int4 input_colPtr[4] = {0,2,5,7};
  int4 input_rowIdx[7] = {0,1,0,2,3,1,3};
  real8 expected_values[12] = {1,4,0,0,2,0,8,11,0,6,0,12};
  OGRealSparseMatrix::Ptr input = OGRealSparseMatrix::create(input_colPtr, input_rowIdx, input_values,4,3);
  OGRealMatrix::Ptr expected = OGRealMatrix::create(expected_values,4,3);
  ConvertTo c;
  OGRealMatrix::Ptr answer = c.convertToOGRealMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealScalarConvertToOGComplexMatrix) {

  real8 value = {13.e0};
  OGRealScalar::Ptr scalar = OGRealScalar::create(value);
  complex16 data[1];
  data[0]=value;
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(data,1,1);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGIntegerScalarConvertToOGComplexMatrix) {

  int4 value = {13};
  OGIntegerScalar::Ptr scalar = OGIntegerScalar::create(13);
  complex16 data[1]; 
  data[0]=value;
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(data,1,1);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGComplexScalarConvertToOGComplexMatrix) {

  complex16 value = {13.e0, 37.e0};
  OGComplexScalar::Ptr scalar = OGComplexScalar::create(value);
  complex16 data[1];
  data[0]=value;
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(data,1,1);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealDiagonalMatrixConvertToOGComplexMatrix) {

  real8 input_values[4] = {1,2,3,4};
  complex16 expected_values[24] = {{1,0},{0,0},{0,0},{0,0},{0,0},{2,0},{0,0},{0,0},{0,0},{0,0},{3,0},{0,0},{0,0},{0,0},{0,0},{4,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
  OGRealDiagonalMatrix::Ptr input = OGRealDiagonalMatrix::create(input_values,4,6);
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(expected_values,4,6);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGComplexDiagonalMatrixConvertToOGComplexMatrix) {

  complex16 input_values[4] = {{1,10},{2,20},{3,30},{4,40}};
  complex16 expected_values[24] = {{1,10},{0,0},{0,0},{0,0},{0,0},{2,20},{0,0},{0,0},{0,0},{0,0},{3,30},{0,0},{0,0},{0,0},{0,0},{4,40},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
  OGComplexDiagonalMatrix::Ptr input = OGComplexDiagonalMatrix::create(input_values,4,6);
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(expected_values,4,6);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGRealSparseMatrixConvertToOGComplexMatrix) {

  real8 input_values[7] = {1,4,2,8,11,6,12};
  int4 input_colPtr[4] = {0,2,5,7};
  int4 input_rowIdx[7] = {0,1,0,2,3,1,3};
  complex16 expected_values[12] = {{1,0},{4,0},{0,0},{0,0},{2,0},{0,0},{8,0},{11,0},{0,0},{6,0},{0,0},{12,0}};
  OGRealSparseMatrix::Ptr input = OGRealSparseMatrix::create(input_colPtr, input_rowIdx, input_values,4,3);
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(expected_values,4,3);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGComplexSparseMatrixConvertToOGComplexMatrix) {

  complex16 input_values[8] = {{1,0},{4,0},{0,70},{2,0},{8,0},{11,0},{6,0},{12,0}};
  int4 input_colPtr[4] = {0,3,6,8};
  int4 input_rowIdx[8] = {0,1,2,0,2,3,1,3};
  complex16 expected_values[12] = {{1,0},{4,0},{0,70},{0,0},{2,0},{0,0},{8,0},{11,0},{0,0},{6,0},{0,0},{12,0}};
  OGComplexSparseMatrix::Ptr input = OGComplexSparseMatrix::create(input_colPtr, input_rowIdx, input_values,4,3);
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(expected_values,4,3);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealMatrixConvertToOGComplexMatrix) {

  real8 input_values[12] = {1,4,7,10,2,5,8,11,3,6,9,12};
  complex16 expected_values[12] = {{1,0},{4,0},{7,0},{10,0},{2,0},{5,0},{8,0},{11,0},{3,0},{6,0},{9,0},{12,0}};
  OGRealMatrix::Ptr input = OGRealMatrix::create(input_values,4,3);
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(expected_values,4,3);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealMatrixConvertToOGLogicalMatrix) {

  real8 input_values[12] = {1,0,1,0,1,0,1,0,1,0,1,0};
  complex16 expected_values[12] = {{1,0},{0,0},{1,0},{0,0},{1,0},{0,0},{1,0},{0,0},{1,0},{0,0},{1,0},{0,0}};
  OGLogicalMatrix::Ptr input = OGLogicalMatrix::create(input_values,4,3);
  OGComplexMatrix::Ptr expected = OGComplexMatrix::create(expected_values,4,3);
  ConvertTo c;
  OGComplexMatrix::Ptr answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}
