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
import com.opengamma.maths.nodes.RDIVIDE;
import com.opengamma.maths.nodes.SIN;
import com.opengamma.maths.nodes.SINH;
import com.opengamma.maths.nodes.SVD;
import com.opengamma.maths.nodes.TAN;
import com.opengamma.maths.nodes.TANH;
import com.opengamma.maths.nodes.TIMES;
import com.opengamma.maths.nodes.TRANSPOSE;

/**
 * DOGMA, the OpenGamma maths assembler.
 * <p>
 * DOGMA is a set of simple functions that operate on OGNumeric
 * classes in a vectorised manner. It largely provides mathematical
 * operations. The operations work using delayed evaluation, so after
 * calling DOGMA maths functions, be sure to call a function
 * that materialises the tree to get some output.
 * </p>
 */
//TODO: Add in a link to docs and examples in the above.

public final class DOGMA {

  // Default ctor is hidden, everything is static
  private DOGMA() {
  };

  // Load in and initialise the native libraries.
  static {
    NativeLibraries.initialize();
  }

  /**
   * DOGMA Function: D
   * <p>
   * Short Description:
   * <p>
   * Creates an OGRealScalar from a java Number.
   * <p>
   * Full Description:
   * <p>
   * Instantiates a new OGRealScalar from the numerical value extracted 
   * from an object of the java Number class.
   * @param num the number from which to create the scalar.
   * @return an OGRealScalar representing {@code num}.
   */
  public static OGTerminal D(Number num) {
    return new OGRealScalar(num);
  }

  /**
   * DOGMA Function: C
   * <p>
   * Short Description:
   * <p>
   * Create an OGComplexScalar with a zero imaginary part from a java Number.
   * <p>
   * Full Description:
   * <p>
   * Instantiates a new OGComplexScalar with the real part taken from the numerical
   * value extracted from an object of the java Number class. The imaginary part is
   * set to zero.
   * @param num the number from which to create the real part of the complex scalar.
   * @return an OGComplexScalar representing {@code num}.
   */

  public static OGTerminal C(Number real) {
    return new OGComplexScalar(real);
  }

  /**
   * DOGMA Function: C
   * <p>
   * Short Description:
   * <p>
   * Create an OGComplexScalar from real and imaginary parts taken from java Numbers.
   * <p>
   * Full Description:
   * <p>
   * Instantiates a new OGComplexScalar with the real and imaginary parts taken from 
   * the numerical value extracted from objects of the java Number class. 
   * @param real the number from which to create the real part of the complex scalar.
   * @param imag the number from which to create the imaginary part of the complex scalar.
   * @return an OGComplexScalar representing {@code num}.
   */
  public static OGTerminal C(Number real, Number imag) {
    return new OGComplexScalar(real, imag);
  }

  // unary nodes
  /**
   * DOGMA Function: Acos
   * <p>
   * Short Description:
   * <p>
   * Returns the arc cosine value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the arc cosine value of the argument for both real and complex
   * numbers.
   * @param arg0 The argument for which the arc cosine values are to be computed.
   * <p>
   * @return the arc cosine of the argument.
   */
  public static OGNumeric acos(OGNumeric arg0) {
    return new ACOS(arg0);
  }

  /**
   * DOGMA Function: Asinh
   * <p>
   * Short Description:
   * <p>
   * Returns the hyperbolic arc sine value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the hyperbolic arc sine value of the argument for both real and
   * complex numbers.
   * <p>
   * @param arg0 The argument for which the hyperbolic arc sine values are to be computed.
   * <p>
   * @return the hyperbolic arc sine of the argument.
   */
  public static OGNumeric asinh(OGNumeric arg0) {
    return new ASINH(arg0);
  }

  /**
   * DOGMA Function: Atan
   * <p>
   * Short Description:
   * <p>
   * Returns the arc tangent value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the arc tangent value of the argument for both real and complex
   * numbers.
   * <p>
   * @param arg0 The argument for which the arc tangent values are to be computed.
   * <p>
   * @return the arc tangent of the argument.
   */
  public static OGNumeric atan(OGNumeric arg0) {
    return new ATAN(arg0);
  }

  /**
   * DOGMA Function: Cos
   * <p>
   * Short Description:
   * <p>
   * Returns the cosine value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the cosine value of the argument for both real and complex numbers.
   * <p>
   * @param arg0 The argument for which the cosine values are to be computed.
   * <p>
   * @return the cosine of the argument.
   */
  public static OGNumeric cos(OGNumeric arg0) {
    return new COS(arg0);
  }

