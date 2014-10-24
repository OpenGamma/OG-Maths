#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${PATH_AND_JAR_NAME})
set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${JAR_TEST_NAME})

set(JPACKAGEFQN "com.opengamma.maths")
fqn_to_dir(JPACKAGEDIR ${JPACKAGEFQN})

set(JAVA_FILES
      logging/TestSlf4jLogger.java
      logging/TestLogger.java
      logging/TestStderrLogger.java
      helpers/CatchersTest.java
      helpers/DenseMemoryManipulationTest.java
      helpers/FuzzyEqualsTest.java
      helpers/IssTest.java
      helpers/MatrixPrimitiveUtilsTest.java
      fuzzer/TreeFuzzerTest.java
      fuzzer/FuzzerMainTest.java
      fuzzer/FixedSizeFIFOTest.java
      testnodes/TestMldivideMaterialise.java
      testnodes/TestTimesMaterialise.java
      testnodes/TestNorm2Materialise.java
      testnodes/TestMtimesMaterialise.java
      testnodes/TestPlusMaterialise.java
      testnodes/TestInvMaterialise.java
      testnodes/TestSumcolsMaterialise.java
      testnodes/TestSumrowsMaterialise.java
      testnodes/TestSVDMaterialise.java
      testnodes/TestLUMaterialise.java
      testnodes/TestPinvMaterialise.java
      testnodes/TestTransposeMaterialise.java
      testnodes/TestPlusMaterialiseToComplex.java
      testnodes/TestCtransposeMaterialise.java
      testnodes/TestTimesMaterialiseToComplex.java
      testnodes/TestNegateMaterialise.java
      testnodes/TestQRMaterialise.java
      datacontainers/matrix/OGComplexDenseMatrixTest.java
      datacontainers/matrix/OGLogicalMatrixTest.java
      datacontainers/matrix/OGRealDiagonalMatrixTest.java
      datacontainers/matrix/OGComplexDiagonalMatrixTest.java
      datacontainers/matrix/OGRealDenseMatrixTest.java
      datacontainers/matrix/OGComplexSparseMatrixTest.java
      datacontainers/matrix/OGRealSparseMatrixTest.java
      datacontainers/other/OGResultTest.java
      datacontainers/other/ComplexArrayContainerTest.java
      datacontainers/scalars/OGIntegerScalarTest.java
      datacontainers/scalars/OGRealScalarTest.java
      datacontainers/scalars/OGComplexScalarTest.java
      testhelpers/ArraysHelpers.java
      DOGMATest.java
      materialisers/TestThreadedOGRealScalarMaterialise.java
      materialisers/TestOGRealDiagonalMatrixMaterialise.java
      materialisers/TestOGComplexSparseMatrixMaterialise.java
      materialisers/TestOGRealSparseMatrixMaterialise.java
      materialisers/TestOGRealDenseMatrixMaterialise.java
      materialisers/TestOGRealScalarMaterialise.java
      materialisers/TestOGComplexScalarMaterialise.java
      materialisers/TestOGComplexDiagonalMatrixMaterialise.java
      materialisers/TestOGComplexDenseMatrixMaterialise.java
    )

if(BITS64)
  set(JAVA_FILES
      ${JAVA_FILES}
      mem/StdlibTest.java
      mem/AlignedAlignedWriteBufferTest.java
      mem/AVXAlignedMemoryBlockTest.java
      )
endif()

foreach(JFILE ${JAVA_FILES})
  set(JAR_SOURCES ${JAR_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/java/${JPACKAGEDIR}/${JFILE})
endforeach()

add_jar(${JAR_TEST_NAME_NO_EXTN} ${JAR_SOURCES} INCLUDE_JARS ${JAR_NAME_NO_EXTN} OUTPUT_DIR ${JARDIR})
