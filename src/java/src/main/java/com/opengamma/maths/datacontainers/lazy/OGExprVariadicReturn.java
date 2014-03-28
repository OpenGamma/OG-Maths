/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.lazy;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.helpers.Catchers;

/**
 * Holds a late evaluated Expr that is used for a node which has a variadic return.
 */
public abstract class OGExprVariadicReturn extends OGExpr {

  private int _nReturnItems;

  /**
   * Construct expr with fixed number of return items
   * @param arg0 the argument for the node
   * @param nReturnItems the number of items this node can return
   */
  public OGExprVariadicReturn(OGNumeric arg0, int nReturnItems) {
    super(arg0);
    Catchers.catchCondition(nReturnItems < 0, "nReturnItems must be >=0");
    _nReturnItems = nReturnItems;
  }

  /**
   * Gets the nReturnItems.
   * @return the nReturnItems
   */
  public int getNReturnItems() {
    return _nReturnItems;
  }

  @Override
  public String toString() {
    return this.getClass().getName();
  }

}
