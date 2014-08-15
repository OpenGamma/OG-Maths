/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.scalar;

import java.util.Arrays;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
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
  public String toDebugString() {
    String str = "\nOGRealScalar: value = " + String.format("%24.18f\n", _data[0]);
    return str;
  }

  @Override
  public String toString() {
    return String.format("%24.18f\n", _data[0]);
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
