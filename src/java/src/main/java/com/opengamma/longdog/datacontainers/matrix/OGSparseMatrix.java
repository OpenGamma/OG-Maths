/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;


/**
 * Sparse matrix super type
 */
public abstract class OGSparseMatrix extends OGArray {

  /**
   * Gets the colPtr.
   * @return the colPtr
   */
  public abstract int[] getColPtr();

  /**
   * Gets the rowIdx.
   * @return the rowIdx
   */
  public abstract int[] getRowIdx();
}
