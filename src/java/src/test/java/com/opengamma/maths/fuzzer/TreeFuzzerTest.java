package com.opengamma.maths.fuzzer;

import static org.testng.Assert.assertTrue;

import org.testng.annotations.Test;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.other.OGLUResult;
import com.opengamma.maths.datacontainers.other.OGSVDResult;
import com.opengamma.maths.materialisers.Materialisers;

/**
 * Tests the TreeFuzzer class
 */
public class TreeFuzzerTest {

  @Test
  public void testTreeSize() {
    long rngSeed = 0L;
    int maxNewNodes = 10;
    int maxDataSize = 5;
    TreeFuzzer fuzzer = new TreeFuzzer(rngSeed, maxNewNodes, maxDataSize);

    // create some trees and assert size is as expected
    OGNumeric tree;

    // Tree is
    //  PLUS
    //  /  \
    // 10  20
    tree = DOGMA.plus(DOGMA.D(10), DOGMA.D(20));
    assertTrue(fuzzer.treeSize(tree, 0) == 3);

    // Tree is
    //     PLUS
    //    /    \
    //  PLUS   PLUS
    //  /  \   /  \
    // 10  20 10  20
    tree = DOGMA.plus(tree, tree);
    assertTrue(fuzzer.treeSize(tree, 0) == 7);

    // Tree is
    //
    // SELECTRESULT (OGIntegerScalar = 1)
    //       |
    //      LU
    //       |
    //     PLUS
    //    /    \
    //  PLUS   PLUS
    //  /  \   /  \
    // 10  20 10  20
    OGLUResult res = DOGMA.lu(tree);
    tree = res.getU();
    assertTrue(fuzzer.treeSize(tree, 0) == 10);
  }

  @Test
  public void checkFuzzer() {
    long rngSeed = 0L;
    int maxTreeRefs = 100;
    int maxDataSize = 10;
    TreeFuzzer fuzzer = new TreeFuzzer(rngSeed, maxTreeRefs, maxDataSize);
    fuzzer.fuzz(60);
  }

  @Test
  public void bug() {
    OGRealDenseMatrix foo = new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 1, 3);
    OGSVDResult res = DOGMA.svd(foo);
    DOGMA.toOGTerminal(res.getVT());
  }
}
