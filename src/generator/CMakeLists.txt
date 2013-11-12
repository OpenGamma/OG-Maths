#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

file(MAKE_DIRECTORY ${BIN_INCLUDE_DIR})

# We maintain the list of generator files so that generated targets depend on them.
# Then, when the generator changes, cmake knows to re-run the generator.
set(GENERATOR ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generator.py)
set(GENERATOR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/generator.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/runnertemplates.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/dispatchtemplates.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/exprtemplates.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/dispatch.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/runners.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/expression.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/exprtree.py
                    ${CMAKE_CURRENT_SOURCE_DIR}/enums.py)

# Runners generation.

set(RUNNERS_HH ${BIN_INCLUDE_DIR}/runners.hh)
set(RUNNERS_CC ${CMAKE_BINARY_DIR}/src/librdag/runners.cc)

add_custom_command(OUTPUT ${RUNNERS_HH}
                   COMMAND ${GENERATOR} -o ${RUNNERS_HH} --runners-hh
                   DEPENDS ${GENERATOR_FILES}
                   COMMENT "Generating runners.hh")

add_custom_command(OUTPUT ${RUNNERS_CC}
                   DEPENDS ${RUNNERS_HH} ${GENERATOR_FILES}
                   COMMAND ${GENERATOR} -o ${RUNNERS_CC} --runners-cc
                   COMMENT "Generating runners.cc")

# A custom_target is needed to act as the bottleneck for parallel builds
# Otherwise, the generator is invoked multiple times concurrently.
add_custom_target(runners_cc DEPENDS ${RUNNERS_CC})

# Dispatch generation.

set(DISPATCH_HH ${BIN_INCLUDE_DIR}/dispatch.hh)
set(DISPATCH_CC ${CMAKE_BINARY_DIR}/src/librdag/dispatch.cc)

add_custom_command(OUTPUT ${DISPATCH_HH}
                   COMMAND ${GENERATOR} -o ${DISPATCH_HH} --dispatch-hh
                   DEPENDS ${GENERATOR_FILES}
                   COMMENT "Generating dispatch.hh")

add_custom_command(OUTPUT ${DISPATCH_CC}
                   DEPENDS ${DISPATCH_HH} ${GENERATOR_FILES}
                   COMMAND ${GENERATOR} -o ${DISPATCH_CC} --dispatch-cc
                   COMMENT "Generating dispatch.cc")

# Custom target to create bottleneck on parallel builds.
add_custom_target(dispatch_cc DEPENDS ${DISPATCH_CC})

# Expression generation.

set(EXPRESSION_HH ${BIN_INCLUDE_DIR}/expression.hh)
set(EXPRESSION_CC ${CMAKE_BINARY_DIR}/src/librdag/expression.cc)

add_custom_command(OUTPUT ${EXPRESSION_HH}
                   COMMAND ${GENERATOR} -o ${EXPRESSION_HH} --expression-hh
                   DEPENDS ${GENERATOR_FILES}
                   COMMENT "Generating expression.hh")

add_custom_command(OUTPUT ${EXPRESSION_CC}
                   COMMAND ${GENERATOR} -o ${EXPRESSION_CC} --expression-cc
                   DEPENDS ${EXPRESSION_HH} ${GENERATOR_FILES}
                   COMMENT "Generating expression.cc")

# Custom target bottleneck.
add_custom_target(expression_cc DEPENDS ${EXPRESSION_CC})

# Numeric generation.

set(NUMERIC_HH ${BIN_INCLUDE_DIR}/numeric.hh)
set(NUMERIC_CC ${CMAKE_BINARY_DIR}/src/librdag/numeric.cc)

add_custom_command(OUTPUT ${NUMERIC_HH}
                   COMMAND ${GENERATOR} -o ${NUMERIC_HH} --numeric-hh
                   DEPENDS ${GENERATOR_FILES}
                   COMMENT "Generating numeric.hh")

add_custom_command(OUTPUT ${NUMERIC_CC}
                   COMMAND ${GENERATOR} -o ${NUMERIC_CC} --numeric-cc
                   DEPENDS ${NUMERIC_HH} ${GENERATOR_FILES}
                   COMMENT "Generating numeric.cc")

# Custom target bottleneck.
add_custom_target(numeric_cc DEPENDS ${NUMERIC_CC})

# Expression enum generation

set(EXPRENUM_HH ${BIN_INCLUDE_DIR}/exprenum.hh)

add_custom_command(OUTPUT ${EXPRENUM_HH}
                   COMMAND ${GENERATOR} -o ${EXPRENUM_HH} --exprenum-hh
                   DEPENDS ${GENERATOR_FILES}
                   COMMENT "Generating exprenum.hh")

# Custom target bottleneck
add_custom_target(exprenum_hh DEPENDS ${EXPRENUM_HH})

set(RDAG_GENERATED ${RUNNERS_CC} ${DISPATCH_CC} ${EXPRESSION_CC} ${NUMERIC_CC} PARENT_SCOPE)
