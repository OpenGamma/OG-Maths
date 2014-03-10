#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

file(GLOB_RECURSE JAR_SOURCES *.java)

add_jar(${JAR_NAME_NO_EXTN} ${JAR_SOURCES}
        RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/config
        INCLUDE_NATIVE TRUE
        DEPENDS ${jar_native_libraries}
        ENTRY_POINT com/opengamma/longdog/nativeloader/NativeLibrariesSelfTest
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

set(JPACKAGEFQN "com.opengamma.longdog.datacontainers")
set(JAVAH_CLASSES OGNumeric)

set(JNIDIR ${CMAKE_BINARY_DIR}/src/nativeheaders)
foreach(JHCLASS ${JAVAH_CLASSES})
  jni_header_target(${JPACKAGEFQN} ${JHCLASS} ${JAR_NAME} ${JARDIR} ${JNIDIR})
endforeach()

set(JPACKAGEFQN "com.opengamma.longdog.datacontainers.matrix")
set(JAVAH_CLASSES OGComplexDiagonalMatrix)

foreach(JHCLASS ${JAVAH_CLASSES})
  jni_header_target(${JPACKAGEFQN} ${JHCLASS} ${JAR_NAME} ${JARDIR} ${JNIDIR})
endforeach()

# create_javadoc_jar(${JARNAME}
#                PACKAGES com.opengamma.maths
#                SOURCEPATH "${CMAKE_CURRENT_SOURCE_DIR}/java"
#                WINDOWTITLE "longdog"
#                DOCTITLE "<h1>longdog</h1>"
#                AUTHOR TRUE
#                USE TRUE
#                VERSION TRUE)