  /**
   * DOGMA Function: Ctranspose
   * <p>
   * Short Description:
   * <p>
   * Returns the conjugate transpose of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the conjugate transpose of the argument. This is the equivalent to
   * arg0' in m-code. If the literal transpose of a matrix is needed then use
   * the function transpose().
   * <p>
   * @param arg0 the array on which the conjugate transpose operation shall be applied.
   * <p>
   * @return Returns the conjugate transpose of the argument.
   */
  public static OGNumeric ctranspose(OGNumeric arg0) {
    return new CTRANSPOSE(arg0);
  }

  /**
   * DOGMA Function: Exp
   * <p>
   * Short Description:
   * <p>
   * Returns the base of the natural logarithm (e) raised to the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the base of the natural logarithm (e) raised to the argument.
   * Mathematically: i.e. e^(arg0).
   * <p>
   * @param arg0 The argument to which "e" shall be raised.
   * <p>
   * @return the value "e" raised to the argument.
   */
  public static OGNumeric exp(OGNumeric arg0) {
    return new EXP(arg0);
  }

  /**
   * DOGMA Function: Inv
   * <p>
   * Short Description:
   * <p>
   * Returns the inverse of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the inverse of the argument. This is the equivalent to inv(arg0) in
   * m-code. If this function is being used to solve a linear system of the form:
   * AX=B, via X=inv(A)*B, please use @{code mldivide()} instead as it has greater
   * numerical stability.
   * <p>
   * @param arg0 The argument to be inverted.
   * <p>
   * @return the inverse of the argument.
   */
  public static OGNumeric inv(OGNumeric arg0) {
    return new INV(arg0);
  }

  /**
   * DOGMA Function: Norm2
   * <p>
   * Short Description:
   * <p>
   * Returns the Euclidean (2-norm) of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the Euclidean (2-norm) of the argument. This is the equivalent to norm(arg0)
   * in m-code.
   * <p>
   * @param arg0 The argument to be for which the 2-norm value shall be computed.
   * <p>
   * @return the 2-norm value of the argument.
   */
  public static OGNumeric norm2(OGNumeric arg0) {
    return new NORM2(arg0);
  }

  /**
   * DOGMA Function: Pinv
   * <p>
   * Short Description:
   * <p>
   * Returns the pseudo-inverse of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the Moore-Penrose pseudo-inverse of the argument. 
   * This is the equivalent to pinv(arg0) in m-code. 
   * If this function is being used to an solve an indeterminate linear system of the form:
   * AX=B, via X=inv(A)*B, please use @{code mldivide()} instead as it has greater
   * numerical stability.
   * 
   * NOTE: Default truncation of singular values applies at threshold:
   * max(singular value) * max(rows,cols) * machine epsilon.
   * <p>
   * @param arg0 The argument for which the pseudo-inverse shall be computed.
   * <p>
   * @return the pseudo-inverse of the argument.
   */
  public static OGNumeric pinv(OGNumeric arg0) {
    return new PINV(arg0);
  }

  /**
   * DOGMA Function: Sin
   * <p>
   * Short Description:
   * <p>
   * Returns the sine value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the sine value of the argument for both real and complex numbers.
   * <p>
   *
   * @param arg0 The argument for which the sine values are to be computed.
   * <p>
   * @return the sin of the argument.
   */
  public static OGNumeric sin(OGNumeric arg0) {
    return new SIN(arg0);
  }

  /**
   * DOGMA Function: Sinh
   * <p>
   * Short Description:
   * <p>
   * Returns the hyperbolic sine value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the hyperbolic sine value of the argument for both real and complex
   * numbers.
   * <p>
   * @param arg0 The argument for which the hyperbolic sine values are to be computed.
   * <p>
   * @return the hyperbolic sine of the argument.
   */
  public static OGNumeric sinh(OGNumeric arg0) {
    return new SINH(arg0);
  }

  /**
   * DOGMA Function: Tan
   * <p>
   * Short Description:
   * <p>
   * Returns the tangent value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the tangent value of the argument for both real and complex
   * numbers.
   * <p>
   * @param arg0 The argument for which the tangent values are to be computed.
   * <p>
   * @return the tangent of the argument.
   */
  public static OGNumeric tan(OGNumeric arg0) {
    return new TAN(arg0);
  }

  /**
   * DOGMA Function: Tanh
   * <p>
   * Short Description:
   * <p>
   * Returns the hyperbolic tangent value of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the hyperbolic tangent value of the argument for both real and
   * complex numbers.
   * <p>
   * @param arg0 The argument for which the hyperbolic tangent values are to be computed.
   * <p>
   * @return the hyperbolic tangent of the argument.
   */
  public static OGNumeric tanh(OGNumeric arg0) {
    return new TANH(arg0);
  }

