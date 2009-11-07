/** @file
  Patterns Memory Tests

  Copyright (c) 2009, Intel Corporation
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

#include "Patterns.h"


STATIC PATTERN_TEST_DATA mPatterns[] = {
  PATTERN_MEM_TEST8 (L"0's", 0x0),
  PATTERN_MEM_TEST8 (L"1's", 0xff),
  PATTERN_MEM_TEST8 (L"5A", 0x5a),
  PATTERN_MEM_TEST8 (L"A5", 0xa5),
  PATTERN_MEM_TEST32 (L"dead beef", 0xdeadbeef),
};

STATIC
EFI_STATUS
EFIAPI
RunPatternMemTest (
  IN EFI_PHYSICAL_ADDRESS     Start,
  IN UINT64                   Length,
  IN VOID                     *Context
  )
{
  UINT64             Pattern;
  UINT64             *Check;
  UINT64             *End;

  Pattern = *(UINT64*) Context;

  SetMem64 ((VOID*)(UINTN)Start, Length, Pattern);

  MtWbinvd ();

  End = (UINT64*)(UINTN) (Start + Length);
  for (Check = (UINT64*)(UINTN)Start; Check < End; Check++) {
    if (*Check != Pattern) {
      MtUiPrint (L"Failed at %p\n", Check);
    }
  }

  return EFI_SUCCESS;
}


/**
  Memory Test Constructor.

  @return  EFI_STATUS
**/
EFI_STATUS
EFIAPI
PatternMemTestConstructor (
  )
{
  UINTN Loop;

  for (Loop = 0; Loop < (sizeof(mPatterns) / sizeof (mPatterns[0])); Loop++) {
    MtSupportInstallMemoryRangeTest (
      mPatterns[Loop].Name,
      RunPatternMemTest,
      (VOID*) &(mPatterns[Loop].Pattern)
      );
  }

  return EFI_SUCCESS;
}


