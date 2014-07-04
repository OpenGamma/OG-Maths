/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

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

  // Type unknown
  UNKNOWN_EXPR_ENUM = 0x0000L ,

  // Is a node type mask
  IS_NODE_MASK = 0xFF00L,

  /*
  * Data container tokens, low 8 bits only, max prime 251_d = 0x00FB
  */
  REAL_SCALAR_ENUM              = 0x0002L ,
  COMPLEX_SCALAR_ENUM           = 0X0003L ,
  REAL_DENSE_MATRIX_ENUM        = 0X0005L ,
  COMPLEX_DENSE_MATRIX_ENUM     = 0X0007L ,
  REAL_SPARSE_MATRIX_ENUM       = 0X000BL ,
  COMPLEX_SPARSE_MATRIX_ENUM    = 0X000DL ,
  REAL_DIAGONAL_MATRIX_ENUM     = 0X0011L ,
  COMPLEX_DIAGONAL_MATRIX_ENUM  = 0X0013L ,
  LOGICAL_MATRIX_ENUM           = 0X0017L ,
  INTEGER_SCALAR_ENUM           = 0X001DL ,

  /*
  * EXPR TOKENS FOR FUNCTIONS, set in bits higher than 8, min prime 257_d = 0x0101
  */

  COPY_ENUM            = 0X010DL ,
  SELECTRESULT_ENUM    = 0X010FL ,
  NORM2_ENUM           = 0X0115L ,
  SVD_ENUM             = 0X0119L ,
  MTIMES_ENUM          = 0X011BL ,
  PINV_ENUM            = 0X0125L ,
  TRANSPOSE_ENUM       = 0x0133L ,
  CTRANSPOSE_ENUM      = 0x0137L ,
  LU_ENUM              = 0x0139L ,
  INV_ENUM             = 0x013DL ,
  MLDIVIDE_ENUM        = 0x014BL ,

#include "exprenum.hh"
} ExprType_t;

#endif

// MAGIC NUMBERS FOR TERMINALS
// D = H
// 2 = 2
// 3 = 3
// 5 = 5
// 7 = 7
// 11 = B
// 13 = D
// 17 = 11
// 19 = 13
// 23 = 17
// 29 = 1D
// 31 = 1F
// 37 = 25
// 41 = 29
// 43 = 2B
// 47 = 2F
// 53 = 35
// 59 = 3B
// 61 = 3D
// 67 = 43
// 71 = 47
// 73 = 49
// 79 = 4F
// 83 = 53
// 89 = 59
// 97 = 61
// 101 = 65
// 103 = 67
// 107 = 6B
// 109 = 6D
// 113 = 71
// 127 = 7F
// 131 = 83
// 137 = 89
// 139 = 8B
// 149 = 95
// 151 = 97
// 157 = 9D
// 163 = A3
// 167 = A7
// 173 = AD
// 179 = B3
// 181 = B5
// 191 = BF
// 193 = C1
// 197 = C5
// 199 = C7
// 211 = D3
// 223 = DF
// 227 = E3
// 229 = E5
// 233 = E9
// 239 = EF
// 241 = F1
// 251 = FB

// MAGIC NUMBERS FOR NODES
// D = H
// 257 = 101
// 263 = 107
// 269 = 10D
// 271 = 10F
// 277 = 115
// 281 = 119
// 283 = 11B
// 293 = 125
// 307 = 133
// 311 = 137
// 313 = 139
// 317 = 13D
// 331 = 14B
// 337 = 151
// 347 = 15B
// 349 = 15D
// 353 = 161
// 359 = 167
// 367 = 16F
// 373 = 175
// 379 = 17B
// 383 = 17F
// 389 = 185
// 397 = 18D
// 401 = 191
// 409 = 199
// 419 = 1A3
// 421 = 1A5
// 431 = 1AF
// 433 = 1B1
// 439 = 1B7
// 443 = 1BB
// 449 = 1C1
// 457 = 1C9
// 461 = 1CD
// 463 = 1CF
// 467 = 1D3
// 479 = 1DF
// 487 = 1E7
// 491 = 1EB
// 499 = 1F3
// 503 = 1F7
// 509 = 1FD
// 521 = 209
// 523 = 20B
// 541 = 21D
// 547 = 223
// 557 = 22D
// 563 = 233
// 569 = 239
// 571 = 23B
// 577 = 241
// 587 = 24B
// 593 = 251
// 599 = 257
// 601 = 259
// 607 = 25F
// 613 = 265
// 617 = 269
// 619 = 26B
// 631 = 277
// 641 = 281
// 643 = 283
// 647 = 287
// 653 = 28D
// 659 = 293
// 661 = 295
// 673 = 2A1
// 677 = 2A5
// 683 = 2AB
// 691 = 2B3
// 701 = 2BD
// 709 = 2C5
// 719 = 2CF
// 727 = 2D7
// 733 = 2DD
// 739 = 2E3
// 743 = 2E7
// 751 = 2EF
// 757 = 2F5
// 761 = 2F9
// 769 = 301
// 773 = 305
// 787 = 313
// 797 = 31D
// 809 = 329
// 811 = 32B
// 821 = 335
// 823 = 337
// 827 = 33B
// 829 = 33D
// 839 = 347
// 853 = 355
// 857 = 359
// 859 = 35B
// 863 = 35F
// 877 = 36D
// 881 = 371
// 883 = 373
// 887 = 377
// 907 = 38B
// 911 = 38F
// 919 = 397
// 929 = 3A1
// 937 = 3A9
// 941 = 3AD
// 947 = 3B3
// 953 = 3B9
// 967 = 3C7
// 971 = 3CB
// 977 = 3D1
// 983 = 3D7
// 991 = 3DF
// 997 = 3E5
// 1009 = 3F1
// 1013 = 3F5
// 1019 = 3FB
// 1021 = 3FD
// 1031 = 407
// 1033 = 409
// 1039 = 40F
// 1049 = 419
// 1051 = 41B
// 1061 = 425
// 1063 = 427
// 1069 = 42D
