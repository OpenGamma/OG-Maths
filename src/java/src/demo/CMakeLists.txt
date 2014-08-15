#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${PATH_AND_JAR_NAME})
set(CMAKE_JAVA_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH} ${JAR_TEST_NAME})

set(JPACKAGEFQN "com.opengamma.maths")
fqn_to_dir(JPACKAGEDIR ${JPACKAGEFQN})

set(JAVA_FILES
    demo/DemoInterface.java
    demo/MtimesVsTimesDemo.java
    demo/RunDemoTest.java
    demo/AdditionDemo.java
    demo/RunDemo.java
   )


foreach(JFILE ${JAVA_FILES})
  set(JAR_SOURCES ${JAR_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/java/${JPACKAGEDIR}/${JFILE})
endforeach()

add_jar(${JAR_TEST_NAME_NO_EXTN} ${JAR_SOURCES} INCLUDE_JARS ${JAR_NAME_NO_EXTN} OUTPUT_DIR ${JARDIR})
