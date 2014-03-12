/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.nodes;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.lazy.OGExpr;

/**
 * MTimes class
 */
public class MTIMES extends OGExpr {

  @Override
  public ExprTypeEnum getType() {
    return ExprTypeEnum.MTIMES;
  }

  public MTIMES(OGNumeric arg0, OGNumeric arg1) {
    super(arg0, arg1);
  }


}
