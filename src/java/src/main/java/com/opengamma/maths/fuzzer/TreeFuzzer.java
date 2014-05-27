/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Random;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.matrix.OGArray;
import com.opengamma.maths.datacontainers.scalar.OGScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionNativeUnspecified;
import com.opengamma.maths.helpers.Catchers;
import com.opengamma.maths.materialisers.Materialisers;

/**
 * Generates pseudo-random trees and runs them
 */
public class TreeFuzzer implements Fuzzer {

  /**
   * Debug level, for debugging purposes.
   */
  private int _debuglevel = 0;

  /**
   * Stack element references. Basically points to arbitrary node in the graph
   */
  private FixedSizeFIFO<OGNumeric> _stackRefs = new FixedSizeFIFO<>();

  /**
   * The methods available.
   */
  private Object[] _methods;

  /**
   * The random number generator.
   */
  private Random _rand;

  /**
   * Maximum number of local reference objects in the tree.
   */
  private int _maxTreeRefs;

  /**
   * Maximum "size" of terminal data (rows and cols guaranteed to be < this number).
   */
  private int _maxDataSize;

  /**
   * List of method names that aren't part of the fuzzer invocation
   */
  static List<String> _bannedMethods;

  /**
   * These methods are all banned, some because the don't do anything API wise, others
   * because the are inherited from {@code Object} and are not applicable.
   */
  static {
    _bannedMethods = new ArrayList<>();

    // part of the DOGMA API
    _bannedMethods.add("disp");
    _bannedMethods.add("toDoubleArrayOfArrays");
    _bannedMethods.add("toComplexArrayContainer");
    _bannedMethods.add("toOGTerminal");
    _bannedMethods.add("C");
    _bannedMethods.add("D");

    // inherited from Object
    _bannedMethods.add("toString");
    _bannedMethods.add("hashCode");
    _bannedMethods.add("getClass");
    _bannedMethods.add("notify");
    _bannedMethods.add("notifyAll");
    _bannedMethods.add("wait");
    _bannedMethods.add("equals");
  };

  /**
   * One second in quantity of nanoseconds.
   */
  private static final long S_SECOND = 1000000000L;

  /**
   * Logging flag, true if logging requested.
   */
  private boolean _logging;

  /**
   * The log file, null if _logging is false.
   */
  private RandomAccessFile _log;

  /**
   * Sets up internal state of dogma methods via reflection.
   * @param seed the seed for the RNG.
   * @param maxTreeRefs the maximum number of references allowable in a tree.
   * @param maxDataSize the maximum length scale of the terminal data (i.e. rows/cols).
   */
  public TreeFuzzer(long seed, int maxTreeRefs, int maxDataSize) {
    this(seed, maxTreeRefs, maxDataSize, false, null);
  }

