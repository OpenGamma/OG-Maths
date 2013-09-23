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
 * Data that a NonOwningPtrVector points to is not owned by it.
 */
template<typename T>
class NonOwningPtrVector
{
  public:
    NonOwningPtrVector();
    ~NonOwningPtrVector();
    typedef typename vector<T const *>::const_iterator citerator;
    void push_back(T const * arg);
    size_t size() const;
    citerator begin() const;
    citerator end() const;
    const T* operator[](size_t n) const;
    NonOwningPtrVector* copy() const;
  protected:
    void emptyVector();
  private:
    vector<T const *>* _vector;
    void _check_arg(T const * arg);
};

/*
 * A vector that holds pointers. Null pointers are not allowed.
 *
 * Data that a PtrVector points to is owned by it. Classes held in a PtrVector
 * must define a method copy() that returns a pointer to a copy of itself.
 */
template<typename T>
class PtrVector: public NonOwningPtrVector<T>
{
  public:
    ~PtrVector();
    PtrVector* copy() const;
};

class OGNumeric;

extern template class NonOwningPtrVector<int>;
extern template class NonOwningPtrVector<OGNumeric>;
extern template class PtrVector<int>;
extern template class PtrVector<OGNumeric>;

} // namespace librdag

#endif /* _CONTAINERS_HH */
