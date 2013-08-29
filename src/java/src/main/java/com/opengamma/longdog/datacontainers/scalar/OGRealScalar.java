/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers.scalar;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.helpers.Catchers;
import com.opengamma.longdog.helpers.MathsException;

/**
 * Real Scalar Numeric class
 */
public class OGRealScalar extends OGScalar {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGRealScalar;

  private double[] _data = new double[1];

  /**
   * @param n a number
   */
  public OGRealScalar(Number n) {
    Catchers.catchNullFromArgList(n, 1);
    try {
      _data[0] = n.doubleValue();
    } catch (Exception e) {
      throw new MathsException("Cannot construct real scalar from type that has no doublevalue");
    }
  }

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }

}
