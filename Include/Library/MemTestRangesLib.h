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
  Initializes the memory test ranges

  @param[in,out] Key  To retrieve the first range, set Key to 0 before calling.
                      To retrieve the next range, pass in the previous Key.

  @retval  EFI_NOT_FOUND  Indicates all ranges have been returned

**/
EFI_STATUS
MtRangesGetNextRange (
  IN OUT INTN                  *Key,
  OUT    EFI_PHYSICAL_ADDRESS  *Start,
  OUT    UINT64                *Length
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

