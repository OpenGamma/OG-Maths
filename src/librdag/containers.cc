/*
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * See distribution for licence.
 *
 */

#include "containers.hh"
#include "expression.hh"

namespace librdag {

/**
 * PtrVector
 */

template<typename cTp>
PtrVector<cTp>::PtrVector(): _vector{}, _owning{false} {}

template<typename cTp>
PtrVector<cTp>::~PtrVector()
{
  if (_owning)
  {
    for (auto it = this->begin(); it != this->end(); ++it)
    {
      delete *it;
    }
  }
}

template<typename cTp>
void
PtrVector<cTp>::push_back(cTp arg)
{
  _check_arg(arg);
  _vector.push_back(arg);
}

template<typename cTp>
size_t
PtrVector<cTp>::size() const
{
  return _vector.size();
}

template<typename cTp>
typename PtrVector<cTp>::citerator
PtrVector<cTp>::begin() const
{
  return _vector.begin();
}

template<typename cTp>
typename PtrVector<cTp>::citerator
PtrVector<cTp>::end() const
{
  return _vector.end();
}

template<typename cTp>
cTp
PtrVector<cTp>::operator[](size_t n) const
{
  return _vector[n];
}

template<typename cTp>
void
PtrVector<cTp>::set_ownership(bool owning)
{
  _owning = owning;
}

template<typename cTp>
bool
PtrVector<cTp>::get_ownership() const
{
  return _owning;
}


template<typename cTp>
void
PtrVector<cTp>::_check_arg(cTp arg)
{
  if (arg == nullptr)
  {
    throw rdag_error("Attempt to put a null pointer in a PtrContainer");
  }
}

namespace detail {

/**
 * PtrVector expects classes to implement the copy() method - this is
 * fine. However, fundamental types do not implement copy, but we know how to
 * copy them. ptrvector_copy_impl defines an implementation for classes using
 * copy() and an implementation for fundamental types that manually copies the
 * data.
 */

template<typename cTp, bool Q = is_fundamental<typename remove_pointer<cTp>::type>::value >
struct ptrvector_copy_impl;

template<typename cTp>
struct ptrvector_copy_impl<cTp, false>
{
  PtrVector<cTp>* operator()(const PtrVector<cTp>* src)
  {
    PtrVector<cTp>* c = new PtrVector<cTp>();
    bool owning = src->get_ownership();
    for (auto it = src->begin(); it != src->end(); ++it)
    {
      if (owning)
      {
        c->push_back((*it)->copy());
      }
      else
      {
        c->push_back(*it);
      }
    }
    c->set_ownership(owning);
    return c;
  }
};

template<typename cTp>
struct ptrvector_copy_impl<cTp, true>
{
  PtrVector<cTp>* operator()(const PtrVector<cTp>* src)
  {
    PtrVector<cTp>* c = new PtrVector<cTp>();
    bool owning = src->get_ownership();
    for (auto it = src->begin(); it != src->end(); ++it)
    {
      if (owning)
      {
        typedef typename remove_pointer<cTp>::type cT;
        typedef typename remove_const<cT>::type T;
        typedef T* Tp;
        Tp n = new T;
        *n = **it;
        c->push_back(n);
      }
      else
      {
        c->push_back(*it);
      }
    }
    c->set_ownership(owning);
    return c;
  }
};

} // namespace detail

template<typename cTp>
PtrVector<cTp>*
PtrVector<cTp>::copy() const
{
  return detail::ptrvector_copy_impl<cTp>()(this);
}

template class PtrVector<const int*>;
template class PtrVector<const OGNumeric*>;

} // namespace librdag
