/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

import org.testng.IAnnotationTransformer;
import org.testng.annotations.ITestAnnotation;
import org.testng.annotations.Listeners;
import org.testng.annotations.Test;

import com.opengamma.maths.helpers.TestGroups;

@Test
@Listeners(TransformAnnotationFuzzOnly.class)
/**
 * Transforms TestNG annotations of that are members of the group "TestGroups.FUZZER".
 */
public class TransformAnnotationFuzzOnly implements IAnnotationTransformer {

  /**
   * Whether to run the fuzzer tests.
   */
  private boolean _runFuzzTests = false;

  /**
   * Constructor to set transformer internal state.
   */
  public TransformAnnotationFuzzOnly() {
    String commandLineConfig = System.getProperty("runFuzzerTests");
    if (commandLineConfig != null) { // if specified on command line, run fuzzer tests
      _runFuzzTests = true;
    }
  }

  @SuppressWarnings("rawtypes")
  @Override
  public void transform(ITestAnnotation annotation, Class theClass, Constructor theCtor, Method theMethod) {

    if (!_runFuzzTests) { // if fuzzer tests is not set we want to disable them
      // does the annotation contain the fuzz group?
      if (annotationContainsFuzzGroup(annotation)) {
        annotation.setEnabled(false); // then don't run it!
      }

    }
  }

  /**
   * Checks an annotation to see if it has groups that contain "TestGroups.FUZZER".
   * @param annotation the annotation to check.
   * @return true if "TestGroups.FUZZER" is found, false else.
   */
  private boolean annotationContainsFuzzGroup(ITestAnnotation annotation) {
    String[] groups = annotation.getGroups();
    for (String group : groups) {
      if (group.equals(TestGroups.FUZZER)) {
        return true;
      }
    }
    return false;
  }

}
