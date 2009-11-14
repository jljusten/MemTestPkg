/** @file
  Memory Test support library class interface

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

#ifndef _MEM_TEST_SUPPORT_LIB_H_INCLUDED_
#define _MEM_TEST_SUPPORT_LIB_H_INCLUDED_

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Library/MemTestUiLib.h>
#include <Library/MemTestRangesLib.h>

typedef
EFI_STATUS
(EFIAPI *TEST_MEM_RANGE)(
  IN EFI_PHYSICAL_ADDRESS     Start,
  IN UINT64                   Length,
  IN UINTN                    PassNumber,
  IN VOID                     *Context
  );

EFI_STATUS
EFIAPI
MtSupportInstallMemoryRangeTest (
  IN CHAR16             *Name,
  IN TEST_MEM_RANGE     TestRangeFunction,
  IN UINTN              NumberOfPasses,
  IN VOID               *Context
  );

typedef
EFI_STATUS
(EFIAPI *RUN_MEM_TEST)(
  IN VOID                     *Context
  );

EFI_STATUS
EFIAPI
MtSupportInstallMemoryTest (
  IN CHAR16                *Name,
  IN RUN_MEM_TEST          MemTestFunction,
  IN VOID                  *Context
  );

VOID
EFIAPI
MtWbinvd (
  VOID
  );

VOID
EFIAPI
MtSupportAbortTesting (
  VOID
  );

#endif

