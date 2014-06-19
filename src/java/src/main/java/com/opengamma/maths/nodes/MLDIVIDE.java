/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;

/**
 * Mldivide class
 */
public class MLDIVIDE extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.MLDIVIDE_ENUM;
  }

  public MLDIVIDE(OGNumeric arg0, OGNumeric arg1) {
    super(arg0, arg1);
  }


}