  /**
   * Sets up internal state of dogma methods via reflection.
   * @param seed the seed for the RNG.
   * @param maxTreeRefs the maximum number of references allowable in a tree.
   * @param maxDataSize the maximum length scale of the terminal data (i.e. rows/cols).
   * @param logging true to switch on logging.
   * @param log the file into which logs will be written.
   */
  public TreeFuzzer(long seed, int maxTreeRefs, int maxDataSize, boolean logging, RandomAccessFile log) {
    Catchers.catchCondition(seed < 0, "Seed should be > 0");
    Catchers.catchCondition(maxTreeRefs < 1, "maxTreeRefs should be >= 1");
    Catchers.catchCondition(maxDataSize < 1, "maxDataSize should be >= 1");
    Catchers.catchCondition((logging == true && log == null), "Logging is requested but not log file has been provided.");

    _rand = new Random(seed);
    _maxTreeRefs = maxTreeRefs;
    _maxDataSize = maxDataSize;
    _logging = logging;
    _log = log;

    // get the methods in the DOGMA class
    Method[] methods = DOGMA.class.getMethods();
    List<Method> methlist = new ArrayList<>();
    // Walk all methods
    for (Method meth : methods) {
      // If the method is "banned", i.e. part of the API we don't want to test, skip.
      if (_bannedMethods.contains(meth.getName())) {
        continue;
      }
      // Get the classes of the arg types for the method in question.
      Class<?>[] argtypes = meth.getParameterTypes();
      // Check that all arg types inherit from OGNumeric or varargs OGNumeric[]
      boolean allOk = true;
      for (Class<?> type : argtypes) {
        if (!(type.isAssignableFrom(OGNumeric.class) || type.isAssignableFrom(OGNumeric[].class))) {
          allOk = false;
          break;
        }
      }
      // If all is ok, it means we can handle the method so add it to the list of testable methods.
      if (allOk) {
        methlist.add(meth);
      }
    }

    // Make sure we have a deterministic methods list, so sort on lexicographical order.
    Collections.sort(methlist, new Comparator<Object>() {
      @Override
      public int compare(final Object first, final Object second) {
        return first.toString().compareTo(second.toString());
      }
    });

    // finally assign to the class member methods list.
    _methods = methlist.toArray();

    if (_debuglevel > 0) {
      System.out.println("DOGMA METHOD LIST");
      System.out.println(methlist);
    }
  }

