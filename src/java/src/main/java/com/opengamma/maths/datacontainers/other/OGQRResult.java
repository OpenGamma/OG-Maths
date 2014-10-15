/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.other;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExprVariadicReturn;

/**
 * Holds the result of a QR Decomposition computation.
 */
public class OGQRResult {

  /**
   * Holds the result reference.
   */
  private OGResult _result;

  /**
   * Create a QR decomposition of a matrix.
   * The matrix, 'A', is decomposed as 'A=Q*R'.
   * Where 'Q' is an orthogonal matrix
   * and 'R' is an upper triangular matrix.
   * @param computeNode the data on which the
   * QR decomposition shall be performed.
   */
  public OGQRResult(final OGExprVariadicReturn computeNode) {
    _result = new OGResult(computeNode);
  }

  /**
   * Gets Q, the orthogonal matrix.
   * @return the orthogonal matrix Q.
   */
  public final OGNumeric getQ() {
    return this._result.get(0);
  }

  /**
   * Gets R, the upper triangular matrix.
   * @return the upper triangular matrix, R.
   */
  public final OGNumeric getR() {
    return this._result.get(1);
  }

}
