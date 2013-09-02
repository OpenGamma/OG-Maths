#ifndef EXPRTYPEENUM_H
#define EXPRTYPEENUM_H

/*
 * Enum for the various nodes in the system.
 * Systems wishing to use this code must have nodes that
 * a) match these values
 * b) have some converter to get to the C++ nodes and tree structre
 */
typedef enum
{

  UNKNOWNEXPR_ENUM = 0x0000L ,

  /*
  * Data container tokens
  */
  OGREALSCALAR_ENUM             = 0x0001L ,
  OGCOMPLEXSCALAR_ENUM          = 0X0002L ,
  OGREALMATRIX_ENUM             = 0X0003L ,
  OGCOMPLEXMATRIX_ENUM          = 0X0004L ,
  OGREALSPARSEMATRIX_ENUM       = 0X0005L ,
  OGCOMPLEXSPARSEMATRIX_ENUM    = 0X0006L ,
  OGREALDIAGONALMATRIX_ENUM     = 0X0007L ,
  OGCOMPLEXDIAGONALMATRIX_ENUM  = 0X0008L ,
  OGLOGICALMATRIX_ENUM          = 0X0009L ,

  /*
  * EXPR TOKENS FOR FUNCTIONS
  */

  PLUS_ENUM            = 0X0100L ,
  MINUS_ENUM           = 0X0200L ,
  TIMES_ENUM           = 0X0300L ,
  LDIVIDE_ENUM         = 0X0400L ,
  MTIMES_ENUM          = 0X0500L ,
  COPY_ENUM            = 0X0600L ,
  SELECTRESULT_ENUM    = 0X0700L ,
  SVD_ENUM             = 0X0800L 

} ExprType_t;

#endif
