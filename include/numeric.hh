/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _NUMERIC_HH
#define _NUMERIC_HH

#include "uncopyable.hh"
#include "exprtypeenum.h"

namespace librdag {
  
// fwd decl  
class OGTerminal;
class OGRealScalar;
class OGComplexScalar;
class OGIntegerScalar;
class OGRealMatrix;
class OGComplexMatrix;
class OGRealDiagonalMatrix;
class OGComplexDiagonalMatrix;
class OGRealSparseMatrix;
class OGComplexSparseMatrix;
class Visitor;
class COPY;
class PLUS;
class NEGATE;
class SVD;
class SELECTRESULT;

/*
 * Base class for absolutely everything!
 */
class OGNumeric: private Uncopyable
{
  public:
    virtual ~OGNumeric();
    virtual void debug_print() const = 0;
    virtual void accept(Visitor &v) const = 0;
    virtual OGNumeric* copy() const = 0;
    virtual const COPY* asCOPY() const;
    virtual const PLUS* asPLUS() const;
    virtual const NEGATE* asNEGATE() const;
    virtual const SVD* asSVD() const;
    virtual const SELECTRESULT* asSELECTRESULT() const;
    virtual const OGTerminal* asOGTerminal() const;
    virtual const OGRealScalar* asOGRealScalar() const;
    virtual const OGComplexScalar* asOGComplexScalar() const;
    virtual const OGIntegerScalar* asOGIntegerScalar() const;
    virtual const OGRealMatrix* asOGRealMatrix() const;
    virtual const OGComplexMatrix* asOGComplexMatrix() const;
    virtual const OGRealDiagonalMatrix* asOGRealDiagonalMatrix() const;
    virtual const OGComplexDiagonalMatrix* asOGComplexDiagonalMatrix() const;
    virtual const OGRealSparseMatrix* asOGRealSparseMatrix() const;
    virtual const OGComplexSparseMatrix* asOGComplexSparseMatrix() const;
    virtual ExprType_t getType() const;
};

} 
#endif // _NUMERIC_HH