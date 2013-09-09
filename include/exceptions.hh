/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXCEPTIONS_HH
#define _EXCEPTIONS_HH

#include <exception>

using namespace std;

namespace librdag {

/*
 * General exception thing
 */
class librdagException: public exception
{
  public:
    virtual const char* what() const throw();
};

} // namespace librdag

#endif
