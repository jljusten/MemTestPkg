/** @file
  MemTest EFI Shell Application.

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

#ifndef _MEM_TEST_H_INCLUDED_
#define _MEM_TEST_H_INCLUDED_

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Library/MemTestUiLib.h>
#include <Library/MemTestRangesLib.h>


EFI_STATUS
EFIAPI
MtSupportRunAllTests (
  );


#endif

