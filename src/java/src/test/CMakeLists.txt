#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#
set(JARNAME ${PROJECT_NAME}-${longdog_VERSION}-tests)
set(LONGDOG ${PROJECT_NAME}-${longdog_VERSION})

set(LONGDOGJAR ${JARDIR}/${LONGDOG}.jar)
set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${LONGDOGJAR})
set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${JARNAME})

file(GLOB_RECURSE JAR_SOURCES *.java)

add_jar(${JARNAME} ${JAR_SOURCES} INCLUDE_JARS ${LONGDOG} OUTPUT_DIR ${JARDIR})
