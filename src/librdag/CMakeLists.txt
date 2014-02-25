#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include_directories (${NATIVE_HEADER_DIR} ${CMAKE_SOURCE_DIR}/include ${JNIDIR} ${JNI_INCLUDE_DIRS} ${BIN_INCLUDE_DIR})
SET(CMAKE_FC_FLAGS  "${CMAKE_FC_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -cpp" )
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wl,--no-undefined")
SET(CMAKE_CC_FLAGS  "${CMAKE_CC_FLAGS} -Wl,--no-undefined -frepo")

set_source_files_properties(${RDAG_GENERATED} PROPERTIES GENERATED TRUE)

# The RDAG library.

set(RDAG_SOURCES containers.cc
                 convertto.cc
                 customrunners.cc
                 entrypt.cc
                 equals.cc
                 execution.cc
                 expressionbase.cc
                 iss.cc
                 numericbase.cc
                 numerictypes.cc
                 terminal.cc
                 ${RDAG_GENERATED})

include(${LAPACK_BUILDINFO})

add_multitarget_library(rdag
                        VERSION ${longdog_VERSION}
                        SOVERSION ${longdog_VERSION_MAJOR}
                        SOURCES ${RDAG_SOURCES}
                        DEPENDS runners_cc dispatch_cc expression_cc numeric_cc exprenum_hh
                        LINK_MULTILIBRARIES oglapack ogblas ogxerbla
                        TARGETS ${TARGET_TYPES})

jar_native_multitarget_library(rdag TARGETS ${TARGET_TYPES})

jar_native_multitarget_library(oglapack TARGETS ${TARGET_TYPES})

jar_native_multitarget_library(ogblas TARGETS ${TARGET_TYPES})

jar_native_multitarget_library(ogxerbla TARGETS ${TARGET_TYPES})

add_subdirectory(test)
