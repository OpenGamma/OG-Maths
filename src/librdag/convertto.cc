
/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "convertto.hh"
#include "terminal.hh"
#include "warningmacros.h"
#include "exceptions.hh"
#include <cstring>

using namespace std;
namespace librdag {


ConvertTo::ConvertTo()
{}

// things that convert to OGRealDenseMatrix
OGRealDenseMatrix::Ptr
ConvertTo::convertToOGRealDenseMatrix(OGRealScalar::Ptr thing) const
{
  OGRealDenseMatrix::Ptr ret = OGRealDenseMatrix::create(new real8[1](),1,1, OWNER);
  ret->getData()[0] = thing->getValue();
  return ret;
}

OGRealDenseMatrix::Ptr
ConvertTo::convertToOGRealDenseMatrix(OGIntegerScalar::Ptr thing) const
{
  OGRealDenseMatrix::Ptr ret = OGRealDenseMatrix::create(new real8[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGRealDenseMatrix::Ptr
ConvertTo::convertToOGRealDenseMatrix(OGRealDiagonalMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  size_t wlen = thing->getDatalen();
  OGRealDenseMatrix::Ptr ret = OGRealDenseMatrix::create(new real8[rows*cols](),rows,cols, OWNER);
  real8 * diagdata = thing->getData();
  real8 * data = ret->getData();
  for(size_t i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGRealDenseMatrix::Ptr
ConvertTo::convertToOGRealDenseMatrix(OGLogicalMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  size_t wlen = thing->getDatalen();
  OGRealDenseMatrix::Ptr ret = OGRealDenseMatrix::create(new real8[wlen](),rows,cols, OWNER);
  real8 * thedata = thing->getData();
  real8 * data = ret->getData();
  memcpy(data,thedata,sizeof(real8)*wlen);
  return ret;
}


OGRealDenseMatrix::Ptr
ConvertTo::convertToOGRealDenseMatrix(OGRealSparseMatrix::Ptr thing) const
{

  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  int4 * colPtr = thing->getColPtr();
  int4 * rowIdx = thing->getRowIdx();
  real8 * sparsedata = thing->getData();

  OGRealDenseMatrix::Ptr ret = OGRealDenseMatrix::create(new real8[rows*cols](),rows,cols, OWNER);
  real8 * data = ret->getData();
  for (size_t ir = 0; ir < cols; ir++)
  {
    for (int4 i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}


// things that convert to OGComplexDenseMatrix

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGRealScalar::Ptr thing) const
{
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGIntegerScalar::Ptr thing) const
{
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGComplexScalar::Ptr thing) const
{
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGRealDiagonalMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  size_t wlen = thing->getDatalen();
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[rows*cols](),rows,cols, OWNER);
  real8 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(size_t i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGComplexDiagonalMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  size_t wlen = thing->getDatalen();
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[rows*cols](),rows,cols, OWNER);
  complex16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(size_t i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGRealSparseMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  int4 * colPtr = thing->getColPtr();
  int4 * rowIdx = thing->getRowIdx();
  real8 * sparsedata = thing->getData();
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[rows*cols](),rows,cols, OWNER);
  complex16 * data = ret->getData();
  for (size_t ir = 0; ir < cols; ir++)
  {
    for (int4 i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGComplexSparseMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  int4 * colPtr = thing->getColPtr();
  int4 * rowIdx = thing->getRowIdx();
  complex16 * sparsedata = thing->getData();
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[rows*cols](),rows,cols, OWNER);
  complex16 * data = ret->getData();
  for (size_t ir = 0; ir < cols; ir++)
  {
    for (int4 i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGRealDenseMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  size_t wlen = thing->getDatalen();
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[wlen](),rows,cols, OWNER);
  real8 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(size_t i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

OGComplexDenseMatrix::Ptr
ConvertTo::convertToOGComplexDenseMatrix(OGLogicalMatrix::Ptr thing) const
{
  size_t rows = thing->getRows();
  size_t cols = thing->getCols();
  size_t wlen = thing->getDatalen();
  OGComplexDenseMatrix::Ptr ret = OGComplexDenseMatrix::create(new complex16[wlen](),rows,cols, OWNER);
  real8 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(size_t i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

} // end namespace
