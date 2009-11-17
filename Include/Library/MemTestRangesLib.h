/** @file
  Memory Test Ranges library class interface

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

#ifndef _MT_RANGES_LIB_H_INCLUDED_
#define _MT_RANGES_LIB_H_INCLUDED_

#include <Uefi.h>
#include <Library/UefiLib.h>


/**
  Returns the total size of all test memory ranges

  @return  The total size of all test memory ranges

**/
UINT64
MtRangesGetTotalSize (
  VOID
  );


/**
  Gets the next test memory range

  @param[in,out] Key     To retrieve the first range, set Key to 0 before calling.
                         To retrieve the next range, pass in the previous Key.
  @param[out]    Start   Start of the next memory range
  @param[out]    Length  Length of the next memory range

  @retval  EFI_NOT_FOUND  Indicates all ranges have been returned
  @retval  EFI_SUCCESS    The next memory range was returned

**/
EFI_STATUS
MtRangesGetNextRange (
  IN OUT INTN                  *Key,
  OUT    EFI_PHYSICAL_ADDRESS  *Start,
  OUT    UINT64                *Length
  );


/**
  Lock memory range

  @param[in]    Start   Start of the memory range
  @param[in]    Length  Length of the memory range

  @retval  EFI_ACCESS_DENIED  The range could not be locked
  @retval  EFI_SUCCESS        The range was locked

**/
EFI_STATUS
EFIAPI
MtRangesLockRange (
  IN    EFI_PHYSICAL_ADDRESS  Start,
  IN    UINT64                Length
  );


/**
  Unlocks a memory range

  @param[in]    Start   Start of the memory range
  @param[in]    Length  Length of the memory range

  @retval  EFI_INVALID_PARAMETER  The range could not be unlocked
  @retval  EFI_SUCCESS            The range was unlocked

**/
VOID
EFIAPI
MtRangesUnlockRange (
  IN    EFI_PHYSICAL_ADDRESS  Start,
  IN    UINT64                Length
  );


/**
  Initializes the memory test ranges

**/
EFI_STATUS
MtRangesConstructor (
  );


/**
  Decontructs the memory test ranges data structures

**/
EFI_STATUS
MtRangesDeconstructor (
  );


#endif

