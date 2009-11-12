/** @file
  Memory test support library

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

#include "SupportLib.h"


STATIC MEM_TEST_INSTANCE *mTests = NULL;
STATIC UINTN mTestCount = 0;
STATIC UINTN mMaxTestCount = 0;


STATIC
EFI_STATUS
EFIAPI
RunMemoryRangeTest (
  IN VOID                     *Context
  )
{
  EFI_STATUS            Status;
  INTN                  Key;
  MEM_RANGE_TEST_DATA   *Test;
  EFI_PHYSICAL_ADDRESS  Start;
  UINT64                Length;
  UINT64                LengthTested;
  UINT64                SubRangeLength;

  Test = (MEM_RANGE_TEST_DATA*) Context;

  Key = 0;
  Status = EFI_SUCCESS;
  while (TRUE) {
    Status = MtRangesGetNextRange (
               &Key,
               &Start,
               &Length
               );
    if (Status == EFI_NOT_FOUND) {
      return EFI_SUCCESS;
    } else if (EFI_ERROR (Status)) {
      return Status;
    }

    LengthTested = 0;
    while (LengthTested < Length) {
      SubRangeLength = MIN(SIZE_1MB, Length - LengthTested);
      Test->RangeTest (Start, SubRangeLength, Test->Context);
      Start += SubRangeLength;
      LengthTested += SubRangeLength;
    }
  }
}


EFI_STATUS
EFIAPI
MtSupportInstallMemoryRangeTest (
  CHAR16                *Name,
  TEST_MEM_RANGE        TestRangeFunction,
  IN  VOID              *Context
  )
{
  EFI_STATUS           Status;
  MEM_RANGE_TEST_DATA  *NewInstance;

  NewInstance = (MEM_RANGE_TEST_DATA*) AllocatePool (sizeof (*NewInstance));
  if (NewInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  NewInstance->RangeTest = TestRangeFunction;
  NewInstance->Context = Context;

  Status = MtSupportInstallMemoryTest (
             Name,
             RunMemoryRangeTest,
             (VOID*) NewInstance
             );
  if (EFI_ERROR (Status)) {
    FreePool (NewInstance);
    return Status;
  }

  return Status;
}


EFI_STATUS
EFIAPI
MtSupportInstallMemoryTest (
  IN CHAR16                *Name,
  IN RUN_MEM_TEST          MemTestFunction,
  IN VOID                  *Context
  )
{
  MEM_TEST_INSTANCE  *NewTests;
  UINTN              NewMaxTests;

  if (mTestCount >= mMaxTestCount) {
    NewMaxTests = mMaxTestCount + 32;
    NewTests =
      (MEM_TEST_INSTANCE*) AllocatePool (
                             sizeof (MEM_TEST_INSTANCE) *
                             mMaxTestCount
                             );
    if (NewTests == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    mMaxTestCount = NewMaxTests;
    mTests = NewTests;
  }

  mTests[mTestCount].Name = Name;
  mTests[mTestCount].RunMemTest = MemTestFunction;
  mTests[mTestCount].Context = Context;
  mTestCount++;

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
MtSupportRunAllTests (
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;
  UINTN       Loop;

  ReturnStatus = EFI_SUCCESS;
  for (Loop = 0; Loop < mTestCount; Loop++) {
    MtUiSetTestName (mTests[Loop].Name);
    Status = mTests[Loop].RunMemTest (mTests[Loop].Context);
    if (EFI_ERROR (Status)) {
      ReturnStatus = Status;
    }
  }

  return ReturnStatus;
}


VOID
EFIAPI
MtWbinvd (
  VOID
  )
{
  AsmWbinvd ();
}


/**
  Memory Test Constructor.

  @return  EFI_STATUS
**/
EFI_STATUS
EFIAPI
MemTestSupportLibConstructor (
  )
{
  return EFI_SUCCESS;
}


