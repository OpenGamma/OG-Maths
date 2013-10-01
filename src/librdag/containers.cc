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
PtrVector<cTp>::PtrVector()
{
  _vector = new vector<cTp>();
}

template<typename cTp>
PtrVector<cTp>::~PtrVector()
{
  delete _vector;
}

template<typename cTp>
void
PtrVector<cTp>::push_back(cTp arg)
{
  _check_arg(arg);
  _vector->push_back(arg);
}

template<typename cTp>
size_t
PtrVector<cTp>::size() const
{
  return _vector->size();
}

template<typename cTp>
typename PtrVector<cTp>::citerator
PtrVector<cTp>::begin() const
{
  return _vector->begin();
}

template<typename cTp>
typename PtrVector<cTp>::citerator
PtrVector<cTp>::end() const
{
  return _vector->end();
}

template<typename cTp>
cTp
PtrVector<cTp>::operator[](size_t n) const
{
  return (*_vector)[n];
}

template<typename cTp>
void
PtrVector<cTp>::_check_arg(cTp arg)
{
  if (arg == nullptr)
  {
    throw new librdagException();
  }
}

template class PtrVector<const int*>;
template class PtrVector<const OGNumeric*>;

/**
 * NonOwningPtrVector
 */

template<typename cTp>
NonOwningPtrVector<cTp>*
NonOwningPtrVector<cTp>::copy() const
{
  NonOwningPtrVector* c = new NonOwningPtrVector();
  for (auto it = this->begin(); it != this->end(); ++it)
  {
    c->push_back(*it);
  }
  return c;
}

template class NonOwningPtrVector<const int*>;
template class NonOwningPtrVector<const OGNumeric*>;

/**
 * OwningPtrVector
 */

template<typename cTp>
OwningPtrVector<cTp>::~OwningPtrVector()
{
  for (auto it = this->begin(); it != this->end(); ++it)
  {
    delete *it;
  }
}

namespace detail {

/**
 * OwningPtrVector expects classes to implement the copy() method - this is
 * fine.  However, fundamental types do not implement copy, but we know how to
 * copy them. ptrvector_copy_impl defines an implementation for classes using
 * copy() and an implementation for fundamental types that manually copies the
 * data.
 */

template<typename cTp, bool Q = is_fundamental<typename remove_pointer<cTp>::type>::value >
struct owningptrvector_copy_impl;

template<typename cTp>
struct owningptrvector_copy_impl<cTp, false>
{
  OwningPtrVector<cTp>* operator()(const OwningPtrVector<cTp>* src)
  {
    OwningPtrVector<cTp>* c = new OwningPtrVector<cTp>();
    for (auto it = src->begin(); it != src->end(); ++it)
    {
      c->push_back((*it)->copy());
    }
    return c;
  }
};

template<typename cTp>
struct owningptrvector_copy_impl<cTp, true>
{
  OwningPtrVector<cTp>* operator()(const OwningPtrVector<cTp>* src)
  {
    OwningPtrVector<cTp>* c = new OwningPtrVector<cTp>();
    for (auto it = src->begin(); it != src->end(); ++it)
    {
      typedef typename remove_pointer<cTp>::type cT;
      typedef typename remove_const<cT>::type T;
      typedef T* Tp;
      Tp n = new T;
      *n = **it;
      c->push_back(n);
    }
    return c;
  }
};

} // namespace detail

template<typename cTp>
OwningPtrVector<cTp>*
OwningPtrVector<cTp>::copy() const
{
  return detail::owningptrvector_copy_impl<cTp>()(this);
}

template class OwningPtrVector<const int*>;
template class OwningPtrVector<const OGNumeric*>;

} // namespace librdag
