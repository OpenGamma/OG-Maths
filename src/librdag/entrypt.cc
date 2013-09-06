#include <stdio.h>
#include "entrypt.h"
#include "expression.hh"
#include "exprtypeenum.h"
#include <typeinfo>
#include <iostream>

using namespace std;

namespace parser
{
/*
 * forward definition of the Walker class, this class essentially encapsulates walking
 * an OGNumeric tree.
 */
class Walker
{
    int level;
    librdag::Visitor * visitor;
  public:
    Walker();
    ~Walker();
    librdag::OGNumeric * walk(librdag::OGNumeric * numeric_expr_types);
    void talkandwalk(librdag::OGNumeric * numeric_expr_types);
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
    void visit(librdag::OGExpr *thing)
    {
      cout << "Have OGExpr type ";
      thing->debug_print();
      cout << "\n";
      std::vector<librdag::OGNumeric *> * tmp = thing->getArgs();
      //hmmm perhaps we should either store the args as a raw array, or insist on everything being a vector.
      // anyway, walk over given args
      std::vector<librdag::OGNumeric *>::iterator it;
      for(it = tmp->begin();  it != tmp->end(); it++)
      {
        _walker->talkandwalk(*(it));
      }
    };
    void visit(librdag::OGArray<real16> *thing)
    {
      cout << "Have OGArray<real16> " << thing << endl;
    }
    void visit(librdag::OGMatrix<real16> *thing)
    {
      cout << "Have OGMatrix<real16> " << thing << endl;
    }
    void visit(librdag::OGMatrix<complex16> *thing)
    {
      cout << "Have OGMatrix<complex16> " << thing << endl;
    }    
    void visit(librdag::OGArray<complex16> *thing)
    {
      cout << "Have OGArray<complex16> " << thing << endl;
    }
    void visit(librdag::OGScalar<real16> *thing)
    {
      cout << "Have OGScalar<real16> " << thing << endl;
    }
    void visit(librdag::OGScalar<complex16> *thing)
    {
      cout << "Have OGScalar<complex16> " << thing << endl;
    }
    void visit(librdag::OGScalar<int> *thing)
    {
      cout << "Have OGScalar<int> " << thing << endl;
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

librdag::OGNumeric * Walker::walk(librdag::OGNumeric * numeric_expr_types)
{
  printf("Walking DAG.\n");
  this->talkandwalk(numeric_expr_types);
  printf("Finished Walking DAG.\n");
  return numeric_expr_types;
}
void Walker::talkandwalk(librdag::OGNumeric * numeric_expr_types)
{
  level++;
  const char ch = ' ';
  cout << std::string(level*2,ch);
  numeric_expr_types->accept(*visitor);
  level--;
}


#ifdef __cplusplus
extern "C" {
#endif
  c_OGNumeric * entrypt(c_OGNumeric * expr)
  {
    printf("Accessing DAG walker.\n");
    Walker * walker = new Walker();
    walker->walk((librdag::OGNumeric *) expr);
    delete walker;
    printf("Returning from DAG walker.\n");
    return expr;
  }
#ifdef __cplusplus
}
#endif


} //end parser namespace
