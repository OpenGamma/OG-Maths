#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#
set(JARNAME ${PROJECT_NAME}-${longdog_VERSION})

file(GLOB_RECURSE JAR_SOURCES *.java)

add_jar(${JARNAME} ${JAR_SOURCES} 
        RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/config
        INCLUDE_NATIVE TRUE
        DEPENDS ${jar_native_libraries}
        ENTRY_POINT com/opengamma/longdog/nativeloader/NativeLibrariesSelfTest
        OUTPUT_DIR ${JARDIR})

add_test(${NATIVE_SELF_TEST} ${Java_JAVA_EXECUTABLE} -jar ${JARDIR}/${JARNAME}.jar)

# Build source JAR
add_custom_target(sources ALL
                  COMMAND ${Java_JAR_EXECUTABLE} -cf ${JARDIR}/${JARNAME}-sources.jar
                          com
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/java)
  
set(JPACKAGEFQN "com.opengamma.longdog.datacontainers")
set(JAVAH_CLASSES OGNumeric)

set(JNIDIR ${CMAKE_BINARY_DIR}/src/nativeheaders)
foreach(JHCLASS ${JAVAH_CLASSES})
  jni_header_target(${JPACKAGEFQN} ${JHCLASS} ${JARNAME} ${JARDIR} ${JNIDIR})
endforeach()
                  
set(JPACKAGEFQN "com.opengamma.longdog.datacontainers.matrix")
set(JAVAH_CLASSES OGComplexDiagonalMatrix)

foreach(JHCLASS ${JAVAH_CLASSES})
  jni_header_target(${JPACKAGEFQN} ${JHCLASS} ${JARNAME} ${JARDIR} ${JNIDIR})
endforeach()
                 
# create_javadoc_jar(${JARNAME}
#                PACKAGES com.opengamma.maths
#                SOURCEPATH "${CMAKE_CURRENT_SOURCE_DIR}/java"
#                WINDOWTITLE "longdog"
#                DOCTITLE "<h1>longdog</h1>"
#                AUTHOR TRUE
#                USE TRUE
#                VERSION TRUE)
