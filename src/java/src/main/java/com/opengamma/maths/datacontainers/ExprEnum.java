/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 *
 * This file is generated during the DOGMA2 build process: src/generator/generator.py
 */

package com.opengamma.maths.datacontainers;

/**
 * Enumerate expr types.
 */
public enum ExprEnum {

  //@formatter:off
  //CSOFF
  UNKNOWNEXPR(0x0000L),

  /*
   * Data container tokens
   */
  OGRealScalar            (0x0002L),
  OGComplexScalar         (0x0003L),
  OGRealDenseMatrix       (0x0005L),
  OGComplexDenseMatrix         (0x0007L),
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
  COPY_ENUM         (0x010DL),
  SELECTRESULT_ENUM (0x010FL),
  NORM2_ENUM        (0x0115L),
  SVD_ENUM          (0x0119L),
  MTIMES_ENUM       (0x011BL),
  PINV_ENUM         (0x0125L),
  TRANSPOSE_ENUM    (0x0133L),
  CTRANSPOSE_ENUM   (0x0137L),
  LU_ENUM           (0x0139L),
  INV_ENUM          (0x013DL),
  MLDIVIDE_ENUM     (0x014BL),
  QR_ENUM           (0x0151L),
  SUMCOLS_ENUM      (0x015BL),
  SUMROWS_ENUM      (0x015DL),


  // Unary expression nodes - start at 175 to leave room for extra non-generated nodes
  ABS_ENUM (0X0175L),
  ACOS_ENUM (0X017BL),
  ACOSH_ENUM (0X017FL),
  ANGLE_ENUM (0X0185L),
  ASIN_ENUM (0X018DL),
  ASINH_ENUM (0X0191L),
  ATAN_ENUM (0X0199L),
  ATANH_ENUM (0X01A3L),
  CONJ_ENUM (0X01A5L),
  COS_ENUM (0X01AFL),
  COSH_ENUM (0X01B1L),
  DIAG_ENUM (0X01B7L),
  DOT_ENUM (0X01BBL),
  EIG_ENUM (0X01C1L),
  ERF_ENUM (0X01C9L),
  ERFC_ENUM (0X01CDL),
  EXP_ENUM (0X01CFL),
  EYE_ENUM (0X01D3L),
  FLIPLR_ENUM (0X01DFL),
  FLIPUD_ENUM (0X01E7L),
  HILB_ENUM (0X01EBL),
  HORZCAT_ENUM (0X01F3L),
  IMAG_ENUM (0X01F7L),
  INVHILB_ENUM (0X01FDL),
  LOG_ENUM (0X0209L),
  MINUS_ENUM (0X020BL),
  NEGATE_ENUM (0X021DL),
  NORMCDF_ENUM (0X0223L),
  POWER_ENUM (0X022DL),
  PLUS_ENUM (0X0233L),
  RDIVIDE_ENUM (0X0239L),
  REAL_ENUM (0X023BL),
  ROUND_ENUM (0X0241L),
  SIN_ENUM (0X024BL),
  SINH_ENUM (0X0251L),
  SQRT_ENUM (0X0257L),
  TAN_ENUM (0X0259L),
  TANH_ENUM (0X025FL),
  TIMES_ENUM (0X0265L),
  VERTCAT_ENUM (0X0269L),
  WILKINSON_ENUM (0X026BL);

  //CSON
  //@formatter:on

  private long _hashDefined;

  /**
   * Construct a new expr enum
   */
  ExprEnum(long value) {
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