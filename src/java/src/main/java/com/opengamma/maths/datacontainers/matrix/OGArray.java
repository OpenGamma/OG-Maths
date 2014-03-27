/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import com.opengamma.maths.datacontainers.OGTerminal;

/**
 * Array base class from which all arrays inherit
 */
public abstract class OGArray extends OGTerminal {

  /**
   * Gets the rows.
   * @return the rows
   */
  public abstract int getRows();

  /**
   * Gets the columns.
   * @return the columns
   */
  public abstract int getCols();

}
