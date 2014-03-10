package com.opengamma.longdog.exceptions;

import java.io.PrintStream;
import java.io.PrintWriter;
import java.util.Collections;
import java.util.IdentityHashMap;
import java.util.Set;

/**
 * Base class for any exceptions that get thrown during the native code execution.
 */
public class MathsExceptionNative extends MathsException {

  private static final long serialVersionUID = 1L;

  public MathsExceptionNative() {
    super();
  }

  /**
   * This form of the constructor is used by the native code for generating the mixed-language backtrace.
   * Frames are represented by 4 strings, containing the class name, method name, file name and line no.
   * The Java side steps through this array to construct suitable StackTraceElement objects to push on
   * top of the Java stack trace.
   *
   * @param msg The error message as presented to the user
   * @param backtraceInfo strings describing each frame
   */
  public MathsExceptionNative(final String msg, final String[] backtraceInfo)
  {
    super(msg);
    StackTraceElement[] javaStackTrace = getStackTrace();
    if ((backtraceInfo.length % 4) != 0) {
      // The backtrace information from C++ is "weird". We shall not attempt to use it.
      System.err.println("WARNING: strange backtrace info received from native");
      return;
    }

    int nNativeFrames = backtraceInfo.length / 4;
    int nJavaFrames = javaStackTrace.length;
    int nFrames = nJavaFrames + nNativeFrames;
    StackTraceElement[] combinedStackTrace = new StackTraceElement[nFrames];
    for (int i = 0; i < nNativeFrames; i++) {
      int index = i * 4;
      String className  = backtraceInfo[index + 0];
      String methodName = backtraceInfo[index + 1];
      String fileName   = backtraceInfo[index + 2];
      int lineNo = Integer.parseInt(backtraceInfo[index + 3]);
      combinedStackTrace[i] = new StackTraceElement(className, methodName, fileName, lineNo);
    }

    for (int i = 0; i < nJavaFrames; i++) {
      combinedStackTrace[i + nNativeFrames] = javaStackTrace[i];
    }

    setStackTrace(combinedStackTrace);
  }

  public MathsExceptionNative(final String s) {
    super(s);
  }

  public MathsExceptionNative(String s, Throwable cause) {
    super(s, cause);
  }

  public MathsExceptionNative(Throwable cause) {
    super(cause);
  }
}
