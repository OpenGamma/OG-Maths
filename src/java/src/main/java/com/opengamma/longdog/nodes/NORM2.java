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
 * Norm2 class
 */
public class NORM2 extends OGExpr {

  @Override
  public ExprTypeEnum getType() {
    return ExprTypeEnum.NORM2;
  }

  public NORM2(OGNumeric arg) {
    super(arg);
  }


}
