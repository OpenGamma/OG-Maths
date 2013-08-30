/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.helpers.Catchers;
import com.opengamma.longdog.helpers.MatrixPrimitiveUtils;

/**
 * Logical matrix
 */
public class OGLogicalMatrix extends OGRealDenseMatrix {

  private static ExprTypeEnum s_type = ExprTypeEnum.OGLogicalMatrix;

  public OGLogicalMatrix(double[] data, int rows, int cols) {
    super(array1d2bool(data), rows, cols);
  }

  public OGLogicalMatrix(double[][] data) {
    super(array2d2bool(data));
  }

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }

  private static double[] array1d2bool(double[] data) {
    Catchers.catchNull(data);
    double[] tmp = new double[data.length];
    for (int i = 0; i < data.length; i++) {
      if (data[i] != 0) {
        tmp[i] = 1;
      }
    }
    return tmp;
  }

  private static double[][] array2d2bool(double[][] data) {
    Catchers.catchNull(data);
    Catchers.catchCondition(MatrixPrimitiveUtils.isRagged(data), "Cannot construct from ragged data");
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
