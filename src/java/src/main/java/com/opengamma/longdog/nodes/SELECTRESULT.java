/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.nodes;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.lazy.OGExpr;

/**
 * Select result class.
 */
public class SELECTRESULT extends OGExpr {

  private int _entry;

  @Override
  public ExprTypeEnum getType() {
    return ExprTypeEnum.SELECTRESULT;
  }

  public SELECTRESULT(OGNumeric arg0, int index) {
    super(arg0);
    _entry = index;
  }

  /**
   * Gets the entry.
   * @return the entry
   */
  public int getEntry() {
    return _entry;
  }

}
