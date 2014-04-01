/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.helpers.MatrixPrimitiveUtils;

/**
 * Logical matrix
 */
public class OGLogicalMatrix extends OGRealDenseMatrix {

  private static ExprEnum s_type = ExprEnum.OGLogicalMatrix;

  public OGLogicalMatrix(double[] data, int rows, int cols) {
    super(array1d2bool(data), rows, cols);
  }

  public OGLogicalMatrix(double[][] data) {
    super(array2d2bool(data));
  }

  @Override
  public ExprEnum getType() {
    return s_type;
  }

  private static double[] array1d2bool(double[] data) {
    Catchers.catchNullFromArgList(data, 1);
    double[] tmp = new double[data.length];
    for (int i = 0; i < data.length; i++) {
      if (data[i] != 0) {
        tmp[i] = 1;
      }
    }
    return tmp;
  }

  private static double[][] array2d2bool(double[][] data) {
    Catchers.catchNullFromArgList(data, 1);
    if (MatrixPrimitiveUtils.isRagged(data)) {
      throw new MathsExceptionIllegalArgument("Cannot construct from ragged data");
    }
    double[][] tmp = new double[data.length][data[0].length];
    for (int i = 0; i < data.length; i++) {
      for (int j = 0; j < data[i].length; j++) {
        if (data[i][j] != 0) {
          tmp[i][j] = 1;
        }
      }
    }
    return tmp;
  }
}
