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

} // namespace librdag

#endif // _EXPRESSIONBASE_HH
