/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers.scalar;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.Catchers;

/**
 * Real Scalar Numeric class.
 */
public class OGIntegerScalar extends OGScalar {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGIntegerScalar;

  private int[] _data = new int[1];

  /**
   * @param n a number
   */
  public OGIntegerScalar(Number n) {
    Catchers.catchNullFromArgList(n, 1);
    try {
      _data[0] = n.intValue();
    } catch (Exception e) {
      throw new MathsExceptionIllegalArgument("Cannot construct integer scalar from type that has no integer value");
    }
  }

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }

  /**
   * FIXME: getData should not always return double, probably.
   */
  @Override
  public double[] getData() {
    return new double[] { _data[0] };
  }

  public int getValue() {
    return _data[0];
  }

  @Override
  public String toString() {
    String str = "\nOGIntegerScalar: value = " + String.format("%d\n", _data[0]);
    return str;
  }

}
