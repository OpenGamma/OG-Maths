/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _ENTRYPT_H
#define _ENTRYPT_H

struct c_OGNumeric;

#ifdef __cplusplus
extern "C" {
#endif
  struct c_OGNumeric * entrypt(struct c_OGNumeric * expr);
#ifdef __cplusplus
}
#endif



#endif
