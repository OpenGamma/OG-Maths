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

 
  /*
   * expr tokens for functions
   */  
  
  PLUS         (0x0101L),
  NEGATE       (0x0107L),
  TIMES        (0x010DL),
  LDIVIDE      (0x010FL),
  MTIMES       (0x0115L),
  COPY         (0x0119L),
  SELECTRESULT (0x011BL),
  SVD          (0x0125L);

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
