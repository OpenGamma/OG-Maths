/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import java.util.Arrays;

import com.opengamma.longdog.datacontainers.OGArray;

/**
 * Diagonal matrix super type
 */
public abstract class OGDiagonalMatrix extends OGArray {

  
//  @Override
//  public String toString() {
//    String str = "OGDiagonalMatrix:" + "\ndata = " + Arrays.toString(_data) + "\nrows = " + _rows + "\ncols = " + _columns;
//    str = str + "\n====Pretty Print====\n";
//    String zeroStr = String.format("%24.18f ", 0.d);
//    for (int i = 0; i < _rows; i++) {
//      for (int j = 0; j < _cols; j++) {
//        if (i == j && i < _data.length && j < _data.length) {
//          str += String.format("%24.18f ", _data[i]);
//        } else {
//          str += zeroStr;
//        }
//      }
//      str += String.format("\n");
//    }
//    return str;
//  }

}
