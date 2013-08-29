/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers;


/**
 * Concrete numeric type base class
 */
public abstract class OGArray implements OGNumeric {

  /** 
   * Holds the data
   */
  private double[] _data;

  /**
   * Holds the number of rows
   */
  private int _rows;

  /**
   * Holds the number of columns
   */
  private int _columns;
 
  /**
   * Returns the data
   * @return the data
   */
  public double[] getData() {
    return _data;
  }

  /**
   * Gets the rows.
   * @return the rows
   */
  public int getRows() {
    return _rows;
  }

  /**
   * Gets the columns.
   * @return the columns
   */
  public int getCols() {
    return _columns;
  }



}
