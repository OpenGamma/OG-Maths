/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.datacontainers.other.OGLUResult;
import com.opengamma.maths.datacontainers.other.OGSVDResult;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nativeloader.NativeLibraries;
import com.opengamma.maths.nodes.ACOS;
import com.opengamma.maths.nodes.ASINH;
import com.opengamma.maths.nodes.ATAN;
import com.opengamma.maths.nodes.COS;
import com.opengamma.maths.nodes.CTRANSPOSE;
import com.opengamma.maths.nodes.EXP;
import com.opengamma.maths.nodes.INV;
import com.opengamma.maths.nodes.LU;
import com.opengamma.maths.nodes.MLDIVIDE;
import com.opengamma.maths.nodes.MTIMES;
import com.opengamma.maths.nodes.NEGATE;
import com.opengamma.maths.nodes.NORM2;
import com.opengamma.maths.nodes.PINV;
import com.opengamma.maths.nodes.PLUS;
import com.opengamma.maths.nodes.SVD;
import com.opengamma.maths.nodes.TRANSPOSE;

/**
 * DOGMA the OpenGamma maths assembler.
 * <p>
 * DOGMA is a set of simple functions that operate on OGNumeric
 * classes in a vectorised manner.
 * </p>
 */
public final class DOGMA {

  /**
   * Default ctor is hidden.
   */
  private DOGMA() {
  };

  static {
    NativeLibraries.initialize();
  }

  // Type from Number
  public static OGTerminal D(Number num) {
    return new OGRealScalar(num);
  }

  public static OGTerminal C(Number real) {
    return new OGComplexScalar(real);
  }

  public static OGTerminal C(Number real, Number imag) {
    return new OGComplexScalar(real, imag);
  }

  // unary nodes

  public static OGNumeric acos(OGNumeric arg0) {
    return new ACOS(arg0);
  }

  public static OGNumeric asinh(OGNumeric arg0) {
    return new ASINH(arg0);
  }
  
  public static OGNumeric atan(OGNumeric arg0) {
    return new ATAN(arg0);
  }

  public static OGNumeric cos(OGNumeric arg0) {
    return new COS(arg0);
  }

  public static OGNumeric ctranspose(OGNumeric arg0) {
    return new CTRANSPOSE(arg0);
  }

  public static OGNumeric exp(OGNumeric arg0) {
    return new EXP(arg0);
  }
  
  public static OGNumeric inv(OGNumeric arg0) {
    return new INV(arg0);
  }

  public static OGNumeric norm2(OGNumeric arg0) {
    return new NORM2(arg0);
  }

  public static OGNumeric pinv(OGNumeric arg0) {
    return new PINV(arg0);
  }

  public static OGNumeric uminus(OGNumeric arg0) {
    return new NEGATE(arg0);
  }

  public static OGNumeric transpose(OGNumeric arg0) {
    return new TRANSPOSE(arg0);
  }

  // binary nodes

  public static OGNumeric plus(final OGNumeric... args) {
    if (args.length < 2) {
      throw new MathsExceptionIllegalArgument("Cannot use plus() with just one argument.");
    }
    OGNumeric ret = args[0];
    for (int i = 1; i < args.length; i++) {
      ret = new PLUS(ret, args[i]);
    }
    return ret;
  }

  public static OGNumeric minus(final OGNumeric... args) {
    if (args.length < 2) {
      throw new MathsExceptionIllegalArgument("Cannot use minus() with just one argument.");
    }
    OGNumeric ret = args[0];
    for (int i = 1; i < args.length; i++) {
      ret = new PLUS(ret, new NEGATE(args[i]));
    }
    return ret;
  }

  public static OGNumeric mldivide(final OGNumeric... args) {
    if (args.length < 2) {
      throw new MathsExceptionIllegalArgument("Cannot use mldivide() with just one argument.");
    }
    OGNumeric ret = args[0];
    for (int i = 1; i < args.length; i++) {
      ret = new MLDIVIDE(ret, args[i]);
    }
    return ret;
  }
  
  public static OGNumeric mtimes(final OGNumeric... args) {
    if (args.length < 2) {
      throw new MathsExceptionIllegalArgument("Cannot use mtimes() with just one argument.");
    }
    OGNumeric ret = args[0];
    for (int i = 1; i < args.length; i++) {
      ret = new MTIMES(ret, args[i]);
    }
    return ret;
  }


  // variadic results
  
  public static OGLUResult lu(OGNumeric arg0) {
    return new OGLUResult(new LU(arg0));
  }
  
  public static OGSVDResult svd(OGNumeric arg0) {
    return new OGSVDResult(new SVD(arg0));
  }

  // materialisers

  public static void disp(OGNumeric arg0) {
    System.out.println(Materialisers.toOGTerminal(arg0).toString());
  }

  public static ComplexArrayContainer toComplexArrayContainer(OGNumeric arg0) {
    return Materialisers.toComplexArrayContainer(arg0);
  }
  
  public static double[][] toDoubleArrayOfArrays(OGNumeric arg0) {
    return Materialisers.toDoubleArrayOfArrays(arg0);
  }

  public static OGTerminal toOGTerminal(OGNumeric arg0) {
    return Materialisers.toOGTerminal(arg0);
  }

}
