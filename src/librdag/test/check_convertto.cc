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

  real16 value = 13.e0;
  pOGRealScalar scalar = pOGRealScalar{new OGRealScalar(value)};
  real16 data[1] = { value };
  data[0]=value;
  pOGRealMatrix expected = pOGRealMatrix{new OGRealMatrix(data,1,1)};
  ConvertTo c;
  pOGRealMatrix answer = c.convertToOGRealMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGIntegerScalarConvertToOGRealMatrix) {

  int value = 13;
  pOGIntegerScalar scalar = pOGIntegerScalar{new OGIntegerScalar(value)};
  real16 data[1];
  data[0] = value;
  pOGRealMatrix expected = pOGRealMatrix{new OGRealMatrix(data,1,1)};
  ConvertTo c;
  pOGRealMatrix answer = c.convertToOGRealMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGLogicalMatrixConvertToOGRealMatrix) {

  real16 input_values[12] = {1,0,1,0,1,0,1,0,1,0,1,0};
  pOGLogicalMatrix input = pOGLogicalMatrix{new OGLogicalMatrix(input_values,3,4)};
  real16 expected_values[12] = {1,0,1,0,1,0,1,0,1,0,1,0};
  pOGRealMatrix expected = pOGRealMatrix{new OGRealMatrix(expected_values,3,4)};
  ConvertTo c;
  pOGRealMatrix answer = c.convertToOGRealMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGRealDiagonalMatrixConvertToOGRealMatrix) {

  real16 input_values[4] = {1,2,3,4};
  real16 expected_values[24] = {1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4,0,0,0,0,0,0,0,0};
  pOGRealDiagonalMatrix input = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(input_values,4,6)};
  pOGRealMatrix expected = pOGRealMatrix{new OGRealMatrix(expected_values,4,6)};
  ConvertTo c;
  pOGRealMatrix answer = c.convertToOGRealMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGRealSparseMatrixConvertToOGRealMatrix) {

  real16 input_values[7] = {1,4,2,8,11,6,12};
  int input_colPtr[4] = {0,2,5,7};
  int input_rowIdx[7] = {0,1,0,2,3,1,3};
  real16 expected_values[12] = {1,4,0,0,2,0,8,11,0,6,0,12};
  pOGRealSparseMatrix input = pOGRealSparseMatrix{new OGRealSparseMatrix(input_colPtr, input_rowIdx, input_values,4,3)};
  pOGRealMatrix expected = pOGRealMatrix{new OGRealMatrix(expected_values,4,3)};
  ConvertTo c;
  pOGRealMatrix answer = c.convertToOGRealMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealScalarConvertToOGComplexMatrix) {

  real16 value = {13.e0};
  pOGRealScalar scalar = pOGRealScalar{new OGRealScalar(value)};
  complex16 data[1];
  data[0]=value;
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(data,1,1)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGIntegerScalarConvertToOGComplexMatrix) {

  int value = {13};
  pOGIntegerScalar scalar = pOGIntegerScalar{new OGIntegerScalar(13)};
  complex16 data[1]; 
  data[0]=value;
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(data,1,1)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGComplexScalarConvertToOGComplexMatrix) {

  complex16 value = {13.e0, 37.e0};
  pOGComplexScalar scalar = pOGComplexScalar{new OGComplexScalar(value)};
  complex16 data[1];
  data[0]=value;
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(data,1,1)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(scalar);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealDiagonalMatrixConvertToOGComplexMatrix) {

  real16 input_values[4] = {1,2,3,4};
  complex16 expected_values[24] = {{1,0},{0,0},{0,0},{0,0},{0,0},{2,0},{0,0},{0,0},{0,0},{0,0},{3,0},{0,0},{0,0},{0,0},{0,0},{4,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
  pOGRealDiagonalMatrix input = pOGRealDiagonalMatrix{new OGRealDiagonalMatrix(input_values,4,6)};
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(expected_values,4,6)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGComplexDiagonalMatrixConvertToOGComplexMatrix) {

  complex16 input_values[4] = {{1,10},{2,20},{3,30},{4,40}};
  complex16 expected_values[24] = {{1,10},{0,0},{0,0},{0,0},{0,0},{2,20},{0,0},{0,0},{0,0},{0,0},{3,30},{0,0},{0,0},{0,0},{0,0},{4,40},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
  pOGComplexDiagonalMatrix input = pOGComplexDiagonalMatrix{new OGComplexDiagonalMatrix(input_values,4,6)};
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(expected_values,4,6)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}

TEST(ConvertToTest, OGRealSparseMatrixConvertToOGComplexMatrix) {

  real16 input_values[7] = {1,4,2,8,11,6,12};
  int input_colPtr[4] = {0,2,5,7};
  int input_rowIdx[7] = {0,1,0,2,3,1,3};
  complex16 expected_values[12] = {{1,0},{4,0},{0,0},{0,0},{2,0},{0,0},{8,0},{11,0},{0,0},{6,0},{0,0},{12,0}};
  pOGRealSparseMatrix input = pOGRealSparseMatrix{new OGRealSparseMatrix(input_colPtr, input_rowIdx, input_values,4,3)};
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(expected_values,4,3)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGComplexSparseMatrixConvertToOGComplexMatrix) {

  complex16 input_values[8] = {{1,0},{4,0},{0,70},{2,0},{8,0},{11,0},{6,0},{12,0}};
  int input_colPtr[4] = {0,3,6,8};
  int input_rowIdx[8] = {0,1,2,0,2,3,1,3};
  complex16 expected_values[12] = {{1,0},{4,0},{0,70},{0,0},{2,0},{0,0},{8,0},{11,0},{0,0},{6,0},{0,0},{12,0}};
  pOGComplexSparseMatrix input = pOGComplexSparseMatrix{new OGComplexSparseMatrix(input_colPtr, input_rowIdx, input_values,4,3)};
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(expected_values,4,3)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealMatrixConvertToOGComplexMatrix) {

  real16 input_values[12] = {1,4,7,10,2,5,8,11,3,6,9,12};
  complex16 expected_values[12] = {{1,0},{4,0},{7,0},{10,0},{2,0},{5,0},{8,0},{11,0},{3,0},{6,0},{9,0},{12,0}};
  pOGRealMatrix input = pOGRealMatrix{new OGRealMatrix(input_values,4,3)};
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(expected_values,4,3)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}


TEST(ConvertToTest, OGRealMatrixConvertToOGLogicalMatrix) {

  real16 input_values[12] = {1,0,1,0,1,0,1,0,1,0,1,0};
  complex16 expected_values[12] = {{1,0},{0,0},{1,0},{0,0},{1,0},{0,0},{1,0},{0,0},{1,0},{0,0},{1,0},{0,0}};
  pOGLogicalMatrix input = pOGLogicalMatrix{new OGLogicalMatrix(input_values,4,3)};
  pOGComplexMatrix expected = pOGComplexMatrix{new OGComplexMatrix(expected_values,4,3)};
  ConvertTo c;
  pOGComplexMatrix answer = c.convertToOGComplexMatrix(input);
  ASSERT_TRUE(*expected==~*answer);
}
