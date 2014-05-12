/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Random;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.matrix.OGArray;
import com.opengamma.maths.datacontainers.scalar.OGScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionNativeUnspecified;
import com.opengamma.maths.materialisers.Materialisers;

/**
 * Generates pseudo-random trees and runs them
 */
public final class TreeFuzzer {

  int _debuglevel = 1;

  /**
   * The log.
   */
  Logger _log = LoggerFactory.getLogger(TreeFuzzer.class);

  /**
   * Stack element references. Basically points to arbitrary node in the graph
   */
  List<OGNumeric> _stackRefs = new ArrayList<>();

  /**
   * The methods
   */
  static Object[] _methods;

  /**
   * RNG
   */
  Random _rand;

  /**
   * Maximum number of local reference objects in the tree.
   */
  int _maxTreeRefs;

  /**
   * Maximum "size" of terminal data (rows and cols guaranteed to be < this number).
   */
  int _maxDataSize;

  /**
   * List of method names that aren't part of the fuzzer invocation
   */
  static List<String> _bannedMethods;

  static {
    _bannedMethods = new ArrayList<>();
    _bannedMethods.add("disp");
    _bannedMethods.add("toDoubleArrayOfArrays");
    _bannedMethods.add("toComplexArrayContainer");
    _bannedMethods.add("toOGTerminal");
    _bannedMethods.add("C");
    _bannedMethods.add("D");
    _bannedMethods.add("toString");
    _bannedMethods.add("hashCode");
    _bannedMethods.add("getClass");
    _bannedMethods.add("notify");
    _bannedMethods.add("notifyAll");
    _bannedMethods.add("wait");
    _bannedMethods.add("equals");
  };

  /**
   * Sets up internal state of dogma methods via reflection.
   * @param seed the seed for the RNG
   * @param maxTreeRefs
   * @param maxDataSize
   */
  public TreeFuzzer(long seed, int maxTreeRefs, int maxDataSize) {
    _rand = new Random(seed);
    _maxTreeRefs = maxTreeRefs;
    _maxDataSize = maxDataSize;

    Method[] methods = DOGMA.class.getMethods();
    List<Method> methlist = new ArrayList<>();
    for (Method meth : methods) {
      if (_bannedMethods.contains(meth.getName())) {
        continue;
      }
      Class<?>[] argtypes = meth.getParameterTypes();
      boolean allOk = true;
      for (Class<?> type : argtypes) {
        if (!(type.isAssignableFrom(OGNumeric.class) || type.isAssignableFrom(OGNumeric[].class))) {
          allOk = false;
          break;
        }
      }
      if (allOk) {
        methlist.add(meth);
      }
    }

    Collections.sort(methlist, new Comparator<Object>() {
      @Override
      public int compare(Object first, Object second) {
        return first.toString().compareTo(second.toString());
      }
    });

    _methods = methlist.toArray();
    if (_debuglevel > 0) {
      System.out.println("DOGMA METHOD LIST");
      System.out.println(methlist);
    }
  }

  /**
   * Calc tree size
   * @param arg the root of the tree
   * @param count the current size count
   */
  public int treeSize(OGNumeric arg, int count) {
    int ret = 0;

    // descend
    while (!(OGArray.class.isAssignableFrom(arg.getClass()) || OGScalar.class.isAssignableFrom(arg.getClass()))) {
      OGExpr expr = (OGExpr) arg;
      // count node
      ret++;
      // walk
      for (int i = 0; i < expr.getExprs().length; i++) {
        ret += treeSize(expr.getExprs()[i], ret);
      }
      break;
    }

    // Count terminals
    if ((OGArray.class.isAssignableFrom(arg.getClass()))) {
      ret++;
    }
    if ((OGScalar.class.isAssignableFrom(arg.getClass()))) {
      ret++;
    }
    return ret;
  }

