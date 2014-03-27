/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.OGNumeric;

/**
 * A expr node
 */
public interface NODE extends OGNumeric {

  OGNumeric[] getExprs();
  
  int getNExprs();

}
