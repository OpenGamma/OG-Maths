/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.other;

import java.util.Arrays;

import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.helpers.MatrixPrimitiveUtils;

/**
 * Holds a complex array.
 */
public class ComplexArrayContainer {

  private double[][] _real;
  private double[][] _imag;
  private boolean _anyImag;

  /**
   * Construct a new complex array container.
   * @param real the real part.
   * @param imag the imaginary part.
   */
  public ComplexArrayContainer(double[][] real, double[][] imag) {
    Catchers.catchNullFromArgList(real, 1);
    Catchers.catchNullFromArgList(imag, 2);
    Catchers.catchCondition(MatrixPrimitiveUtils.isRagged(real), "Array data cannot be ragged.");
    Catchers.catchCondition(MatrixPrimitiveUtils.isRagged(imag), "Array data cannot be ragged.");
    int rows = real.length;
    int cols = real[0].length;
    Catchers.catchCondition(rows != imag.length, "Real and Imag parts have different numbers of rows");
    Catchers.catchCondition(cols != imag[0].length, "Real and Imag parts have different numbers of columns");

    OUTER:
    //CSIGNORE
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if ((Double.doubleToRawLongBits(imag[i][j]) & 0x7ffffffffffffffL) != 0L) {
          _anyImag = true;
          break OUTER;
        }
      }
    }
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

  /**
   * Returns true if any part of the imaginary data is nonzero
   * @return true if any part of the imaginary data is nonzero
   */
  public boolean anyImaginary() {
    return _anyImag;
  }

  @Override
  public String toString() {
    String str = "ComplexArrayContainer:";
    str = str + "\n====Pretty Print====\n";
    int rows = _real.length;
    int cols = _real[0].length;
    str = str + "Rows: " + rows + "Cols: " + cols + "\n";
    str = str + "Data:\n";
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        str += String.format("%24.18f + %24.18fi, ", _real[i][j], _imag[i][j]);
      }
      str += String.format("\n");
    }
    str += "\n";
    return str;
  }

}
