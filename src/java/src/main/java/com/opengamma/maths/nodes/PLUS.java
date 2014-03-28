/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;

/**
 * Plus class
 */
public class PLUS extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.PLUS_ENUM;
  }

  public PLUS(OGNumeric arg0, OGNumeric arg1) {
    super(arg0, arg1);
  }


}
