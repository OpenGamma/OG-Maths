/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "expression.hh"
#include "terminal.hh"
#include "execution.hh"

namespace librdag {

class Lineariser: public Visitor
{
  public:
    _ExpressionList* linearise(OGNumeric* tree)
    {
      _execlist = new _ExpressionList();
      tree->accept(*this);
      return _execlist;
    }

    virtual void visit(OGExpr const *tree) override
    {
      const ArgContainer* args = tree->getArgs();
      for (auto it = args->begin(); it != args->end(); ++it)
      {
        (*it)->accept(*this);
      }
      _execlist->push_back(tree);
    }

    virtual void visit(OGArray<real16> const *thing)
    {
      visit_terminal(thing);
    }

    virtual void visit(OGArray<complex16> const *thing)
    {
      visit_terminal(thing);
    }

    virtual void visit(OGMatrix<real16> const *thing)
    {
      visit_terminal(thing);
    }

    virtual void visit(OGMatrix<complex16> const *thing)
    {
      visit_terminal(thing);
    }

    virtual void visit(OGScalar<real16> const *thing)
    {
      visit_terminal(thing);
    }

    virtual void visit(OGScalar<complex16> const *thing)
    {
      visit_terminal(thing);
    }

    virtual void visit(OGScalar<int> const *thing)
    {
      visit_terminal(thing);
    }

  private:
    void visit_terminal(OGTerminal const *tree)
    {
      _execlist->push_back(tree);
    }
    _ExpressionList* _execlist;
};

ExecutionList::ExecutionList(OGNumeric* tree)
{
  _execList = Lineariser().linearise(tree);
}

ExecutionList::~ExecutionList()
{
  delete _execList;
}

size_t
ExecutionList::size()
{
  return _execList->size();
}

ExecutionList::citerator
ExecutionList::begin()
{
  return _execList->begin();
}

ExecutionList::citerator
ExecutionList::end()
{
  return _execList->end();
}

const OGNumeric*
ExecutionList::operator[](size_t n)
{
  return _execList->operator[](n);
}

} // namespace librdag
