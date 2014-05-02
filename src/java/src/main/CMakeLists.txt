#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

set(JPACKAGEFQN "com.opengamma.maths")
fqn_to_dir(JPACKAGEDIR ${JPACKAGEFQN})

set(JAVA_FILES
    materialisers/Materialisers.java
    helpers/Catchers.java
    helpers/DenseMemoryManipulation.java
    helpers/FuzzyEquals.java
    helpers/Iss.java
    helpers/MatrixPrimitiveUtils.java
    DOGMA.java
    nodes/COPY.java
    nodes/CTRANSPOSE.java
    nodes/LU.java
    nodes/MTIMES.java
    nodes/NEGATE.java
    nodes/NODE.java
    nodes/NORM2.java
    nodes/PINV.java
    nodes/PLUS.java
    nodes/SELECTRESULT.java
    nodes/SVD.java
    nodes/TRANSPOSE.java
    nativeloader/SupportedInstructionSet.java
    nativeloader/NativeLibraries.java
    nativeloader/NativeLibrariesSelfTest.java
    exceptions/MathsException.java
    exceptions/MathsExceptionIllegalArgument.java
    exceptions/MathsExceptionNative.java
    exceptions/MathsExceptionNativeComputation.java
    exceptions/MathsExceptionNativeConversion.java
    exceptions/MathsExceptionNativeUnspecified.java
    exceptions/MathsExceptionNonConformance.java
    exceptions/MathsExceptionNotImplemented.java
    exceptions/MathsExceptionNullPointer.java
    exceptions/MathsExceptionOnInitialization.java
    exceptions/MathsExceptionUnsupportedPlatform.java
    datacontainers/matrix/OGSparseMatrix.java
    datacontainers/matrix/OGComplexDiagonalMatrix.java
    datacontainers/matrix/OGComplexDenseMatrix.java
    datacontainers/matrix/OGRealDiagonalMatrix.java
    datacontainers/matrix/OGDiagonalMatrix.java
    datacontainers/matrix/OGDenseMatrix.java
    datacontainers/matrix/OGComplexSparseMatrix.java
    datacontainers/matrix/OGLogicalMatrix.java
    datacontainers/matrix/OGRealDenseMatrix.java
    datacontainers/matrix/OGRealSparseMatrix.java
    datacontainers/matrix/OGArray.java
    datacontainers/ExprEnum.java
    datacontainers/OGTerminal.java
    datacontainers/other/OGResult.java
    datacontainers/other/ComplexArrayContainer.java
    datacontainers/other/OGSVDResult.java
    datacontainers/other/OGLUResult.java
    datacontainers/lazy/OGExpr.java
    datacontainers/lazy/OGExprVariadicReturn.java
    datacontainers/OGNumeric.java
    datacontainers/scalar/OGScalar.java
    datacontainers/scalar/OGRealScalar.java
    datacontainers/scalar/OGComplexScalar.java
    datacontainers/scalar/OGIntegerScalar.java
    )

foreach(JFILE ${JAVA_FILES})
  set(JAR_SOURCES ${JAR_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/java/${JPACKAGEDIR}/${JFILE})
endforeach()

add_jar(${JAR_NAME_NO_EXTN} ${JAR_SOURCES}
        RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_BINARY_DIR}/verinfo.yaml
        INCLUDE_NATIVE TRUE
        DEPENDS verinfo
        ENTRY_POINT com/opengamma/maths/nativeloader/NativeLibrariesSelfTest
        OUTPUT_DIR ${JARDIR})

foreach(TARGET ${TARGET_TYPES})
  if(SUPPORT_${TARGET})
    add_test(${NATIVE_SELF_TEST}_${TARGET} ${Java_JAVA_EXECUTABLE} -DinstructionSet=${TARGET} -jar ${PATH_AND_JAR_NAME})
  endif()
endforeach()

add_test(${NATIVE_SELF_TEST} ${Java_JAVA_EXECUTABLE} -jar ${PATH_AND_JAR_NAME})

# Build source JAR
add_custom_target(sources ALL
                  COMMAND ${Java_JAR_EXECUTABLE} -cf ${PATH_AND_JAR_SOURCES_NAME}
                          com
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/java)

set(JPACKAGEFQN "com.opengamma.maths.datacontainers")
set(JAVAH_CLASSES OGNumeric)

set(JNIDIR ${CMAKE_BINARY_DIR}/src/nativeheaders)
foreach(JHCLASS ${JAVAH_CLASSES})
  jni_header_target(${JPACKAGEFQN} ${JHCLASS} ${JAR_NAME} ${JARDIR} ${JNIDIR})
endforeach()

set(JPACKAGEFQN "com.opengamma.maths.datacontainers.matrix")
set(JAVAH_CLASSES OGComplexDiagonalMatrix)

foreach(JHCLASS ${JAVAH_CLASSES})
  jni_header_target(${JPACKAGEFQN} ${JHCLASS} ${JAR_NAME} ${JARDIR} ${JNIDIR})
endforeach()

create_javadoc_jar(${JAR_NAME_NO_EXTN}
                   PACKAGES com.opengamma.maths
                   SOURCEPATH "${CMAKE_CURRENT_SOURCE_DIR}/java"
                   WINDOWTITLE "OG-Maths"
                   DOCTITLE "<h1>OG-Maths</h1>"
                   AUTHOR TRUE
                   USE TRUE
                   VERSION TRUE)
