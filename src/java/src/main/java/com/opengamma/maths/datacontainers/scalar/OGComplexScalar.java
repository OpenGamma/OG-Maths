/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.scalar;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;

/**
 * Complex Scalar Numeric class.
 */
public class OGComplexScalar extends OGScalar {

  private double[] _data = new double[2];

  /** 
   * Define complex <i>i</i>
   */
  public static final OGComplexScalar I = new OGComplexScalar(0, 1);
  /**
   * Define complex <i>-i</i>
   */
  public static final OGComplexScalar NEGATIVE_I = new OGComplexScalar(0, -1);
  /**
   * Define complex 0 + 0<i>i</i>
   */
  public static final OGComplexScalar ZERO = new OGComplexScalar(0);

  private static ExprEnum s_type = ExprEnum.OGComplexScalar;

  /**
   * Construct from real only, imaginary set to 0
   * @param real the real part
   */
  public OGComplexScalar(Number real) {
    Catchers.catchNullFromArgList(real, 1);
    _data[0] = real.doubleValue();
    _data[1] = 0.d;
  }

  /**
   * Construct from real and imaginary parts
   * @param real the real part
   * @param imag the imaginary part
   */
  public OGComplexScalar(Number real, Number imag) {
    Catchers.catchNullFromArgList(real, 1);
    Catchers.catchNullFromArgList(imag, 2);
    _data[0] = real.doubleValue();
    _data[1] = imag.doubleValue();
  }

  /**
   * Construct from ordered double[] of length 2, real is at address [0] and imaginary is at address[1]
   * @param complex real part is at address [0] and imaginary part is at address[1]
   */
  public OGComplexScalar(double[] complex) {
    Catchers.catchNullFromArgList(complex, 1);
    if (complex.length != 2) {
      throw new MathsExceptionIllegalArgument("Assignment from a double[] to OGComplexScalar can only be performed from a double[] of length 2");
    }
    _data[0] = complex[0];
    _data[1] = complex[1];
  }

  /**
   * Get the real component of the number
   * @return the real component of the number
   */
  public double getReal() {
    return _data[0];
  }

  /**
   * Get the real component of the number
   * @return the real component of the number
   */
  public double getImag() {
    return _data[1];
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
    String str = "\nOGComplexScalar: value = " + String.format("%24.18f " + (_data[1] >= 0 ? "+" : "-") + "%24.18fi, ", _data[0], Math.abs(_data[1]));
    return str;
  }

  @Override
  protected OGComplexDenseMatrix asOGComplexDenseMatrix() {
    return new OGComplexDenseMatrix(this.getData(), 1, 1);
  }
}
