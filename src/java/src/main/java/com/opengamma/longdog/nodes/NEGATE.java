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
 * Negate class
 */
public class NEGATE extends OGExpr {

  @Override
  public ExprTypeEnum getType() {
    return ExprTypeEnum.NEGATE;
  }

  public NEGATE(OGNumeric arg) {
    super(arg);
  }


}
