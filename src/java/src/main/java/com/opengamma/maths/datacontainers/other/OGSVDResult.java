/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.other;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExprVariadicReturn;

/**
 * Holds the result of a Singular Value Decomposition computation.
 */
public class OGSVDResult {

  /**
   * Holds the result reference.
   */
  private OGResult _result;

  /**
   * Create a Singular value decomposition of some data.
   * @param computeNode the data on which the 
   * Singular Value decomposition shall be performed.
   */
  public OGSVDResult(final OGExprVariadicReturn computeNode) {
    _result = new OGResult(computeNode);
  }

  /**
   * Gets U, the matrix of left singular vectors.
   * @return the matrix of left singular vectors.
   */
  public final OGNumeric getU() {
    return this._result.get(0);
  }

  /**
   * Gets S a diagonal matrix containing the singular
   * values returned in descending order.
   * @return a diagonal matrix containing the singular
   * values in descending order.
   */
  public final OGNumeric getS() {
    return this._result.get(1);
  }

  /**
   * Gets V**T, the matrix of the right singular vectors _transposed_.
   * @return the matrix of the right singular vectors _transposed_.
   */
  public final OGNumeric getVT() {
    return this._result.get(2);
  }

}
