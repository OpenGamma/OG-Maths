/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.helpers;


/**
 * Catch things that will cause problems
 */
public class Catchers {

  // general complaints about null

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(boolean[] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   */
  public static void catchNull(boolean[] thisArray) {
    if (thisArray == null) {
      throw new MathsException("Array points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(byte[] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   */
  public static void catchNull(byte[] thisArray) {
    if (thisArray == null) {
      throw new MathsException("Array points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(short[] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   */
  public static void catchNull(short[] thisArray) {
    if (thisArray == null) {
      throw new MathsException("Array points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(int[] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   */
  public static void catchNull(int[] thisArray) {
    if (thisArray == null) {
      throw new MathsException("Array points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(long[] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   */
  public static void catchNull(long[] thisArray) {
    if (thisArray == null) {
      throw new MathsException("Array points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(double[] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   */
  public static void catchNull(double[] thisArray) {
    if (thisArray == null) {
      throw new MathsException("Array points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(double[][] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
    for (int i = 0; i < thisArray.length; i++) {
      if (thisArray[i] == null) {
        throw new MathsException("Array " + varName + " has row " + i + " pointing to NULL. STOPPING");
      }
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(float[] thisArray, String varName) {
    if (thisArray == null) {
      throw new MathsException("Array " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   */
  public static void catchNull(float[] thisArray) {
    if (thisArray == null) {
      throw new MathsException("Array points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param someBlob an object to check
   * @param varName the literal name of the variable so that the error string makes sense 
   */
  public static void catchNull(Object someBlob, String varName) {
    if (someBlob == null) {
      throw new MathsException("Object " + varName + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param someBlob an object to check
   */
  public static void catchNull(Object someBlob) {
    if (someBlob == null) {
      throw new MathsException("Object ref " + someBlob + " points to NULL. STOPPING");
    }
  }

  ///////////// Null from arg list
  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list
   */
  public static void catchNullFromArgList(boolean[] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list
   */
  public static void catchNullFromArgList(short[] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list
   */
  public static void catchNullFromArgList(int[] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list
   */
  public static void catchNullFromArgList(long[] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list
   */
  public static void catchNullFromArgList(float[] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list
   */
  public static void catchNullFromArgList(double[] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param someBlob the object to check
   * @param pos the position in the arg list
   */
  public static void catchNullFromArgList(Object someBlob, int pos) {
    if (someBlob == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list 
   */
  public static void catchNullFromArgList(double[][] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
    for (int i = 0; i < thisArray.length; i++) {
      if (thisArray[i] == null) {
        throw new MathsException("Argument passed in position " + pos + " has row " + i + " pointing to NULL. STOPPING");
      }
    }
  }

  /**
   * Catches null pointer
   * @param thisArray the array to check
   * @param pos the position in the arg list 
   */
  public static void catchNullFromArgList(int[][] thisArray, int pos) {
    if (thisArray == null) {
      throw new MathsException("Argument passed in position " + pos + " points to NULL. STOPPING");
    }
    for (int i = 0; i < thisArray.length; i++) {
      if (thisArray[i] == null) {
        throw new MathsException("Argument passed in position " + pos + " has row " + i + " pointing to NULL. STOPPING");
      }
    }
  }

  // Numeric catchers  
  // my kingdom for a template/macro  

  // <
  /**
   * Catches negative from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeFromArgList(short aNumber, int pos) {
    if (aNumber < 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches negative from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeFromArgList(int aNumber, int pos) {
    if (aNumber < 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches negative from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeFromArgList(long aNumber, int pos) {
    if (aNumber < 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches negative from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeFromArgList(float aNumber, int pos) {
    if (aNumber < 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches negative from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeFromArgList(double aNumber, int pos) {
    if (aNumber < 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to zero. STOPPING");
    }
  }

  // <=
  /**
   * Catches negative or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeOrZeroFromArgList(short aNumber, int pos) {
    if (aNumber <= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than zero. STOPPING");
    }
  }

  /**
   * Catches negative or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeOrZeroFromArgList(int aNumber, int pos) {
    if (aNumber <= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than zero. STOPPING");
    }
  }

  /**
   * Catches negative or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeOrZeroFromArgList(long aNumber, int pos) {
    if (aNumber <= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than zero. STOPPING");
    }
  }

  /**
   * Catches negative or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeOrZeroFromArgList(float aNumber, int pos) {
    if (aNumber <= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than zero. STOPPING");
    }
  }

  /**
   * Catches negative or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBeNegativeOrZeroFromArgList(double aNumber, int pos) {
    if (aNumber <= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than zero. STOPPING");
    }
  }

  // >=
  /**
   * Catches positive or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveOrZeroFromArgList(short aNumber, int pos) {
    if (aNumber >= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches positive or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveOrZeroFromArgList(int aNumber, int pos) {
    if (aNumber >= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches positive or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveOrZeroFromArgList(long aNumber, int pos) {
    if (aNumber >= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches positive or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveOrZeroFromArgList(float aNumber, int pos) {
    if (aNumber >= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to zero. STOPPING");
    }
  }

  /**
   * Catches positive or zero from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveOrZeroFromArgList(double aNumber, int pos) {
    if (aNumber >= 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to zero. STOPPING");
    }
  }

  // >
  /**
   * Catches positive from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveFromArgList(short aNumber, int pos) {
    if (aNumber > 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than zero. STOPPING");
    }
  }

  /**
   * Catches positive from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveFromArgList(int aNumber, int pos) {
    if (aNumber > 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than zero. STOPPING");
    }
  }

  /**
   * Catches positive from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveFromArgList(long aNumber, int pos) {
    if (aNumber > 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than zero. STOPPING");
    }
  }

  /**
   * Catches positive from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveFromArgList(float aNumber, int pos) {
    if (aNumber > 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than zero. STOPPING");
    }
  }

  /**
   * Catches positive from arg list
   * @param aNumber the value to check
   * @param pos the position in the arg list
   */
  public static void catchValueShouldNotBePositiveFromArgList(double aNumber, int pos) {
    if (aNumber > 0) {
      throw new MathsException("Argument passed in position " + pos + " should be less than zero. STOPPING");
    }
  }

  // Catch bounds var1 should be less than var2
  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanXFromArgList(short aNumber, short aValue, int pos) {
    if (aNumber >= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanXFromArgList(int aNumber, int aValue, int pos) {
    if (aNumber >= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanXFromArgList(long aNumber, long aValue, int pos) {
    if (aNumber >= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanXFromArgList(float aNumber, float aValue, int pos) {
    if (aNumber >= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanXFromArgList(double aNumber, double aValue, int pos) {
    if (aNumber >= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than " + aValue + ". STOPPING");
    }
  }

  // Catch bounds var1 should be less than or equal to var2
  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanOrEqualToXFromArgList(short aNumber, short aValue, int pos) {
    if (aNumber > aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanOrEqualToXFromArgList(int aNumber, int aValue, int pos) {
    if (aNumber > aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanOrEqualToXFromArgList(long aNumber, long aValue, int pos) {
    if (aNumber > aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanOrEqualToXFromArgList(float aNumber, float aValue, int pos) {
    if (aNumber > aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeLessThanOrEqualToXFromArgList(double aNumber, double aValue, int pos) {
    if (aNumber > aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be less than or equal to " + aValue + ". STOPPING");
    }
  }

  // Catch bounds var1 should be Greater than var2
  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanXFromArgList(short aNumber, short aValue, int pos) {
    if (aNumber <= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanXFromArgList(int aNumber, int aValue, int pos) {
    if (aNumber <= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanXFromArgList(long aNumber, long aValue, int pos) {
    if (aNumber <= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanXFromArgList(float aNumber, float aValue, int pos) {
    if (aNumber <= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanXFromArgList(double aNumber, double aValue, int pos) {
    if (aNumber <= aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than " + aValue + ". STOPPING");
    }
  }

  // Catch bounds var1 should be Greater than or equal to var2
  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanOrEqualToXFromArgList(short aNumber, short aValue, int pos) {
    if (aNumber < aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanOrEqualToXFromArgList(int aNumber, int aValue, int pos) {
    if (aNumber < aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanOrEqualToXFromArgList(long aNumber, long aValue, int pos) {
    if (aNumber < aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanOrEqualToXFromArgList(float aNumber, float aValue, int pos) {
    if (aNumber < aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to " + aValue + ". STOPPING");
    }
  }

  /**
   * Catches bad value from arg list
   * @param aNumber the value to check
   * @param aValue the value to which a aNumber should be compared
   * @param pos the position in the arg list
   */
  public static void catchValueShouldBeGreaterThanOrEqualToXFromArgList(double aNumber, double aValue, int pos) {
    if (aNumber < aValue) {
      throw new MathsException("Argument passed in position " + pos + " should be greater than or equal to " + aValue + ". STOPPING");
    }
  }

  public static void catchCondition(boolean cond, String message) {
    if (cond) {
      throw new MathsException("Condition failed to be met. Error message: " + message);
    }
  }

  // Catch bad bounds/insufficient data
  /**
   * Catches insufficient data given to a routine
   * @param aMatrix the data to length check
   * @param expectedMinimumLength the minimum length of the data
   * @param pos the position in the arg list
   */
  public static void catchInsufficientDataSizeFromArgList(double[] aMatrix, int expectedMinimumLength, int pos) {
    catchNull(aMatrix);
    if (aMatrix.length < expectedMinimumLength) {
      throw new MathsException("Argument passed in position " + pos + " does not contain a sufficient quantity of data to continue. STOPPING");
    }
  }





}
