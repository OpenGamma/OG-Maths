/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <stdio.h>
#include "entrypt.hh"
#include "dispatch.hh"
#include "numeric.hh"
#include "expression.hh"
#include "execution.hh"
#include "terminal.hh"
#include "exprtypeenum.h"
#include <typeinfo>
#include <iostream>
#include "lapack_raw.h"

using namespace std;

namespace librdag
{
/*
 * forward definition of the Walker class, this class essentially encapsulates walking
 * an OGNumeric tree.
 */
class Walker
{
    int level;
    Visitor * visitor;
  public:
    Walker();
    ~Walker();
    pOGNumeric walk(pOGNumeric numeric_expr_types);
    void talkandwalk(pOGNumeric numeric_expr_types);
};

/**
 * Visitor for printing info specific to tree nodes
 */
class PrintTreeVisitor: public librdag::Visitor
{
    Walker * _walker;
  public:
    PrintTreeVisitor(Walker * walker)
    {
      _walker = walker;
    }
    ~PrintTreeVisitor()
    {
      _walker = NULL;
    }
    virtual void visit(librdag::OGExpr const *thing) override
    {
      cout << "Have OGExpr type ";
      thing->debug_print();
      cout << "\n";
      const ArgContainer& tmp = thing->getArgs();
      //hmmm perhaps we should either store the args as a raw array, or insist on everything being a vector.
      // anyway, walk over given args
      for(auto it = tmp.begin();  it != tmp.end(); it++)
      {
        _walker->talkandwalk(*(it));
      }
    };
    void visit(librdag::OGScalar<real16> const *thing) override
    {
      cout << "Have OGScalar<real16> " << thing << endl;
    }
    void visit(librdag::OGScalar<complex16> const *thing) override
    {
      cout << "Have OGScalar<complex16> " << thing << endl;
    }
    void visit(librdag::OGScalar<int> const *thing) override
    {
      cout << "Have OGScalar<int> " << thing << endl;
    }
    void visit(librdag::OGMatrix<real16> const *thing) override
    {
      cout << "Have OGMatrix<real16> " << thing << endl;
    }
    void visit(librdag::OGMatrix<complex16> const *thing) override
    {
      cout << "Have OGMatrix<complex16> " << thing << endl;
    }
    void visit(librdag::OGDiagonalMatrix<real16> const *thing) override
    {
      cout << "Have OGDiagonalMatrix<real16> " << thing << endl;
    }
    void visit(librdag::OGDiagonalMatrix<complex16> const *thing) override
    {
      cout << "Have OGDiagonalMatrix<complex16> " << thing << endl;
    }
    void visit(librdag::OGSparseMatrix<real16> const *thing) override
    {
      cout << "Have OGSparseMatrix<real16> " << thing << endl;
    }
    void visit(librdag::OGSparseMatrix<complex16> const *thing) override
    {
      cout << "Have OGSparseMatrix<complex16> " << thing << endl;
    }
};

/*
 * Definition of Walker class
 * a bit hideous, Walker has internal state for "level" so we pass "this" to the PrintTreeVisitor
 */
Walker::Walker()
{
  level = 0;
  visitor = new PrintTreeVisitor(this);
}

Walker::~Walker()
{
  delete visitor;
}

pOGNumeric
Walker::walk(pOGNumeric numeric_expr_types)
{
  printf("Walking DAG.\n");
  this->talkandwalk(numeric_expr_types);
  printf("Finished Walking DAG.\n");
  return numeric_expr_types;
}

void
Walker::talkandwalk(pOGNumeric numeric_expr_types)
{
  level++;
  const char ch = ' ';
  cout << std::string(level*2,ch);
  numeric_expr_types->accept(*visitor);
  level--;
}

pOGTerminal
entrypt(pOGNumeric expr)
{
  // Sort out LAPACK so xerbla calls don't kill the processes.
  int zero = 0;
  set_xerbla_death_switch(&zero);

  pOGTerminal terminal = expr->asOGTerminal();
  if (terminal != pOGTerminal{})
  {
    // If we were passed a terminal, simply return it.
    return terminal;
  }
  else
  {
    ExecutionList el{expr};
    Dispatcher disp;
    
    DEBUG_PRINT("Dispatching from entrypt\n");

    for (auto it = el.begin(); it != el.end(); ++it)
    {
      pOGExpr expr = (*it)->asOGExpr();
      if (expr != pOGExpr{})
      {
        disp.dispatch(expr);
      }
    }

    const RegContainer& regs = expr->asOGExpr()->getRegs();
    if(regs[0]->asOGTerminal() == nullptr)
    {
      throw rdag_error("Evaluated terminal is not casting asOGTerminal correctly.");
    }

    return static_pointer_cast<const OGTerminal, const OGNumeric>(regs[0]);
  }
}

} // namespace librdag
