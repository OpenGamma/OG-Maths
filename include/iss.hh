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

bool isScalar(const OGTerminal::Ptr&);
bool isMatrix(const OGTerminal::Ptr&);
bool isVector(const OGTerminal::Ptr&);
bool isReal(const OGTerminal::Ptr&);
bool isComplex(const OGTerminal::Ptr&);

} // end namespace librdage

#endif // _ISS_HH
