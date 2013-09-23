/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

/**
 * Holds references to data needed to construct a terminal type
 */

#include "numerictypes.hh"

namespace librdag {
class OGTerminal;
}

namespace convert {

template <typename T> struct OGTerminalPtrContainer_t
{
  T** data;
  int rows;
  int cols;
  int * colPtr;
  int * rowIdx;
};

OGTerminalPtrContainer_t<real16>* dispatchToReal16ArrayOfArrays(const librdag::OGTerminal* terminal);
OGTerminalPtrContainer_t<complex16>* dispatchToComplex16ArrayOfArrays(const librdag::OGTerminal* terminal);


} // namespace convert
