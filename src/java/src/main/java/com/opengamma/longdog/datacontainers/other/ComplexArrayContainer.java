/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers.other;

/**
 * Holds a complex array.
 */
public class ComplexArrayContainer {

  private double[][] _real;
  private double[][] _imag;

  /**
   * Construct a new complex array container.
   * @param real the real part.
   * @param imag the imaginary part.
   */
  public ComplexArrayContainer(double[][] real, double[][] imag) {
    _real = real;
    _imag = imag;
  }

  /**
   * Gets the real part.
   * @return the real part
   */
  public double[][] getReal() {
    return _real;
  }

  /**
   * Gets the imaginary part.
   * @return the imaginary part
   */
  public double[][] getImag() {
    return _imag;
  }

}
