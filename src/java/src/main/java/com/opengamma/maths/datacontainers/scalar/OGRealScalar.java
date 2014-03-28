/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.scalar;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;

/**
 * Real Scalar Numeric class.
 */
public class OGRealScalar extends OGScalar {
  private static ExprEnum s_type = ExprEnum.OGRealScalar;

  private double[] _data = new double[1];

  /**
   * @param n a number
   */
  public OGRealScalar(Number n) {
    Catchers.catchNullFromArgList(n, 1);
    try {
      _data[0] = n.doubleValue();
    } catch (Exception e) {
      throw new MathsExceptionIllegalArgument("Cannot construct real scalar from type that has no doublevalue");
    }
  }

  @Override
  public ExprEnum getType() {
    return s_type;
  }

  @Override
  public double[] getData() {
    return _data;
  }

  @Override
  public String toString() {
    String str = "\nOGRealScalar: value = " + String.format("%24.18f\n", _data[0]);
    return str;
  }

}
