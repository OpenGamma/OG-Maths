/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JDISPATCH_HH
#define _JDISPATCH_HH

#include "numerictypes.hh"
#include "expression.hh"
#include "terminal.hh"
#include "register.hh"
#include "warningmacros.h"
#include "exceptions.hh"

namespace convert {

using namespace librdag;  
  
class DispatchToReal16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToReal16ArrayOfArrays();
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED *thing);
    void setData(real16 ** data);
    void setRows(int rows);
    void setCols(int cols);
    real16 ** getData();
    int getRows();
    int getCols();
  private:
    real16 ** _data = nullptr;
    int rows;
    int cols;
    
};



class DispatchToComplex16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToComplex16ArrayOfArrays();
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED *thing);
    void setData(complex16 ** data);
    void setRows(int rows);
    void setCols(int cols);
    complex16 ** getData();
    int getRows();
    int getCols();
  private:
    complex16 ** _data = nullptr;
    int rows;
    int cols;    
};
  
} // namespace convert

#endif // _JDISPATCH_HH

