/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _ENTRYPT_H
#define _ENTRYPT_H

#include "numeric.hh"
#include "terminal.hh"

namespace librdag {

OGTerminal::Ptr entrypt(OGNumeric::Ptr expr);

} // namespace librdag

#endif
