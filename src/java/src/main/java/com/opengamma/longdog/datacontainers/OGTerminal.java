/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers;

/**
 * Holds a terminal node in the AST (OGArray or OGScalar)
 */
public abstract class OGTerminal implements OGNumeric {

  /**
   * Gets the data held in the OGTerminal node
   * @return the data
   */
  public abstract double[] getData();

}
