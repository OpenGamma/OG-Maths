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
 * NonOwningPtrVector
 */

template<typename T>
NonOwningPtrVector<T>::NonOwningPtrVector()
{
  _vector = new vector<T const *>();
}

template<typename T>
NonOwningPtrVector<T>::~NonOwningPtrVector()
{
  delete _vector;
}

template<typename T>
void
NonOwningPtrVector<T>::push_back(T const * arg)
{
  _check_arg(arg);
  _vector->push_back(arg);
}

template<typename T>
size_t
NonOwningPtrVector<T>::size() const
{
  return _vector->size();
}

template<typename T>
typename NonOwningPtrVector<T>::citerator
NonOwningPtrVector<T>::begin() const
{
  return _vector->begin();
}

template<typename T>
typename NonOwningPtrVector<T>::citerator
NonOwningPtrVector<T>::end() const
{
  return _vector->end();
}

template<typename T>
const T*
NonOwningPtrVector<T>::operator[](size_t n) const
{
  return (*_vector)[n];
}

template<typename T>
NonOwningPtrVector<T>*
NonOwningPtrVector<T>::copy() const
{
  NonOwningPtrVector* c = new NonOwningPtrVector();
  for (auto it = this->begin(); it != this->end(); ++it)
  {
    c->push_back(*it);
  }
  return c;
}

template<typename T>
void
NonOwningPtrVector<T>::emptyVector()
{
  for (auto it = _vector->begin(); it != _vector->end(); ++it)
  {
    delete *it;
  }
}

template<typename T>
void
NonOwningPtrVector<T>::_check_arg(T const * arg)
{
  if (arg == nullptr)
  {
    throw new librdagException();
  }
}

template class NonOwningPtrVector<int>;
template class NonOwningPtrVector<OGNumeric>;

/**
 * PtrVector
 */

template<typename T>
PtrVector<T>::~PtrVector()
{
  NonOwningPtrVector<T>::emptyVector();
}

namespace detail {

/**
 * PtrVector expects classes to implement the copy() method - this is fine.
 * However, fundamental types do not implement copy, but we know how to copy
 * them. ptrvector_copy_impl defines an implementation for classes using copy()
 * and an implementation for fundamental types that manually copies the data.
 */

template<typename T, bool Q = is_fundamental<T>::value >
struct ptrvector_copy_impl;

template<typename T>
struct ptrvector_copy_impl<T, false>
{
  PtrVector<T>* operator()(const PtrVector<T>* src)
  {
    PtrVector<T>* c = new PtrVector<T>();
    for (auto it = src->begin(); it != src->end(); ++it)
    {
      c->push_back((*it)->copy());
    }
    return c;
  }
};

template<typename T>
struct ptrvector_copy_impl<T, true>
{
  PtrVector<T>* operator()(const PtrVector<T>* src)
  {
    PtrVector<T>* c = new PtrVector<T>();
    for (auto it = src->begin(); it != src->end(); ++it)
    {
      T* n = new T;
      *n = **it;
      c->push_back(n);
    }
    return c;
  }
};

} // namespace detail

template<typename T>
PtrVector<T>*
PtrVector<T>::copy() const
{
  return detail::ptrvector_copy_impl<T>()(this);
}

template class PtrVector<int>;
template class PtrVector<OGNumeric>;

} // namespace librdag
