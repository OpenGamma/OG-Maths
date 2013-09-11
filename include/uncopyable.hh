/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _UNCOPYABLE_HH
#define _UNCOPYABLE_HH

class Uncopyable {
  protected:
    Uncopyable() = default;
    ~Uncopyable() = default;
  private:
    Uncopyable(const Uncopyable&) = delete;
    Uncopyable& operator=(const Uncopyable&) = delete;
};

#endif // _UNCOPYABLE_HH
