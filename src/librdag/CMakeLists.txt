#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#
add_subdirectory(test)

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wl,--no-undefined")
SET(CMAKE_CC_FLAGS  "${CMAKE_CC_FLAGS} -Wl,--no-undefined -frepo")

set(RDAG_SOURCES entrypt.cc expression.cc execution.cc terminal.cc containers.cc numeric.cc register.cc
                 dispatch.cc equals.cc numerictypes.cc convertto.cc runners.cc)

add_library(rdag SHARED ${RDAG_SOURCES})

set_target_properties(rdag PROPERTIES SOVERSION ${longdog_VERSION_MAJOR} VERSION ${longdog_VERSION})
install(TARGETS rdag DESTINATION lib)

jar_native_library(rdag)
