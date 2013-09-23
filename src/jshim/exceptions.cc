/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "exceptions.hh"

namespace convert {

const char*
convertException::what() const throw()
{
  return "Exception occurred.";
}

} // namespace convert
