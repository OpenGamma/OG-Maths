/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import com.opengamma.longdog.datacontainers.OGArray;

/**
 * Sparse matrix super type
 */
public abstract class OGSparseMatrix extends OGArray {
  private int[] _colPtr;
  private int[] _rowIdx;

  /**
   * Gets the colPtr.
   * @return the colPtr
   */
  public int[] getColPtr() {
    return _colPtr;
  }

  /**
   * Gets the rowIdx.
   * @return the rowIdx
   */
  public int[] getRowIdx() {
    return _rowIdx;
  }
}