  /**
   * Performs the fuzzing.
   */
  public void fuzz(long time) {
    DataCreator dataCreator = new DataCreator(_maxDataSize, _rand.nextInt());

    OGNumeric expr = null;
    long tic = System.nanoTime();
    long elapsed = 0;
    long oneSecond = 1000000000L;
    while (elapsed < time) {
      expr = createExpr(dataCreator, _rand.nextLong());
      if (treeSize(expr, 0) < _maxTreeRefs) {
        _stackRefs.add(expr);
        runTree(expr);
      }
      if (_debuglevel > 0) {
        System.out.println("Tree is size: " + treeSize(expr, 0));
      }
      long toc = System.nanoTime();
      elapsed = (long) ((toc - tic) / 1.e9);

      // attempt to keep the mem usage down, makes leaks easier to spot!
      // gc() every ~5s.
      if (elapsed % 5 * oneSecond == 0) {
        System.gc();
      }
    }

  }

  private void runTree(OGNumeric root) {
    try {
      Materialisers.toOGTerminal(root);
    } catch (MathsException e) {
      // ignore exceptions, trees/operations may be invalid
      if (e instanceof MathsExceptionNativeUnspecified) {
        System.out.println();
        e.printStackTrace();
        StringBuffer buf = new StringBuffer();
        Materialisers.printTree(root, buf, 2, true);
        System.out.println(buf.toString());
      }
    }
    try {
      Materialisers.toDoubleArrayOfArrays(root);
    } catch (MathsException e) {
      // ignore exceptions, trees/operations may be invalid
    }
    try {
      Materialisers.toComplexArrayContainer(root);
    } catch (MathsException e) {
      // ignore exceptions, trees/operations may be invalid
    }
  }

  private OGNumeric aBitOfTree(DataCreator dataCreator) {
    boolean retTerminal = _rand.nextBoolean();
    if (retTerminal) {
      return dataCreator.getAnOGTerminal();
    } else {
      return _stackRefs.get(_rand.nextInt(_stackRefs.size()));
    }
  }

  private OGNumeric createExpr(DataCreator dataCreator, long seed) {
    int n_meths = _methods.length;
    Method meth = (Method) _methods[_rand.nextInt(n_meths)];
    int nparams = meth.getParameters().length;

    if (_debuglevel > 0) {
      System.out.println("\n\n Running method: " + meth.toGenericString() + "\n\n");
      for (int i = 0; i < meth.getParameters().length; i++) {
        System.out.println(meth.getParameters()[i].toString());
        System.out.println(meth.isVarArgs());
      }
    }

    Object ret = null;
    if (meth.isVarArgs()) {
      int local_nparams = nparams;
      local_nparams = _rand.nextInt(10) + 2; // always need 2 args on the varargs NODEs
      OGNumeric[] args = new OGNumeric[local_nparams];
      for (int i = 0; i < local_nparams; i++) {
        args[i] = aBitOfTree(dataCreator);
      }
      try {
        ret = meth.invoke(null, (Object) args);
      } catch (IllegalAccessException | IllegalArgumentException | InvocationTargetException ex) {
        ex.printStackTrace();
        throw new RuntimeException(ex);
      }
    } else {
      try {
        ret = meth.invoke(null, aBitOfTree(dataCreator));
      } catch (IllegalAccessException | IllegalArgumentException | InvocationTargetException ex) {
        ex.printStackTrace();
        throw new RuntimeException(ex);
      }
    }

    // See if we have a compound return type like OGSVDResult
    Class<?> retType = meth.getReturnType();
    if (!retType.isAssignableFrom(OGNumeric.class)) {
      List<Method> available_methods = new ArrayList<>();
      Method[] localmeth = retType.getMethods();
      for (Method lmeth : localmeth) {
        if (lmeth.getReturnType().isAssignableFrom(OGNumeric.class)) {
          available_methods.add(lmeth);
        }
      }
      try {
        if (_debuglevel > 0) {
          System.out.println(available_methods.toString());
        }
        ret = available_methods.get(_rand.nextInt(available_methods.size())).invoke(ret, new Object[0]);
      } catch (IllegalAccessException | IllegalArgumentException | InvocationTargetException ex) {
        ex.printStackTrace();
        throw new RuntimeException(ex);
      }
    }
    if (_debuglevel > 0) {
      StringBuffer buf = new StringBuffer();
      Materialisers.printTree((OGNumeric) ret, buf, 2);
      System.out.println(buf.toString());
    }
    return (OGNumeric) ret;
  }

}
