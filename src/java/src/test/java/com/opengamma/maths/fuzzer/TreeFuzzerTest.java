package com.opengamma.maths.fuzzer;

import static org.testng.Assert.assertTrue;

import java.io.IOException;
import java.nio.file.Files;

import org.testng.annotations.Test;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.other.OGLUResult;
import com.opengamma.maths.datacontainers.other.OGSVDResult;
import com.opengamma.maths.helpers.TestGroups;

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
    assertTrue(fuzzer.treeRefCount(tree, 0) == 3);

    // Tree is
    //     PLUS
    //    /    \
    //  PLUS   PLUS
    //  /  \   /  \
    // 10  20 10  20
    tree = DOGMA.plus(tree, tree);
    assertTrue(fuzzer.treeRefCount(tree, 0) == 7);

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
    assertTrue(fuzzer.treeRefCount(tree, 0) == 10);
  }

  /*
   * The following two tests are demonstrations of the fuzzer interfaces 
   */

  @Test(groups = TestGroups.FUZZER)
  public void DemoSingleTreeFuzzer() {
    long rngSeed = 0L;
    int maxTreeRefs = 500;
    int maxDataSize = 20;
    long runtime = 5;
    TreeFuzzer fuzzer = new TreeFuzzer(rngSeed, maxTreeRefs, maxDataSize);
    fuzzer.fuzz(runtime);
  }

  @Test(groups = TestGroups.FUZZER)
  public void DemoThreadedTreeFuzzer() {
    int maxTreeRefs = 500;
    int maxDataSize = 20;
    int nthreads = 8;
    long runtime = 5;
    Fuzzer fuzzer = null;
    try {
      fuzzer = new ThreadedTreeFuzzer(runtime, maxTreeRefs, maxDataSize, nthreads, true, Files.createTempDirectory("og-maths-logs-").toFile());
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
    fuzzer.fuzz(runtime);
  }
}
