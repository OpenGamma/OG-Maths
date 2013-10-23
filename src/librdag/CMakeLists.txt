#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#
add_subdirectory(test)

set(BIN_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${BIN_INCLUDE_DIR})

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS} ${BIN_INCLUDE_DIR})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wl,--no-undefined")
SET(CMAKE_CC_FLAGS  "${CMAKE_CC_FLAGS} -Wl,--no-undefined -frepo")

set(RUNNERS_HH ${BIN_INCLUDE_DIR}/runners.hh)
set(RUNNERS_CC ${CMAKE_BINARY_DIR}/src/librdag/runners.cc)
set(RUNNERS_GENERATOR ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generator.py)

set(GENERATOR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/generator.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/templates.py)

add_custom_command(OUTPUT ${RUNNERS_HH}
                   COMMAND ${RUNNERS_GENERATOR} -o ${RUNNERS_HH} --runners-hh
                   DEPENDS ${GENERATOR_FILES}
                   COMMENT "Generating runners.hh")

add_custom_command(OUTPUT ${RUNNERS_CC}
                   DEPENDS ${RUNNERS_HH} ${GENERATOR_FILES}
                   COMMAND ${RUNNERS_GENERATOR} -o ${RUNNERS_CC} --runners-cc
                   COMMENT "Generating runners.cc")

set(RDAG_SOURCES entrypt.cc expression.cc execution.cc terminal.cc containers.cc numeric.cc register.cc
                 dispatch.cc equals.cc numerictypes.cc convertto.cc ${RUNNERS_CC})

add_library(rdag SHARED ${RDAG_SOURCES})

set_target_properties(rdag PROPERTIES SOVERSION ${longdog_VERSION_MAJOR} VERSION ${longdog_VERSION})
install(TARGETS rdag DESTINATION lib)

jar_native_library(rdag)
