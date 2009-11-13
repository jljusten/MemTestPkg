/**

  Bit Shift Memory Test - Provides a memory test that produces walking bits
  with inversions...

  Copyright (c) 2009, Intel Corporation
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

#include "BitShift.h"

BOOLEAN
VerifyMemory (
    IN EFI_PHYSICAL_ADDRESS     Start,
    IN UINT64                   Length,
    IN UINT64                   Pattern
    );

STATIC PATTERN_TEST_DATA mBasePatterns[] = {
    {CREATE_PATTERN_UINT8(1),  L"Bit Shift: (BYTE)"},
    {CREATE_PATTERN_UINT16(1), L"Bit Shift: (WORD)"},
    {CREATE_PATTERN_UINT32(1), L"Bit Shift: (DWORD)"},
    {CREATE_PATTERN_UINT64(1), L"Bit Shift: (QWORD)"}
    };
STATIC UINT8 mBasePatternsCount = sizeof(mBasePatterns)/sizeof(PATTERN_TEST_DATA);

STATIC
EFI_STATUS
EFIAPI
RunBitShiftMemTest (
    IN EFI_PHYSICAL_ADDRESS     Start,
    IN UINT64                   Length,
    IN VOID                     *Context
    )
{
    UINT64                  End = 0;
    UINT64                  Pattern = 0;
    BOOLEAN                 Verify = TRUE;
    BOOLEAN                 TestDone = FALSE;                     

    //
    // Display range being tested
    //
    End = Start + Length;
    DEBUG ((DEBUG_INFO, "Testing Memory Range: 0x%016lx - 0x%016lx\n", Start, End));
    
    Pattern = *((UINT64*) Context);
    TestDone = FALSE;
        
    while (!TestDone) {
        //
        // Check to see if the test is complete
        //
        if (Pattern & BIT(63)) {
            TestDone = TRUE;
        }
            
        //
        // Perform normal pattern test
        //
        DEBUG ((DEBUG_INFO, "Pattern: 0x%016lx\n", Pattern));
        SetMem64 ((VOID*)(UINTN)Start, Length, Pattern);
        MtWbinvd ();
        if (VerifyMemory (Start, Length, Pattern)) {
            Verify = FALSE;
        }

        //
        // Perform inversion pattern test
        //
        Pattern = ~Pattern;
        DEBUG ((DEBUG_INFO, "Pattern: 0x%016lx\n", Pattern));
        SetMem64 ((VOID*)(UINTN)Start, Length, Pattern);
        MtWbinvd ();
        if (VerifyMemory (Start, Length, Pattern)) {
            Verify = FALSE;
        }

        //
        // Invert again and shift for next iteration
        //
        Pattern = ~Pattern;
        Pattern <<= 1;
    }

    return EFI_SUCCESS;
}

BOOLEAN
VerifyMemory (
    IN EFI_PHYSICAL_ADDRESS     Start,
    IN UINT64                   Length,
    IN UINT64                   Pattern
    )
{
    EFI_PHYSICAL_ADDRESS        *End;
    EFI_PHYSICAL_ADDRESS        *Check;
    BOOLEAN                     Failed = FALSE;

    //
    // Check that the memory regions match the fill pattern
    //
    End = (UINT64*)(UINTN) (Start + Length);
    Check = (EFI_PHYSICAL_ADDRESS*)(UINTN) Start;
    for (; Check < End; Check++) {
        if (*Check != Pattern) {
            MtUiPrint (L"    Failed at %016lx\n", Check);
            Failed = TRUE;
        }
    }

    return (!Failed);
}


//
//  MovingBitsInit () - Registers test function for the Moving Bits test
//
EFI_STATUS
EFIAPI
BitShiftInit (
  )
{
    UINT8   Index;
    
    for (Index = 0; Index < mBasePatternsCount; Index++) {
        MtSupportInstallMemoryRangeTest (
            mBasePatterns[Index].Name,
            RunBitShiftMemTest,
            (VOID*)&mBasePatterns[Index].Pattern
            );
    }

    return EFI_SUCCESS;
}


