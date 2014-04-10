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
 * A vector that holds pointers. Null pointers are not allowed. If it owns its pointers,
 * then it deletes them on destruction.
 */
template<typename cTp>
class PtrVector
{
  static_assert(is_pointer<cTp>::value && is_const<typename remove_pointer<cTp>::type>::value,
                "Type cTp must be pointer to const type");
  public:
    PtrVector();
    virtual ~PtrVector();
    typedef typename vector<cTp>::const_iterator citerator;
    void push_back(cTp arg);
    size_t size() const;
    citerator begin() const;
    citerator end() const;
    cTp operator[](size_t n) const;
    virtual PtrVector* copy() const;
    void set_ownership(bool owning);
    bool get_ownership() const;
  private:
    vector<cTp> _vector;
    bool _owning;
    void _check_arg(cTp arg);
};

class OGNumeric;

extern template class PtrVector<const int*>;
extern template class PtrVector<const OGNumeric*>;

} // namespace librdag

#endif /* _CONTAINERS_HH */
