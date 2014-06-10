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
    DOGMATest.java
    testnodes/TestNorm2Materialise.java
    testnodes/TestPlusMaterialise.java
    testnodes/TestPlusMaterialiseToComplex.java
    testnodes/TestNegateMaterialise.java
    testnodes/TestSVDMaterialise.java
    testnodes/TestMtimesMaterialise.java
    materialisers/TestOGComplexDenseMatrixMaterialise.java
    materialisers/TestOGRealDenseMatrixMaterialise.java
    materialisers/TestThreadedOGRealScalarMaterialise.java
    materialisers/TestOGRealScalarMaterialise.java
    materialisers/TestOGRealDiagonalMatrixMaterialise.java
    materialisers/TestOGRealSparseMatrixMaterialise.java
    materialisers/TestOGComplexSparseMatrixMaterialise.java
    materialisers/TestOGComplexScalarMaterialise.java
    materialisers/TestOGComplexDiagonalMatrixMaterialise.java
    helpers/CatchersTest.java
    helpers/DenseMemoryManipulationTest.java
    helpers/FuzzyEqualsTest.java
    helpers/MatrixPrimitiveUtilsTest.java
    logging/TestLogger.java
    logging/TestStderrLogger.java
    logging/TestSlf4jLogger.java
    testhelpers/ArraysHelpers.java
    datacontainers/matrix/OGComplexDiagonalMatrixTest.java
    datacontainers/matrix/OGComplexDenseMatrixTest.java
    datacontainers/matrix/OGComplexSparseMatrixTest.java
    datacontainers/matrix/OGLogicalMatrixTest.java
    datacontainers/matrix/OGRealDiagonalMatrixTest.java
    datacontainers/matrix/OGRealSparseMatrixTest.java
    datacontainers/matrix/OGRealDenseMatrixTest.java
    datacontainers/scalars/OGComplexScalarTest.java
    datacontainers/scalars/OGIntegerScalarTest.java
    datacontainers/scalars/OGRealScalarTest.java
    datacontainers/other/OGResultTest.java
    datacontainers/other/ComplexArrayContainerTest.java
    fuzzer/FuzzerMainTest.java
    fuzzer/TreeFuzzerTest.java
    fuzzer/FixedSizeFIFOTest.java
    mem/StdlibTest.java
    mem/AlignedAlignedWriteBufferTest.java
    mem/AVXAlignedMemoryBlockTest.java
    )

foreach(JFILE ${JAVA_FILES})
  set(JAR_SOURCES ${JAR_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/java/${JPACKAGEDIR}/${JFILE})
endforeach()

add_jar(${JAR_TEST_NAME_NO_EXTN} ${JAR_SOURCES} INCLUDE_JARS ${JAR_NAME_NO_EXTN} OUTPUT_DIR ${JARDIR})