  /**
   * DOGMA Function: Transpose
   * <p>
   * Short Description:
   * <p>
   * Returns the literal transpose of the argument.
   * <p>
   * Full Description:
   * <p>
   * Returns the literal transpose of the argument. If the conjugate transpose
   * of a matrix is needed then use the function ctranspose().
   * <p>
   * @param arg0 the array on which the transpose operation shall be applied.
   * <p>
   * @return Returns the transpose of the argument.
   */
  public static OGNumeric transpose(OGNumeric arg0) {
    return new TRANSPOSE(arg0);
  }

  /**
   * DOGMA Function: Uminus
   * <p>
   * Short Description:
   * <p>
   * Returns the argument with its values negated.
   * <p>
   * Full Description:
   * <p>
   * Returns the argument with its values negated. Mathematically: -(arg0)
   * <p>
   * @param arg0 The argument for which the values shall be negated.
   * <p>
   * @return the argument with its values negated.
   */
  public static OGNumeric uminus(OGNumeric arg0) {
    return new NEGATE(arg0);
  }

  // binary nodes

  /**
  * DOGMA Function: Plus
  * <p>
  * Short Description:
  * <p>
  * Returns the element wise sum of the input arguments.
  * <p>
  * Full Description:
  * <p>
  * Returns the element wise addition of the input arguments. This is the
  * equivalent of the m-code operation arg0+arg1, where arg0 is args[0] 
  * and arg1 is args[1]. The operation is vectorised such that if either 
  * argument is a single number it is applied to each element of the argument, 
  * whereas if both arguments are the same dimension element wise addition takes 
  * place.
  * <p>
  * @param args The arguments to add.
  *
  * @return The element wise sum of the input arguments.
  */
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

  /**
   * DOGMA Function: Minus
   * <p>
   * Short Description:
   * <p>
   * Returns the element wise subtraction of all the input arguments from the first.
   * <p>
   * Full Description:
   * <p>
   * Returns the element wise subtraction of all arguments from the
   * first argument. This is the equivalent of the m-code operation arg0-arg1, where
   * arg0 is args[0] and arg1 is args[1]. The operation is vectorised such that if 
   * either argument is a single number it is applied to each element of the argument,
   * whereas if both arguments are the same dimension element wise subtraction takes place.
   * <p>
   * @param args The arguments used in the subtraction. The first argument, args[0] is the
   * argument from which all subsequent arguments are subtracted.
   *
   * @return The element wise subtraction of the arguments from the first argument.
   */
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

  /**
   * DOGMA Function: Mldivide
   * <p>
   * Short Description:
   * <p>
   * Performs matrix left division.
   * <p>
   * Full Description:
   * <p>
   * The matrix left division operation akin to the m-code "\" operator. This function
   * finds solutions to linear systems: If the matrix arg0 is square and
   * invertible a solution is found equivalent to {(arg0)^-1}*arg1. If the matrix
   * arg0 is either singular or not square a solution is found based on the
   * minimum 2-norm of the residual, where arg0 is args[0] and arg1 is args[1].
   * <p>
   * @param args The arguments used in the matrix left division. The first argument, args[0]
   * is the argument from which all subsequent arguments arguments are matrix left divided.
   *
   * @return The matrix left division of the arguments from the first argument.
   */
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

  /**
   * DOGMA Function: Mtimes
   * <p>
   * Short Description:
   * <p>
   * Returns the matrix multiplication of the first argument by all subsequent arguments.
   * <p>
   * Full Description:
   * <p>
   * Returns the matrix multiplication of the first argument by all subsequent arguments.
   * This is the equivalent of the m-code operation arg0*arg1, where arg0 is args[0] and arg1 is args[1].
   * The operation is vectorised such that if either argument is a single number it is applied
   * to each element of the argument, whereas if both arguments are the same
   * dimension full matrix multiplication takes place.
   * <p>
   * @param args The arguments used in the matrix multiplication.
   *
   * @return The matrix multiplication of the arguments.
   */
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

  /**
   * DOGMA Function: Rdivide
   * <p>
   * Short Description:
   * <p>
   * Returns the element wise right division of the first argument by all subsequent arguments.
   * <p>
   * Full Description:
   * <p>
   * Returns the element wise right division of the first argument by all subsequent arguments.
   * This is the equivalent of the m-code operation arg0./arg1, where
   * arg0 is args[0] and arg1 is args[1]. The operation is vectorised such that if 
   * either argument is a single number it is applied to each element of the argument,
   * whereas if both arguments are the same dimension element wise right division takes place.
   * <p>
   * @param args The arguments used in the right division. The first argument, args[0] is the
   * argument right divided by all subsequent arguments.
   *
   * @return The element wise right division of the first argument by all subsequent arguments.
   */
  public static OGNumeric rdivide(final OGNumeric... args) {
    if (args.length < 2) {
      throw new MathsExceptionIllegalArgument("Cannot use rdivide() with just one argument.");
    }
    OGNumeric ret = args[0];
    for (int i = 1; i < args.length; i++) {
      ret = new RDIVIDE(ret, args[i]);
    }
    return ret;
  }

