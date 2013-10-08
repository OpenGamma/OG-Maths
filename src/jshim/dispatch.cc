/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "dispatch.hh"
#include "visitor.hh"
#include "warningmacros.h"
#include "terminal.hh"
#include "expression.hh"

namespace convert {

using namespace librdag;

class DispatchToReal16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToReal16ArrayOfArrays() override
    {
      real16 ** arr = this->getData();
      if(arr)
      {

        for(int i=0; i<this->getRows(); i++)
        {
          delete arr[i];
        }
        delete arr;
      }
    }
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr)");
    };
    virtual void visit(librdag::OGScalar<real16> const *thing) override
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(1);
      this->setCols(1);
    }
    virtual void visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<complex16>)");
    }
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int>)");
    }
    virtual void visit(librdag::OGMatrix<real16> const *thing) override
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());
    }
    virtual void visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16>)");
    }
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing) override
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());
    }
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16>)");
    }
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing) override
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());
    }
    virtual void visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16>)");
    }
    void setData(real16 ** data)
    {
      this->_data = data;
    }
    void setRows(int rows)
    {
      this->rows = rows;
    }
    void setCols(int cols)
    {
      this->cols = cols;
    }
    real16 ** getData()
    {
      return this->_data;
    }
    int getRows()
    {
      return this->rows;
    }
    int getCols()
    {
      return this->cols;
    }
  private:
    real16 ** _data = NULL;
    int rows;
    int cols;
};

class DispatchToComplex16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToComplex16ArrayOfArrays() override
    {
      complex16 ** arr = this->getData();
      if(arr)
      {

        for(int i=0; i<this->getRows(); i++)
        {
          delete arr[i];
        }
        delete arr;
      }
    }
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr)");
    };
    virtual void visit(librdag::OGScalar<complex16> const *thing) override
    {
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(1);
      this->setCols(1);
    }
    virtual void visit(librdag::OGScalar<real16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<real16>)");
    }
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int>)");
    }
    virtual void visit(librdag::OGMatrix<complex16> const *thing) override
    {
      printf("Visitor: librdag::OGMatrix<complex16> branch\n");
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());
    }
    virtual void visit(librdag::OGMatrix<real16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<real16>)");
    }
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const *thing) override
    {
      printf("Visitor: librdag::OGMatrix<complex16> branch\n");
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());
    }
    virtual void visit(librdag::OGDiagonalMatrix<real16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<real16>)");
    }
    virtual void visit(librdag::OGSparseMatrix<complex16> const *thing) override
    {
      printf("Visitor: librdag::OGMatrix<complex16> branch\n");
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());
    }
    virtual void visit(librdag::OGSparseMatrix<real16> const SUPPRESS_UNUSED *thing) override
    {
      throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<real16>)");
    }
    void setData(complex16 ** data)
    {
      this->_data = data;
    }
    void setRows(int rows)
    {
      this->rows = rows;
    }
    void setCols(int cols)
    {
      this->cols = cols;
    }
    complex16 ** getData()
    {
      return this->_data;
    }
    int getRows()
    {
      return this->rows;
    }
    int getCols()
    {
      return this->cols;
    }
  private:
    complex16 ** _data = NULL;
    int rows;
    int cols;
};

OGTerminalPtrContainer_t<real16>* dispatchToReal16ArrayOfArrays(const librdag::OGTerminal* terminal)
{
  DispatchToReal16ArrayOfArrays *visitor = new DispatchToReal16ArrayOfArrays();
  terminal->accept(*visitor);
  OGTerminalPtrContainer_t<real16>* result = new OGTerminalPtrContainer_t<real16>();
  result->data = visitor->getData();
  result->rows = visitor->getRows();
  result->cols = visitor->getCols();
  result->colPtr = nullptr;
  result->rowIdx = nullptr;
  return result;
}

OGTerminalPtrContainer_t<complex16>* dispatchToComplex16ArrayOfArrays(const librdag::OGTerminal* terminal)
{
  DispatchToComplex16ArrayOfArrays *visitor = new DispatchToComplex16ArrayOfArrays();
  terminal->accept(*visitor);
  OGTerminalPtrContainer_t<complex16>* result = new OGTerminalPtrContainer_t<complex16>();
  result->data = visitor->getData();
  result->rows = visitor->getRows();
  result->cols = visitor->getCols();
  result->colPtr = nullptr;
  result->rowIdx = nullptr;
  return result;
}

} // namespace convert
