/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _VISITOR_HH
#define _VISITOR_HH

#include "numerictypes.hh"

namespace librdag {

/*
 * Forward declare types for visitor class
 */
class OGExpr;
template  <typename T> class OGArray;
template  <typename T> class OGMatrix;
template  <typename T> class OGDiagonalMatrix;
template  <typename T> class OGSparseMatrix;
template  <typename T> class OGScalar;

/*
 * Pure function visitor class to access all fundamental types
 */
class Visitor
{
  public:
    Visitor() = default;
    virtual ~Visitor() = default;
    virtual void visit(OGExpr const *thing) = 0;
    virtual void visit(OGScalar<real16> const *thing) = 0;
    virtual void visit(OGScalar<complex16> const *thing) = 0;
    virtual void visit(OGScalar<int> const *thing) = 0;
    virtual void visit(OGMatrix<real16> const *thing) = 0;
    virtual void visit(OGMatrix<complex16> const *thing) = 0;
    virtual void visit(OGDiagonalMatrix<real16> const *thing) = 0;
    virtual void visit(OGDiagonalMatrix<complex16> const *thing) = 0;
    virtual void visit(OGSparseMatrix<real16> const *thing) = 0;
    virtual void visit(OGSparseMatrix<complex16> const *thing) = 0;
};

}

#endif /* VISITOR_HH_ */
