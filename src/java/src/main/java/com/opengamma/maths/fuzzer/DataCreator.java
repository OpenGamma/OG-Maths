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
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;

public class DataCreator {
  private int _maxsize;
  private Random _rand;
  private int _rows;
  private int _cols;

  /**
   * Creates a new DataCreato_rand 
   * @param maxDataSize the maximum dimension of a matrix
   * @param seed the random seed used in creating the matrices
   */
  public DataCreator(int maxDataSize, long seed) {
    _maxsize = maxDataSize;
    _rand = new Random(seed);
    _rows = _rand.nextInt(_maxsize);
    _cols = _rows;
  }

  /**
   * Gets an array of random doubles of a given length
   * @param length the number of double required
   * @return a new array of random doubles of length {@code length}
   */
  private double[] getRandomData(int length) {
    double[] data = new double[length];
    for (int i = 0; i < length; i++) {
      data[i] = _rand.nextDouble();
    }
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
    if (_rand.nextInt() % 10 == 0) {
      lrows = intGt0();
      lcols = lrows;
    } else {
      // sometimes create random rows and cols
      lrows = intGt0();
      lcols = intGt0();
    }

    final int typecount = 6;
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
        return new OGRealDiagonalMatrix(getRandomData(Math.min(lcols,lrows)), lrows, lcols);
      case 5:
        return new OGComplexDiagonalMatrix(getRandomData(2 * Math.min(lcols,lrows)), lrows, lcols);
      default:
        throw new MathsExceptionIllegalArgument("Unknown value to type switch, value was " + typecount);
    }
  }
}
