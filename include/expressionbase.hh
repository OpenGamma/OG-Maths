/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXPRESSIONBASE_HH
#define _EXPRESSIONBASE_HH

#include "numeric.hh"
#include "visitor.hh"
#include "containers.hh"

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
typedef OwningPtrVector<const OGNumeric*>  RegContainer;

/**
 *  Expr type
 */
class OGExpr: public OGNumeric
{
  public:
    virtual ~OGExpr();
    ArgContainer* getArgs() const;
    size_t getNArgs() const;
    virtual const OGExpr* asOGExpr() const override;
    virtual void debug_print() const override;
    virtual void accept(Visitor &v) const override;
    virtual RegContainer * getRegs() const;
    /**
     * Construct an expression node only.
     * NOTE: This merely constructs the object without wiring anything in.
     * Users need to wire up the \a ArgContainer and \a RegContainer manually.
     */
    OGExpr();
    /**
     * Set the ArgContainer member of this class as \a arg.
     */
    void setArgContainer(ArgContainer * arg);
    /**
     * Set the ArgContainer member of this class as \a reg.
     */
    void setRegContainer(RegContainer * reg);
    /**
     * Construct an expression with args
     * @param args the args
     */
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
  public:
    OGUnaryExpr();
    OGUnaryExpr(ArgContainer* args);
};

class OGBinaryExpr : public OGExpr
{
  protected:
    OGBinaryExpr(ArgContainer* args);
};

/**
 * Non-autogenerated nodes
 */

class COPY: public OGUnaryExpr
{
  public:
    using OGUnaryExpr::OGUnaryExpr;
    virtual OGNumeric* copy() const override;
    virtual const COPY* asCOPY() const override;
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

class NORM2: public OGUnaryExpr
{
  public:
    using OGUnaryExpr::OGUnaryExpr;
    virtual OGNumeric* copy() const override;
    virtual const NORM2* asNORM2() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;
};

} // namespace librdag

#endif // _EXPRESSIONBASE_HH
