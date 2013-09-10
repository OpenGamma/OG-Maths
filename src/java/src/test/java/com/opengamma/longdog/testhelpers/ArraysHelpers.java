package com.opengamma.longdog.testhelpers;

import java.util.Arrays;

import com.opengamma.longdog.helpers.Catchers;

/**
 * Helper class for arrays
 */
public class ArraysHelpers {

  /**
   * Returns true if the two arrays are numerically equal
   * @param array1 the first array
   * @param array2 the second array
   * @return true if the two arrays are numerically equal
   */
  public static boolean ArraysEquals(double[][] array1, double[][] array2) {
    Catchers.catchNullFromArgList(array1, 1);
    Catchers.catchNullFromArgList(array2, 2);
    if (array1 == array2)
    {
      return true;
    }
    int rows = array1.length;
    if (rows != array2.length)
    {
      return false;
    }
    for (int i = 0; i < rows; i++) {
      if (!Arrays.equals(array1[i], array2[i])) {
        return false;
      }
    }
    return true;
  }
}
