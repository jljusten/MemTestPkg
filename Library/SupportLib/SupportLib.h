/** @file
  Memory test support library

  Copyright (c) 2006 - 2009, Intel Corporation
  Copyright (c) 2009, Jordan Justen
  All rights reserved.

  This program and the accompanying materials are licensed and made
  available under the terms and conditions of the BSD License which
  accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS"
  BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER
  EXPRESS OR IMPLIED.

**/

#ifndef _SUPPORT_LIB_H_INCLUDED_
#define _SUPPORT_LIB_H_INCLUDED_

#include <Library/MemTestSupportLib.h>

typedef struct {
  TEST_MEM_RANGE           RangeTest;
  VOID                     *Context;
} MEM_RANGE_TEST_DATA;

typedef struct {
  CHAR16                *Name;
  RUN_MEM_TEST          RunMemTest;
  VOID                  *Context;
} MEM_TEST_INSTANCE;

#endif

