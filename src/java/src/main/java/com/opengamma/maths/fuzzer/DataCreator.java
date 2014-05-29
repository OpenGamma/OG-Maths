/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.fuzzer;

import java.util.Random;

import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGComplexDiagonalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGComplexSparseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealSparseMatrix;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;

/**
 * A class that creates OGTerminal data types.
 */
public class DataCreator {
  
  /**
   * The maximum size of the data [fmax(rows,cols)].
   */
  private int _maxsize;

  /**
   * The RNG.
   */
  private Random _rand;

  /**
   * Creates a new DataCreator.
   * @param maxDataSize the maximum dimension of a matrix
   * @param seed the random seed used in creating the matrices
   */
  public DataCreator(int maxDataSize, long seed) {
    _maxsize = maxDataSize;
    _rand = new Random(seed);
  }

  /**
   * Gets an array of random doubles of a given length.
   * @param length the number of double required
   * @return a new array of random doubles of length {@code length}
   */
  private double[] getRandomData(int length) {
    double[] data = new double[length];
    for (int i = 0; i < length; i++) {
      if (_rand.nextBoolean()) {
        data[i] = _rand.nextInt();
      } else {
        data[i] = -_rand.nextInt();
      }
    }
    return data;
  }

  /**
   * Gets an array of arrays of random doubles with many zeros present, of a given size.
   * @param rows the number of rows in the data.
   * @param cols the number of columns in the data.
   * @return a new array of random doubles of length {@code length}
   */
  private double[][] getRandomSparseData(int rows, int cols) {
    double[][] data = new double[rows][cols];
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (_rand.nextBoolean()) {
          if (_rand.nextBoolean()) {
            data[i][j] = _rand.nextInt();
          } else {
            data[i][j] = -_rand.nextInt();
          }
        } else {
          data[i][j] = 0;
        }
      }
    }
    data[0][0] = 1; // set this to prevent entirely empty matrices
    return data;
  }

  /**
   * Next int, will be > 0
   * @return
   */
  private int intGt0() {
    return _rand.nextInt(_maxsize) + 1;
  }

  /**
   * Gets a "random" OGTerminal
   * @return an OGTerminal
   */
  OGTerminal getAnOGTerminal() {
    int lrows, lcols;

    //  largely create square things
    if (_rand.nextInt() % 3 != 0) {
      lrows = intGt0();
      lcols = lrows;
    } else {
      // sometimes create random rows and cols
      lrows = intGt0();
      lcols = intGt0();
    }

    final int typecount = 8;
    switch (_rand.nextInt(typecount)) {
      case 0:
        return new OGRealScalar(_rand.nextDouble());
      case 1:
        return new OGComplexScalar(_rand.nextDouble(), _rand.nextDouble());
      case 2:
        return new OGRealDenseMatrix(getRandomData(lrows * lcols), lrows, lcols);
      case 3:
        return new OGComplexDenseMatrix(getRandomData(2 * lrows * lcols), lrows, lcols);
      case 4:
        return new OGRealDiagonalMatrix(getRandomData(Math.min(lcols, lrows)), lrows, lcols);
      case 5:
        return new OGComplexDiagonalMatrix(getRandomData(2 * Math.min(lcols, lrows)), lrows, lcols);
      case 6:
        return new OGRealSparseMatrix(getRandomSparseData(lrows, lcols));
      case 7:
        return new OGComplexSparseMatrix(getRandomSparseData(lrows, lcols), getRandomSparseData(lrows, lcols));
      default:
        throw new MathsExceptionIllegalArgument("Unknown value to type switch, value was " + typecount);
    }
  }
}
