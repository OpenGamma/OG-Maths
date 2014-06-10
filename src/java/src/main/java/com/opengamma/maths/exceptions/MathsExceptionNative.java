package com.opengamma.maths.exceptions;

import com.opengamma.maths.logging.Logger;

/**
 * Base class for any exceptions that get thrown during the native code execution.
 */
public class MathsExceptionNative extends MathsException {

  /**
   * This form of the constructor is used on Windows, where no native stack trace is provided.
   * @param msg the error message
   */
  public MathsExceptionNative(String msg) {
	super(msg);
  }

  /**
   * Serial ID
   */
  private static final long serialVersionUID = 2689725471014028060L;

  /**
   * The logger instance
   */
  private static Logger s_log = new Logger(MathsExceptionNative.class);

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
      s_log.warn("Strange backtrace info received from native");
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
}
