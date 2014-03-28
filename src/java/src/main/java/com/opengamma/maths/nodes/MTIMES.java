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
 * MTimes class
 */
public class MTIMES extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.MTIMES_ENUM;
  }

  public MTIMES(OGNumeric arg0, OGNumeric arg1) {
    super(arg0, arg1);
  }


}
