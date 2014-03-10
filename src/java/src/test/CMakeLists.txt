#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${PATH_AND_JAR_NAME})
set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${JAR_TEST_NAME})

file(GLOB_RECURSE JAR_SOURCES *.java)
add_jar(${JAR_TEST_NAME_NO_EXTN} ${JAR_SOURCES} INCLUDE_JARS ${JAR_NAME_NO_EXTN} OUTPUT_DIR ${JARDIR})
