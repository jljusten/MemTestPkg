/** @file
  User interface functions for Mem Test application

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemTestUiLib.h>
#include <Library/MemTestSupportLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>


//
// Global variables
//
STATIC UINT64 mProgressTotal;
STATIC UINTN  mLastPercent;

//
// Define the maximum message length that this library supports
//
#define MAX_MESSAGE_LENGTH  0x100

/**
  Sets the name of test

  @param[in] TestName    The name of the test.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
VOID
MtUiSetTestName (
  IN CHAR16        *TestName
  )
{
  Print (L"Test: %s\n", TestName);
}


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
  )
{
  CHAR16   Buffer[MAX_MESSAGE_LENGTH];
  VA_LIST  Marker;
  UINTN    Return;

  //
  // If Format is NULL, then ASSERT().
  //
  ASSERT (Format != NULL);

  //
  // Convert the DEBUG() message to a Unicode String
  //
  VA_START (Marker, Format);
  Return = UnicodeVSPrint (Buffer, MAX_MESSAGE_LENGTH,  Format, Marker);
  VA_END (Marker);

  //
  // Send the print string to the Console Output device
  //
  if ((gST != NULL) && (gST->ConOut != NULL)) {
    gST->ConOut->OutputString (gST->ConOut, Buffer);
  }

  return Return;
}


/**
  Indicates the total size of a pass through the memory test.
  
**/
VOID
EFIAPI
MtUiSetProgressTotal (
  IN UINT64   Total
  )
{
  mLastPercent = 0;
  mProgressTotal = Total;
}


/**
  Allows the memory test to indicate progress to the UI library.

**/
VOID
EFIAPI
MtUiUpdateProgress (
  IN UINT64   Progress
  )
{
  UINTN Percent;

  ASSERT (Progress < mProgressTotal);

  Percent = 
    (UINTN) DivU64x64Remainder (
              MultU64x32 (Progress, 100),
              mProgressTotal,
              NULL
              );
  if ((Percent / 5) > (mLastPercent / 5)) {
    MtUiPrint (L"%d%%\n", Percent);
    if (gBS->CheckEvent (gST->ConIn->WaitForKey) == EFI_SUCCESS) {
      MtSupportAbortTesting ();
    }
  }
  mLastPercent = Percent;
}


