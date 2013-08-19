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
 * Plus class
 */
public class MINUS extends OGExpr {

  @Override
  public ExprTypeEnum getType() {
    return ExprTypeEnum.MINUS;
  }

  public MINUS(OGNumeric arg0, OGNumeric arg1) {
    super(arg0, arg1);
  }


}
