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

namespace convert
{

using namespace librdag;

DispatchToReal16ArrayOfArrays::~DispatchToReal16ArrayOfArrays()
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

void DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr)");
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<real16> const  *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(1);
    this->setCols(1);
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<complex16>)");
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int>)");
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<real16> const *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16>)");
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<real16> const *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<complex16>)");
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGSparseMatrix<real16> const *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGSparseMatrix<complex16>)");
}

void DispatchToReal16ArrayOfArrays::setData(real16 ** data)
{
    this->_data = data;
}

void DispatchToReal16ArrayOfArrays::setRows(int rows)
{
    this->rows = rows;
}

void DispatchToReal16ArrayOfArrays::setCols(int cols)
{
    this->cols = cols;
}

real16 ** DispatchToReal16ArrayOfArrays::getData()
{
    return this->_data;
}

int DispatchToReal16ArrayOfArrays::getRows()
{
    return this->rows;
}

int DispatchToReal16ArrayOfArrays::getCols()
{
    return this->cols;
}

DispatchToComplex16ArrayOfArrays::~DispatchToComplex16ArrayOfArrays()
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
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGExpr const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGExpr)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<complex16> const *thing)
{
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(1);
    this->setCols(1);
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<real16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<real16>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<int>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGMatrix<complex16> const *thing)
{
    printf("Visitor: librdag::OGMatrix<complex16> branch\n");
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGMatrix<real16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGMatrix<real16>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<complex16> const *thing)
{
    printf("Visitor: librdag::OGMatrix<complex16> branch\n");
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<real16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<real16>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGSparseMatrix<complex16> const *thing)
{
    printf("Visitor: librdag::OGMatrix<complex16> branch\n");
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGSparseMatrix<real16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGSparseMatrix<real16>)");
}
void DispatchToComplex16ArrayOfArrays::setData(complex16 ** data)
{
    this->_data = data;
}
void DispatchToComplex16ArrayOfArrays::setRows(int rows)
{
    this->rows = rows;
}
void DispatchToComplex16ArrayOfArrays::setCols(int cols)
{
    this->cols = cols;
}
complex16 ** DispatchToComplex16ArrayOfArrays::getData()
{
    return this->_data;
}
int DispatchToComplex16ArrayOfArrays::getRows()
{
    return this->rows;
}
int DispatchToComplex16ArrayOfArrays::getCols()
{
    return this->cols;
}
} // namespace convert
