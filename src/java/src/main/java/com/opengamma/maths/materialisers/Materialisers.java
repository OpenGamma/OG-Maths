/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.materialisers;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.matrix.OGArray;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.datacontainers.scalar.OGScalar;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.nativeloader.NativeLibraries;
import com.opengamma.maths.nodes.SELECTRESULT;

/**
 * Contains functions that materialise DOGMA ASTs.
 */
public class Materialisers {

  /**
   * Load the native libraries
   */
  static {
    NativeLibraries.initialize();
  }

  /* native library bindings */
  private static native double[][] materialiseToJDoubleArrayOfArrays(OGNumeric arg0);

  private static native ComplexArrayContainer materialiseToJComplexArrayContainer(OGNumeric arg0);

  private static native boolean materialiseToJBoolean(OGNumeric arg0);

  private static native OGTerminal materialiseToOGTerminal(OGNumeric arg0);

  /**
   * Materialise the tree at arg0 to a complex array stored in a ComplexArrayContainer.
   * @param arg0 the root of the tree to materialise.
   * @return a ComplexArrayContainer of the materialised tree.
   */
  public static double[][] toDoubleArrayOfArrays(OGNumeric arg0) {
    Catchers.catchNullFromArgList(arg0, 1);
    return materialiseToJDoubleArrayOfArrays(arg0);
  }

  /**
   * Materialise the tree at arg0 to a complex array stored in a ComplexArrayContainer.
   * @param arg0 the root of the tree to materialise.
   * @return a ComplexArrayContainer of the materialised tree.
   */
  public static ComplexArrayContainer toComplexArrayContainer(OGNumeric arg0) {
    Catchers.catchNullFromArgList(arg0, 1);
    return materialiseToJComplexArrayContainer(arg0);
  }

  /**
   * Materialise the tree at arg0 to a complex array stored in a ComplexArrayContainer.
   * @param arg0 the root of the tree to materialise.
   * @return a ComplexArrayContainer of the materialised tree.
   */
  public static boolean toBoolean(OGNumeric arg0) {
    Catchers.catchNullFromArgList(arg0, 1);
    return materialiseToJBoolean(arg0);
  }

  /**
   * Naively print the tree associated with arg
   * @param arg the root of the tree
   * @param buf a buffer in which to hold the printed tree
   * @param level the level at which to start indents (usually 0)
   */
  public static void printTree(OGNumeric arg, StringBuffer buf, int level) {
    Catchers.catchNullFromArgList(arg, 1);
    Catchers.catchNullFromArgList(buf, 2);
    Catchers.catchCondition(level < 0, "Level must be >= 0");
    final String tab = "   ";
    level++;
    while (!(OGArray.class.isAssignableFrom(arg.getClass()) || OGScalar.class.isAssignableFrom(arg.getClass()))) {
      OGExpr expr = (OGExpr) arg;
      buf.append(new String(new char[level]).replace("\0", tab) + arg.getClass());
      buf.append("\n");
      for (int i = 0; i < expr.getExprs().length; i++) {
        printTree(expr.getExprs()[i], buf, level);
      }
      break;
    }
    if ((OGArray.class.isAssignableFrom(arg.getClass()))) {
      buf.append(new String(new char[level]).replace("\0", tab) + arg.getClass() + "\n");
    }
    level--;
  }

  public static OGTerminal toOGTerminal(OGNumeric arg0) {
    Catchers.catchNullFromArgList(arg0, 1);
    return materialiseToOGTerminal(arg0);
  }
}
