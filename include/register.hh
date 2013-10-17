/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _REGISTER_HH
#define _REGISTER_HH

#include "terminal.hh"

namespace librdag {

/**
 * The Register interface
 */
class Register
{
  public:
    Register();
    virtual ~Register();
    virtual int incRef();
    virtual int decRef();
    virtual OGNumeric * copy() const = 0;
  private:
    virtual void alloc() = 0;
    virtual void free() = 0;
    int _refCount;
};

class OGRealScalarRegister: public OGRealScalar, public Register
{
  public:
    OGRealScalarRegister();
    virtual OGNumeric * copy() const override;
  private:
    virtual void alloc() override;
    virtual void free() override;
};

class OGComplexScalarRegister: public OGComplexScalar, public Register
{
  public:
    OGComplexScalarRegister();
    virtual OGNumeric * copy() const override;
  private:
    virtual void alloc() override;
    virtual void free() override;
};

class OGRealMatrixRegister: public OGRealMatrix, public Register
{
  public:
    OGRealMatrixRegister(int rows, int cols);
    virtual OGNumeric * copy() const override;
  private:
    virtual void alloc() override;
    virtual void free() override;
};

class OGComplexMatrixRegister: public OGComplexMatrix, public Register
{
  public:
    OGComplexMatrixRegister(int rows, int cols);
    virtual OGNumeric * copy() const override;    
  private:
    virtual void alloc() override;
    virtual void free() override;
};

class OGRealDiagonalMatrixRegister: public OGRealDiagonalMatrix, public Register
{
  public:
    OGRealDiagonalMatrixRegister(int rows, int cols);
    virtual OGNumeric * copy() const override;    
  private:
    virtual void alloc() override;
    virtual void free() override;
};

class OGComplexDiagonalMatrixRegister: public OGComplexDiagonalMatrix, public Register
{
  public:
    OGComplexDiagonalMatrixRegister(int rows, int cols);
    virtual OGNumeric * copy() const override;    
  private:
    virtual void alloc() override;
    virtual void free() override;
};
/*
class OGRealSparseMatrixRegister
{
  private:
    virtual void alloc() override;
    virtual void free() override;
};

class OGComplexSparseMatrixRegister
{
  private:
    virtual void alloc() override;
    virtual void free() override;
};
*/
} // namespace librdag

#endif
