/**

  Bit Shift Memory Test - Provides a memory test that produces walking bits
  with inversions...

  Copyright (c) 2006 - 2009, Intel Corporation
  Copyright (c) 2009, Erik Bjorge
  All rights reserved.

  This program and the accompanying materials are licensed and made
  available under the terms and conditions of the BSD License which
  accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS"
  BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER
  EXPRESS OR IMPLIED.

**/

#ifndef _MEM_TEST_BIT_SHIFT_H_INCLUDED_
#define _MEM_TEST_BIT_SHIFT_H_INCLUDED_

#include <Library/MemTestSupportLib.h>

typedef struct {
  UINT64                   Pattern;
  CHAR16                   *Name;
} PATTERN_TEST_DATA;

#define CREATE_PATTERN_UINT64(Value) \
    ((UINT64)Value)
#define CREATE_PATTERN_UINT32(Value) \
    CREATE_PATTERN_UINT64 (((UINT64)((UINT32) Value)) | ((((UINT64)((UINT32)(Value))) << 32)))
#define CREATE_PATTERN_UINT16(Value) \
    CREATE_PATTERN_UINT32 (((UINT32)((UINT16) Value)) | ((((UINT32)((UINT16)(Value))) << 16)))
#define CREATE_PATTERN_UINT8(Value) \
    CREATE_PATTERN_UINT16 (((UINT16)((UINT8) Value)) | ((((UINT16)((UINT8)(Value))) << 8)))

#define BIT(a) (((UINT64) 1) << (a))

STATIC
EFI_STATUS
EFIAPI
RunBitShiftMemTest (
  IN EFI_PHYSICAL_ADDRESS     Start,
  IN UINT64                   Length,
  IN UINTN                    PassNumber,
  IN VOID                     *Context
  );

#endif

