/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers.other;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExprVariadicReturn;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.nodes.SELECTRESULT;

/**
 * Holds the result of a computation.
 */
public class OGResult {

  protected OGExprVariadicReturn _computeNode;
  private int _nResults;

  /**
   * Construct a new OGResult
   * @param computeNode the compute node to which this result refers
   */
  public OGResult(OGExprVariadicReturn computeNode) {
    Catchers.catchNullFromArgList(computeNode, 1);
    _computeNode = computeNode;
    _nResults = _computeNode.getNReturnItems();
  }

  /**
   * Get a particular result
   * @param index the index of the result to get
   * @return the result at location "index"
   */
  public OGNumeric get(int index) {
    Catchers.catchCondition(index < 0, "index must be >=0");
    Catchers.catchCondition(index >= _nResults, "index must be < " + _nResults);
    return new SELECTRESULT(_computeNode, index);
  }

}
