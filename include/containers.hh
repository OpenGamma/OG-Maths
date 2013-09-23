/*
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * See distribution for licence.
 *
 */

#ifndef _CONTAINERS_HH
#define _CONTAINERS_HH

#include <vector>
#include "exceptions.hh"
#include "warningmacros.h"

using namespace std;

namespace librdag {

/*
 * A vector that holds pointers. Null pointers are not allowed.
 *
 * Data that a PtrVector points to is owned by it.
 */
template<typename T>
class PtrVector
{
  public:
    PtrVector()
    {
      _vector = new vector<T*>();
    }

    ~PtrVector()
    {
      for (auto it = _vector->begin(); it != _vector->end(); ++it)
      {
        delete *it;
      }
      delete _vector;
    }

    typedef typename vector<T*>::const_iterator citerator;

    void push_back(T* arg)
    {
      _check_arg(arg);
      _vector->push_back(arg);
    }

    size_t size() const
    {
      return _vector->size();
    }

    citerator begin() const
    {
      return _vector->begin();
    }

    citerator end() const
    {
      return _vector->end();
    }

    const T* operator[](size_t n) const
    {
      return (*_vector)[n];
    }

    PtrVector* copy() const
    {
      PtrVector* c = new PtrVector();
      for (auto it = this->begin(); it != this->end(); ++it)
      {
        c->push_back((*it)->copy());
      }
      return c;
    }

  private:
    vector<T*>* _vector;
    void _check_arg(T* arg)
    {
      if (arg == nullptr)
      {
        throw new librdagException();
      }
    }
};

} // namespace librdag

#endif /* _CONTAINERS_HH */
