
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

// things that convert to OGRealMatrix
OGRealMatrix *
ConvertTo::convertToOGRealMatrix(OGRealScalar const * thing) const
{
  OGRealMatrix * ret = new OGRealMatrix(new real16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGRealMatrix *
ConvertTo::convertToOGRealMatrix(OGIntegerScalar const * thing) const
{
  OGRealMatrix * ret = new OGRealMatrix(new real16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGRealMatrix *
ConvertTo::convertToOGRealMatrix(OGRealDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGRealMatrix * ret = new OGRealMatrix(new real16[rows*cols](),rows,cols, OWNER);
  real16 * diagdata = thing->getData();
  real16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGRealMatrix *
ConvertTo::convertToOGRealMatrix(OGLogicalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGRealMatrix * ret = new OGRealMatrix(new real16[wlen](),rows,cols, OWNER);
  real16 * thedata = thing->getData();
  real16 * data = ret->getData();
  memcpy(data,thedata,sizeof(real16)*wlen);
  return ret;
}


OGRealMatrix *
ConvertTo::convertToOGRealMatrix(OGRealSparseMatrix const * thing) const
{

  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();

  OGRealMatrix * ret = new OGRealMatrix(new real16[rows*cols](),rows,cols, OWNER);
  real16 * data = ret->getData();
  for (int ir = 0; ir < cols; ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}


// things that convert to OGComplexMatrix

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealScalar const * thing) const
{
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGIntegerScalar const * thing) const
{
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGComplexScalar const * thing) const
{
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[1](),1,1, OWNER);
  ret->getData()[0]=thing->getValue();
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER);
  real16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGComplexDiagonalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER);
  complex16 * diagdata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i+i*rows]=diagdata[i];
  }
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealSparseMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  real16 * sparsedata = thing->getData();
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER);
  complex16 * data = ret->getData();
  for (int ir = 0; ir < cols; ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGComplexSparseMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int * colPtr = thing->getColPtr();
  int * rowIdx = thing->getRowIdx();
  complex16 * sparsedata = thing->getData();
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[rows*cols](),rows,cols, OWNER);
  complex16 * data = ret->getData();
  for (int ir = 0; ir < cols; ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      data[rowIdx[i] + ir * rows] = sparsedata[i];
    }
  }
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGRealMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[wlen](),rows,cols, OWNER);
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

OGComplexMatrix *
ConvertTo::convertToOGComplexMatrix(OGLogicalMatrix const * thing) const
{
  int rows = thing->getRows();
  int cols = thing->getCols();
  int wlen = thing->getDatalen();
  OGComplexMatrix * ret = new OGComplexMatrix(new complex16[wlen](),rows,cols, OWNER);
  real16 * densedata = thing->getData();
  complex16 * data = ret->getData();
  for(int i=0;i<wlen;i++)
  {
    data[i]=densedata[i];
  }
  return ret;
}

} // end namespace