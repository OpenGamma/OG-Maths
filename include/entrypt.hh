/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _ENTRYPT_H
#define _ENTRYPT_H
#include "terminal.hh"

namespace librdag {

class OGNumeric;
class OGTerminal;

const OGTerminal* entrypt(const OGNumeric* expr);

} // namespace librdag

#endif
