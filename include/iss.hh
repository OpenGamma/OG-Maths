/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _ISS_HH
#define _ISS_HH
#include "terminal.hh"
#include "modifiermacros.h"

namespace librdag {

/**
 * Returns true if an OGTerminal is a scalar type.
 * @param term an OGTerminal to test.
 * @return true if \a term is a scalar type, false else.
 */
bool isScalar(const OGTerminal::Ptr& term);

/**
 * Returns true if an OGTerminal is a matrix type.
 * @param term an OGTerminal to test.
 * @return true if \a term is a matrix type, false else.
 */
bool isMatrix(const OGTerminal::Ptr& term);

/**
 * Returns true if an OGTerminal has vector dimensions (i.e. single row or single column).
 * @param term an OGTerminal to test.
 * @return true if \a term has vector dimensions, false else.
 */
bool isVector(const OGTerminal::Ptr& term);

/**
 * Returns true if an OGTerminal is a real space OGTerminal type.
 * @param term an OGTerminal to test.
 * @return true if \a term is a real space OGTerminal type, false else.
 */
bool isReal(const OGTerminal::Ptr& term);

/**
 * Returns true if an OGTerminal is a complex space OGTerminal type.
 * @param term an OGTerminal to test.
 * @return true if \a term is a complex space OGTerminal type, false else.
 */
bool isComplex(const OGTerminal::Ptr& term);

/**
 * Returns true if the data is finite (i.e. not NaN and not +/-Inf).
 * @param T the type of the data, real8 and complex16 are valid.
 * @param data the data to test.
 * @param n the number of items in \a data.
 * @return true if the data is finite else false.
 */
template<typename T>
bool isfinite(T * data, int4 n);

} // end namespace librdag

#endif // _ISS_HH
