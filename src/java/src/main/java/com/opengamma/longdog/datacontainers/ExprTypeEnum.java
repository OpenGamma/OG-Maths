/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers;

/**
 * Enumerate expr types.
 */
public enum ExprTypeEnum {

  //@formatter:off
  //CSOFF
  UNKNOWNEXPR(0x0000L),

  /*
   * Data container tokens
   */
  OGRealScalar            (0x0002L),
  OGComplexScalar         (0x0003L),
  OGRealMatrix            (0x0005L),
  OGComplexMatrix         (0x0007L),
  OGRealSparseMatrix      (0x000BL),
  OGComplexSparseMatrix   (0x000DL),
  OGRealDiagonalMatrix    (0x0011L),
  OGComplexDiagonalMatrix (0x0013L),
  OGLogicalMatrix         (0x0017L),
  OGIntegerScalar         (0x001DL),


  /*
   * expr tokens for functions
   */
  // Non-generated nodes
  COPY         (0x010DL),
  SELECTRESULT (0x010FL),
  NORM2        (0x0115L),
  SVD          (0x0119L),

  // Unary expression nodes - start at 175 to leave room for extra non-generated nodes
  ABS (0X0175L),
  ACOS (0X017BL),
  ACOSH (0X017FL),
  ANGLE (0X0185L),
  ASIN (0X018DL),
  ASINH (0X0191L),
  ATAN (0X0199L),
  ATANH (0X01A3L),
  CONJ (0X01A5L),
  COS (0X01AFL),
  COSH (0X01B1L),
  CTRANSPOSE (0X01B7L),
  DIAG (0X01BBL),
  EIG (0X01C1L),
  ERF (0X01C9L),
  ERFC (0X01CDL),
  EXP (0X01CFL),
  EYE (0X01D3L),
  FLIPLR (0X01DFL),
  FLIPUD (0X01E7L),
  HILB (0X01EBL),
  IMAG (0X01F3L),
  INVHILB (0X01F7L),
  LOG (0X01FDL),
  LU (0X0209L),
  NEGATE (0X020BL),
  NORMCDF (0X021DL),
  REAL (0X0223L),
  ROUND (0X022DL),
  SIN (0X0233L),
  SINH (0X0239L),
  SQRT (0X023BL),
  TAN (0X0241L),
  TRANSPOSE (0X024BL),
  WILKINSON (0X0251L),
  // Binary Expression nodes
  HORZCAT      (0X0257L),
  MLDIVIDE     (0X0259L),
  MTIMES       (0X025FL),
  PLUS         (0X0265L),
  POWER        (0X0269L),
  RDIVIDE      (0X026BL),
  TIMES        (0X0277L),
  VERTCAT      (0X0281L),
  DOT          (0X0283L);


  //CSON
  //@formatter:on

  private long _hashDefined;

  /**
   * Construct a new expr enum
   */
  ExprTypeEnum(long value) {
    this._hashDefined = value;
  }

  /**
   * Gets the hash defined value for a given expr enum
   * @return the hash defined value for a given expr enum
   */
  public long getHashDefinedValue() {
    return _hashDefined;
  }

}
