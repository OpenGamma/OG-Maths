#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

# Convert a fully qualified package name to its directory structure. 
# Example FQN: com.opengamma.maths.nativewrappers
# Example DIR: com/opengamma/maths/nativewrappers
# (You need to append/prepend a slash yourself where necessary)
macro(fqn_to_dir DIR FQN)
  string(REGEX REPLACE "\\." "/" ${DIR} ${FQN})
endmacro(fqn_to_dir)

# Given a class in a package, what is the name of the corresponding native
# header file?
# Example: PACKAGE=com.opengamma.maths.nativewrappers, CLASS=OGBLASRawWrapper
# HFILE set to: com_opengamma_maths_nativewrappers_OGBLASRawWrapper.h
macro(native_header_file HFILE PACKAGE CLASS)
  string(REGEX REPLACE "\\." "_" PKGCNAME ${PACKAGE})
  set(${HFILE} ${PKGCNAME}_${CLASS}.h)
endmacro(native_header_file)

# Create a command for generating a JNI header file for the class CLASS with package
# name FQN, inside a jar file JAR. If the output directory for the header file is
# different to the current binary dir, it can be replaced with JNIDIR.
# Example: FQN=com.opengamma.maths.nativewrappers CLASS=OGBLASRawWrapper
#          JAR=OG-MathsWrappers-0.0.1,
#          JARDIR=${CMAKE_CURRENT_BINARY_DIR},
#          JNIDIR=${CMAKE_BINARY_DIR}/src/jni. 
# This will create a target generate_com_opengamma_maths_nativewrappers_OGBLASRawWrapper.h 
# that can be depended upon to force execution of javah to create the file.
macro(jni_header_target FQN CLASS JAR JARDIR JNIDIR)
  native_header_file(HEADER ${FQN} ${CLASS})
  if(NOT DEFINED JNIDIR)
    set(JNIDIR ${CMAKE_CURRENT_BINARY_DIR})
  endif(NOT DEFINED JNIDIR)
  set(JNIDIRARG -d ${JNIDIR})
  set(CPARG -classpath ${JARDIR}/${JAR}.jar)
  add_custom_command(OUTPUT ${JNIDIR}/${HEADER}
                     COMMAND ${Java_JAVAH_EXECUTABLE} -jni ${JNIDIRARG} ${CPARG} ${FQN}.${CLASS}
                     DEPENDS ${JAR})
  add_custom_target(generate_${HEADER} DEPENDS ${JNIDIR}/${HEADER})
endmacro(jni_header_target)
