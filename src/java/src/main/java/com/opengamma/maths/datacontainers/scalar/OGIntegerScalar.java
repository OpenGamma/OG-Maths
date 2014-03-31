/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.scalar;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;

/**
 * Real Scalar Numeric class.
 */
public class OGIntegerScalar extends OGScalar {
  private static ExprEnum s_type = ExprEnum.OGIntegerScalar;

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
  public ExprEnum getType() {
    return s_type;
  }

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

  @Override
  protected OGComplexDenseMatrix asOGComplexDenseMatrix() {
    return new OGComplexDenseMatrix(new double[][] { { this.getData()[0] } });
  }

  @Override
  protected OGRealDenseMatrix asOGRealDenseMatrix() {
    return new OGRealDenseMatrix(new double[][] { { this.getData()[0] } });
  }
}
