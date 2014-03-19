#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

exprenum_java = """\
/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 *
 * This file is generated during the DOGMA2 build process: src/generator/generator.py
 */

package com.opengamma.longdog.datacontainers;

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
  COPY_ENUM         (0x010DL),
  SELECTRESULT_ENUM (0x010FL),
  NORM2_ENUM        (0x0115L),
  SVD_ENUM          (0x0119L),

  // Unary expression nodes - start at 175 to leave room for extra non-generated nodes
%(generated_nodes)s;

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

}"""