  /**
   * DOGMA Function: Times
   * <p>
   * Short Description:
   * <p>
   * Returns the element wise multiplication of the input arguments.
   * <p>
   * Full Description:
   * <p>
   * Returns the element wise multiplication of the input arguments. This is the
   * equivalent of the m-code operation arg0.*arg1, where arg0 is args[0] and
   * arg1 is args[1]. The operation is vectorised such that if either argument
   * is a single number it is applied as a scaling, whereas if both arguments 
   * are the same dimension element wise multiplication takes place.
   * <p>
   * @param arg0 The first argument to multiply.
   * @param arg1 The second argument to multiply
   *
   * @return The element wise multiplication of the first argument by all subsequent arguments.
   */
  public static OGNumeric times(final OGNumeric... args) {
    if (args.length < 2) {
      throw new MathsExceptionIllegalArgument("Cannot use times() with just one argument.");
    }
    OGNumeric ret = args[0];
    for (int i = 1; i < args.length; i++) {
      ret = new TIMES(ret, args[i]);
    }
    return ret;
  }

  // variadic results
  /**
   * DOGMA Function: LU
   * <p>
   * Short Description:
   * <p>
   * The LU function computes the LU decomposition of a matrix.
   * <p>
   * Full Description:
   * <p>
   * The LU function is the LU workhorse. It computes the LU decomposition of a
   * matrix.
   * <p>
   * @param arg0 The data array from which the LU decomposition shall be found.
   * @return An OGLUResult containing the L and U matrices.
   */
  public static OGLUResult lu(OGNumeric arg0) {
    return new OGLUResult(new LU(arg0));
  }

  /**
   * DOGMA Function: SVD
   * <p>
   * Short Description:
   * <p>
   * The SVD function computes the singular value decomposition of a matrix.
   * <p>
   * Full Description:
   * <p>
   * The SVD function is the singular value decomposition workhorse. It computes
   * singular values and the associated matrices of left and
   * right singular vectors.
   * <p>
   * @param arg0 The data array from which the singular values and (possible) matrices of singular vectors shall be found.
   * @return An OGSVDResult containing the matrices of left and right singular vectors and a matrix of singular values.
   */
  public static OGSVDResult svd(OGNumeric arg0) {
    return new OGSVDResult(new SVD(arg0));
  }

  // materialisers
  /**
   * DOGMA Function: Disp
   * <p>
   * Short Description:
   * <p>
   * Displays (prints) OGNumerics to stdout.
   * <p>
   * Full Description:
   * <p>
   * A crude function for simply printing the contents of an OGNumeric to stdout.
   * <p>
   * @param arg0 The argument to print to stdout.
   */
  public static void disp(OGNumeric arg0) {
    System.out.println(Materialisers.toOGTerminal(arg0).toString());
  }

  /**
   * DOGMA Function: toComplexArrayContainer
   * <p>
   * Short Description:
   * <p>
   * Materialises an OGNumeric into a ComplexArrayContainer type.
   * <p>
   * Full Description:
   * <p>
   * Materialises an OGNumeric (executes a tree and produces a concrete result)
   * into a ComplexArrayContainer type.
   * <p>
   * @param arg0 The argument to materialise.
   * @return a ComplexArrayContainer, the result of materialising the OGNumeric tree at arg0.
   */
  public static ComplexArrayContainer toComplexArrayContainer(OGNumeric arg0) {
    return Materialisers.toComplexArrayContainer(arg0);
  }

  /**
   * DOGMA Function: toDoubleArrayOfArrays
   * <p>
   * Short Description:
   * <p>
   * Materialises an OGNumeric into a double[][] type.
   * <p>
   * Full Description:
   * <p>
   * Materialises an OGNumeric (executes a tree and produces a concrete result)
   * into a double[][] type.
   * <p>
   * @param arg0 The argument to materialise.
   * @return a double[][], the result of materialising the OGNumeric tree at arg0.
   */
  public static double[][] toDoubleArrayOfArrays(OGNumeric arg0) {
    return Materialisers.toDoubleArrayOfArrays(arg0);
  }

  /**
   * DOGMA Function: toOGTerminal
   * <p>
   * Short Description:
   * <p>
   * Materialises an OGNumeric into an OGTerminal type.
   * <p>
   * Full Description:
   * <p>
   * Materialises an OGNumeric (executes a tree and produces a concrete result)
   * into a OGTerminal type.
   * <p>
   * @param arg0 The argument to materialise.
   * @return an OGTerminal, the result of materialising the OGNumeric tree at arg0.
   */
  public static OGTerminal toOGTerminal(OGNumeric arg0) {
    return Materialisers.toOGTerminal(arg0);
  }

}
