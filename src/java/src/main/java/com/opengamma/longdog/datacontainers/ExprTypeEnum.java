/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers;

/**
 * Enumerate expr types
 */
public enum ExprTypeEnum {

  //@formatter:off
  //CSOFF
  UNKNOWNEXPR(0x0000L),
  
  /*
   * Data container tokens 
   */
  OGRealScalar            (0x0001L),
  OGComplexScalar         (0x0002L),
  OGRealMatrix            (0x0003L),
  OGComplexMatrix         (0x0004L),
  OGRealSparseMatrix      (0x0005L),
  OGComplexSparseMatrix   (0x0006L),
  OGRealDiagonalMatrix    (0x0007L),
  OGComplexDiagonalMatrix (0x0008L),
  OGLogicalMatrix         (0x0009L),

 
  /*
   * expr tokens for functions
   */  
  
  PLUS         (0x0100L),
  MINUS        (0x0200L),
  TIMES        (0x0300L),
  LDIVIDE      (0x0400L),
  MTIMES       (0x0500L),
  COPY         (0x0600L),
  SELECTRESULT (0x0700L),
  SVD          (0x0800L);

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
