/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXECUTION_HH
#define _EXECUTION_HH

#include <vector>
#include "numeric.hh"

namespace librdag {

// To save expression.hh inclusion
class OGNumeric;

// Internal data structure for the ExecutionList.
typedef std::vector<pOGNumeric> _ExpressionList;

// An ExecutionList holds a list of expression nodes that are in order such that
// no node has inputs that are computed by a node further down the list. Thus,
// the list can be executed in sequence to compute an expression
class ExecutionList
{
  public:
    ExecutionList(pOGNumeric tree);
    ~ExecutionList();
    typedef typename _ExpressionList::const_iterator citerator;
    size_t size();
    citerator begin();
    citerator end();
    pOGNumeric operator[](size_t n);
  private:
    _ExpressionList* _execList;
    ExecutionList() = delete;
};

} // namespace librdag

#endif // _EXECUTION_HH
