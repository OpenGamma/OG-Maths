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
   * expr tokens for assignment
   */
  SelectResult0 (0x0010L),
  SelectResult1 (0x0020L),
  SelectResult2 (0x0030L),
  SelectResult3 (0x0040L),
  SelectResult4 (0x0050L),
  SelectResult5 (0x0060L),
  SelectResult6 (0x0070L),
  SelectResult7 (0x0080L),
  SelectResult8 (0x0090L),
  SelectResult9 (0x00A0L),
  SelectResult10(0x00B0L),
  SelectResult11(0x00C0L),
  SelectResult12(0x00D0L),
  SelectResult13(0x00E0L),
  SelectResult14(0x00F0L),
  
  /*
   * expr tokens for functions
   */  
  
  PLUS   (0x0100L),
  MINUS  (0x0200L),
  TIMES  (0x0200L),
  LDIVIDE(0x0300L),
  MTIMES (0x0500L),
  COPY   (0x0600L);  

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
