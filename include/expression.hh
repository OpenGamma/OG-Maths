/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXPRESSION_HH
#define _EXPRESSION_HH

// bindings
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <memory>

#include "numeric.hh"
#include "visitor.hh"
#include "exceptions.hh"
#include "containers.hh"
#include "register.hh"

using namespace std;


/**
 * The namespace for the DAG library
 */
namespace librdag
{

/**
 * Container for expression arguments
 */

typedef OwningPtrVector<const OGNumeric*> ArgContainer;

/**
 *  Expr type
 */
class OGExpr: public OGNumeric
{
  public:
    virtual ~OGExpr();
    const ArgContainer* getArgs() const;
    size_t getNArgs() const;
    virtual const OGExpr* asOGExpr() const override;    
    virtual void debug_print() const override;
    virtual void accept(Visitor &v) const override;
    virtual const RegContainer * getRegs() const;
  protected:
    OGExpr(ArgContainer* args);
  private:
    ArgContainer * _args;
    RegContainer * _regs;
};

/**
 * Things that extend OGExpr
 */

class OGUnaryExpr: public OGExpr
{
  protected:
    OGUnaryExpr(ArgContainer* args);
};

class OGBinaryExpr : public OGExpr
{
  protected:
    OGBinaryExpr(ArgContainer* args);
};

class COPY: public OGUnaryExpr
{
  public:
    COPY(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const COPY* asCOPY() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;      
};


class PLUS: public OGBinaryExpr
{
  public:
    PLUS(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const PLUS* asPLUS() const override;
    virtual void debug_print() const override;     
    virtual ExprType_t getType() const override;      
};


class NEGATE: public OGUnaryExpr
{
  public:
    NEGATE(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const NEGATE* asNEGATE() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;      
};

class SVD: public OGUnaryExpr
{
  public:
    SVD(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const SVD* asSVD() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;      
};

class SELECTRESULT: public OGBinaryExpr
{
  public:
    SELECTRESULT(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const SELECTRESULT* asSELECTRESULT() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;      
};


} // namespace librdag

#endif