  /**
   * Calculate the size of a tree.
   * This method will return the count of the number of references present in a tree.
   * @param arg the root of the tree.
   * @param count the current size count.
   * @return the number of refs in this tree plus {@code count}.
   */
  public final int treeRefCount(OGNumeric arg, int count) {
    int ret = 0;

    // descend
    while (!(OGArray.class.isAssignableFrom(arg.getClass()) || OGScalar.class.isAssignableFrom(arg.getClass()))) {
      OGExpr expr = (OGExpr) arg;
      // count node
      ret++;
      // walk
      for (int i = 0; i < expr.getExprs().length; i++) {
        ret += treeRefCount(expr.getExprs()[i], ret);
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
   * {@inheritDoc}
   */
  public final void fuzz(final long time) {
    // the local data providing class.
    DataCreator dataCreator = new DataCreator(_maxDataSize, _rand.nextInt());

    // buffer for use in logging
    StringBuffer exprstrbuf = null;
    if (_logging) {
      exprstrbuf = new StringBuffer();
    }

    OGNumeric expr = null;
    long elapsed = 0;
    long startTime = System.nanoTime();
    long test = time * S_SECOND;
    long stopTime = startTime + test > startTime ? startTime + test : Long.MAX_VALUE;

    // main execution loop, whilst time remains, keep throwing trees at the library
    while (System.nanoTime() < stopTime) {
      // create an expression
      expr = createExpr(dataCreator, _rand.nextLong());
      // if the expression size is valid, then cache it for use as input for trees that
      // will be created in the future, then run this tree.
      int treeRefCount = treeRefCount(expr, 0);
      if (treeRefCount < _maxTreeRefs) {
        _stackRefs.add(expr);

        // If logging, dump the tree to file. Idea is to log what is going to be
        // run so if something catastrophic happens, we can get the logs, recreate and debug
        if (_logging) {
          Materialisers.printTree(expr, exprstrbuf, 2, true);
          try {
            _log.seek(0); // rewind
            _log.writeChars(exprstrbuf.toString()); // write
          } catch (IOException ex) {
            ex.printStackTrace();
            throw new RuntimeException(ex);
          }
          exprstrbuf.setLength(0);
        }
        executeTree(expr);
      }
      if (_debuglevel > 0) {
        System.out.println("Tree is size: " + treeRefCount(expr, 0));
      }
      long toc = System.nanoTime();
      elapsed = ((toc - startTime) / S_SECOND);
      if (_debuglevel > 0) {
        System.out.println("Elapsed time whilst executing trees: " + elapsed + "s.");
      }

    }

  }

  /**
   * Executes the a specified tree.
   * @param root the root of the tree to execute.
   * This method will attempt all types of materialise, but will only process one set of errors.
   */
  private void executeTree(final OGNumeric root) {
    Catchers.catchNullFromArgList(root, 1);
    // Try the toOGTerminal materialiser first as it's generally more tricky
    try {
      Materialisers.toOGTerminal(root);
    } catch (MathsException e) {
      // Ignore exceptions, trees/operations may be invalid
      // However, catch unspecified errors, we should be catching and processing all errors
      // from the libjshim side so if we're here something has gone wrong and we need to report.
      if (e instanceof MathsExceptionNativeUnspecified) {
        e.printStackTrace();
        StringBuffer buf = new StringBuffer();
        Materialisers.printTree(root, buf, 2, true);
        System.out.println(buf.toString());
      }
    }

    // Run the other two materialisers
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

  /**
   * Gets a piece of tree, either some part of a tree we've seen before, or a new OGTerminal.
   * @param dataCreator the class that will provide new OGTerminals if needed.
   * @return an OGNumeric, could be a terminal could be a bit of tree.
   */
  private OGNumeric aBitOfTree(DataCreator dataCreator) {
    boolean retTerminal = _rand.nextBoolean();
    if (retTerminal) {
      return dataCreator.getAnOGTerminal();
    } else { // ==0 branch is to stop the code trying to return a piece of tree when none are cached.
      if (_stackRefs.size() == 0 || _stackRefs.size() > Integer.MAX_VALUE) {
        return dataCreator.getAnOGTerminal();
      } else {
        return _stackRefs.get(_rand.nextInt(_stackRefs.size()));
      }
    }
  }

  /**
   * Creates valid expressions which are returned, it also caches them for use later.
   * @param dataCreator a class to provide new OGTerminals when needed.
   * @param seed a seed for the random number generator.
   */
  private OGNumeric createExpr(DataCreator dataCreator, long seed) {

    // Select a NODE to use from the array of available methods.
    int n_meths = _methods.length;
    Method meth = (Method) _methods[_rand.nextInt(n_meths)];
    int nparams = meth.getGenericParameterTypes().length;

    if (_debuglevel > 0) {
      System.out.println("\n\n Running method: " + meth.toGenericString() + "\n\n");
      for (int i = 0; i < nparams; i++) {
        System.out.println(meth.getGenericParameterTypes()[i].toString());
        System.out.println(meth.isVarArgs());
      }
    }

    // Work out what data the NODE needs and provide it.
    Object ret = null;
    if (meth.isVarArgs()) { // If method is varargs, create 2 + (N<=10) arguments and invoke the NODE creation.
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
    } else { // method takes a single argument, get a bit of tree and invoke the NODE.
      try {
        ret = meth.invoke(null, aBitOfTree(dataCreator));
      } catch (IllegalAccessException | IllegalArgumentException | InvocationTargetException ex) {
        ex.printStackTrace();
        throw new RuntimeException(ex);
      }
    }

    // Now check the return type, we need to return a valid expression so hiding types like OGSVDResult
    // need future work to lift out something sane.
    // Get return type
    Class<?> retType = meth.getReturnType();
    // If the return type does not extend an OGNumeric execute branch.
    if (!retType.isAssignableFrom(OGNumeric.class)) {
      List<Method> available_methods = new ArrayList<>();
      Method[] localmeth = retType.getMethods();
      // check the methods in the return type for ones that will produce something that extends OGNumeric, store them
      for (Method lmeth : localmeth) {
        if (lmeth.getReturnType().isAssignableFrom(OGNumeric.class)) {
          available_methods.add(lmeth);
        }
      }
      try {
        if (_debuglevel > 0) {
          System.out.println(available_methods.toString());
        }
        // invoke a random but valid method on the return type based on the current tree stub to create a valid expression
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
    // finally, return the expression
    return (OGNumeric) ret;
  }

}
