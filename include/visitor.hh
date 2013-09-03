/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
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
template  <typename T> class OGScalar;

/*
 * Pure function visitor class to access all fundamental types
 */
class Visitor
{
  public:
	  Visitor();
	  virtual ~Visitor();
    virtual void visit(OGExpr *thing) = 0;
    virtual void visit(OGArray<real16> *thing) = 0;
    virtual void visit(OGArray<complex16> *thing) = 0;
    virtual void visit(OGMatrix<real16> *thing) = 0;
    virtual void visit(OGScalar<real16> *thing) = 0;
    virtual void visit(OGScalar<complex16> *thing) = 0;
};

}

#endif /* VISITOR_HH_ */
