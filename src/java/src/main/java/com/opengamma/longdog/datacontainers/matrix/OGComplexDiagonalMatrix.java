/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;

/**
 * Complex Diagonal matrix type
 */
public class OGComplexDiagonalMatrix extends OGDiagonalMatrix {

  private static ExprTypeEnum s_type = ExprTypeEnum.OGComplexDiagonalMatrix;

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }
  
//  @Override
//  public String toString() {
//    String str = "OGComplexDiagonalMatrix:" + "\ndata = " + Arrays.toString(_data) + "\nrows = " + _rows + "\ncols = " + _columns;
//    str = str + "\n====Pretty Print====\n";
//    String zeroStr = String.format("%24.18f + %24.18fi ", 0.d, 0.d);
//    double imag;
//    for (int i = 0; i < 2 * _rows; i += 2) {
//      for (int j = 0; j < 2 * _columns; j += 2) {
//        if (i == j && i < _data.length && j < _data.length) {
//          imag = _data[i + 1];
//          str += String.format("%24.18f " + (imag >= 0 ? "+" : "-") + "%24.18fi, ", _data[i], Math.abs(imag));
//        } else {
//          str += zeroStr;
//        }
//      }
//      str += String.format("\n");
//    }
//    return str;
//  }


}
