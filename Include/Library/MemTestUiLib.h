/** @file
  Memory Test UI library class interface

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

#ifndef _MT_UI_LIB_H_INCLUDED_
#define _MT_UI_LIB_H_INCLUDED_

#include <Uefi.h>
#include <Library/UefiLib.h>


/**
  Sets the name of test

  @param[in] TestName    The name of the test.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
VOID
MtUiSetTestName (
  IN CHAR16        *TestName
  );


/** 
  This function behaves the same at the UefiLib Print function,
  but the location and presentation may be altered by the UiLib
  instance.

  @param Format   Null-terminated Unicode format string.
  @param ...      Variable argument list whose contents are accessed based 
                  on the format string specified by Format.
  
  @return Number of Unicode characters printed to ConOut.

**/
UINTN
EFIAPI
MtUiPrint (
  IN CONST CHAR16  *Format,
  ...
  );

#endif

