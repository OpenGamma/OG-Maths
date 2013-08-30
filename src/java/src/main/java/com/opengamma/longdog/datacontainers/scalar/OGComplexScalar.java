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
 * Complex Scalar Numeric class
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

  private static ExprTypeEnum s_type = ExprTypeEnum.OGComplexScalar;

  /**
   * Construct from real only, imaginary set to 0
   * @param real the real part
   */
  public OGComplexScalar(double real) {
    _data[0] = real;
    _data[1] = 0.d;
  }

  /**
   * Construct from real and imaginary parts
   * @param real the real part
   * @param imag the imaginary part
   */
  public OGComplexScalar(double real, double imag) {
    _data[0] = real;
    _data[1] = imag;
  }

  /**
   * Construct from ordered double[] of length 2, real is at address [0] and imaginary is at address[1]
   * @param complex real part is at address [0] and imaginary part is at address[1]
   */
  public OGComplexScalar(double[] complex) {
    Catchers.catchNullFromArgList(complex, 1);
    if (complex.length != 2) {
      throw new MathsException("Assignment from a double[] to OGComplexScalar can only be performed from a double[] of length 2");
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
  public ExprTypeEnum getType() {
    return s_type;
  }

  @Override
  public double[] getData() {
    return _data;
  }

}
