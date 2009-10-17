/** @file
  Memory range functions for Mem Test application

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

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemTestRangesLib.h>


EFI_MEMORY_DESCRIPTOR       *mMemoryMap = NULL;
UINTN                       mMemoryMapCount;
UINTN                       mPagesAllocated;


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
  )
{
  if (*Key < 0 || *Key >= mMemoryMapCount) {
    return EFI_NOT_FOUND;
  }

  *Start = mMemoryMap[*Key].PhysicalStart;
  *Length = MultU64x32 (mMemoryMap[*Key].NumberOfPages, EFI_PAGE_SIZE);
  (*Key)++;

  return EFI_SUCCESS;
}


EFI_STATUS
ReadMemoryRanges (
  )
{
  EFI_STATUS                  Status;
  UINTN                       NumberOfEntries;
  UINTN                       Loop;
  UINTN                       Size;
  EFI_MEMORY_DESCRIPTOR       *MemoryMap;
  EFI_MEMORY_DESCRIPTOR       *MapEntry;
  UINTN                       MapKey;
  UINTN                       DescSize;
  UINT32                      DescVersion;
  UINT64                      Available;
  EFI_PHYSICAL_ADDRESS        PagesAddress;
  UINT64                      NumberOfPages;
  UINT64                      SizeInBytes;

  Size = 0;
  Status = gBS->GetMemoryMap (&Size, NULL, NULL, NULL, NULL);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  MemoryMap = (EFI_MEMORY_DESCRIPTOR*) AllocatePool (Size);
  if (MemoryMap == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->GetMemoryMap (&Size, MemoryMap, &MapKey, &DescSize, &DescVersion);
  if (EFI_ERROR (Status)) {
    FreePool (MemoryMap);
    return Status;
  }

  if (DescVersion != EFI_MEMORY_DESCRIPTOR_VERSION) {
    FreePool (MemoryMap);
    return EFI_UNSUPPORTED;
  }

  NumberOfEntries = Size / DescSize;

  for (
      MapEntry = MemoryMap, Loop = 0, Available = 0;
      Loop < NumberOfEntries;
      MapEntry = (EFI_MEMORY_DESCRIPTOR*) ((UINT8*) MapEntry + DescSize),
        Loop++
    ) {
    if (MapEntry->Type == EfiConventionalMemory) {
      Available += MapEntry->NumberOfPages;
    }
  }

  mMemoryMap = MemoryMap;
  mMemoryMapCount = 0;
  mPagesAllocated = 0;
  for (
      MapEntry = MemoryMap, Loop = 0;
      (Loop < NumberOfEntries) && (Available > (SIZE_1MB / EFI_PAGE_SIZE));
      MapEntry = (EFI_MEMORY_DESCRIPTOR*) ((UINT8*) MapEntry + DescSize)
    ) {
    if (MapEntry->Type == EfiConventionalMemory) {
      PagesAddress = MapEntry->PhysicalStart;
      NumberOfPages = MapEntry->NumberOfPages;
      if (PagesAddress == 0ll && NumberOfPages > 1) {
        PagesAddress = PagesAddress + EFI_PAGE_SIZE;
        NumberOfPages--;
      }
      NumberOfPages = MIN (
                        NumberOfPages,
                        Available - (SIZE_1MB / EFI_PAGE_SIZE)
                        );
      Status = gBS->AllocatePages (
                      AllocateAddress,
                      EfiBootServicesData,
                      NumberOfPages,
                      &PagesAddress
                      );
      if (EFI_ERROR (Status)) {
        MtRangesDeconstructor ();
        return Status;
      }

      if (MapEntry->PhysicalStart != PagesAddress) {
        MapEntry->PhysicalStart = PagesAddress;
      }
      if (MapEntry->NumberOfPages != NumberOfPages) {
        MapEntry->NumberOfPages = NumberOfPages;
      }

      if (MapEntry != &mMemoryMap[mMemoryMapCount]) {
        CopyMem (&mMemoryMap[mMemoryMapCount], MapEntry, sizeof (*MapEntry));
      }
      mMemoryMapCount++;

      mPagesAllocated += NumberOfPages;
      Available -= NumberOfPages;
    }
  }

  if (mMemoryMapCount > 0) {
    Print (L"Memory Test Ranges:\n");
  }
  for (Loop = 0; Loop < mMemoryMapCount; Loop++) {
    SizeInBytes = MultU64x32 (mMemoryMap[Loop].NumberOfPages, EFI_PAGE_SIZE);
    Print (
      L"0x%012lx - 0x%012lx (0x%012lx)\n",
      mMemoryMap[Loop].PhysicalStart,
      mMemoryMap[Loop].PhysicalStart + SizeInBytes - 1,
      SizeInBytes
      );
  }
  if (mPagesAllocated > 0) {
    SizeInBytes = MultU64x32 (mPagesAllocated, EFI_PAGE_SIZE);
    Print (L"Total: 0x%lx (", SizeInBytes);
    if (SizeInBytes >= SIZE_4GB) {
      Print (
        L"%ldGB",
        DivU64x64Remainder (SizeInBytes, SIZE_1GB, NULL)
        );
    } else if (SizeInBytes >= SIZE_2MB) {
      Print (L"%ldMB", DivU64x32 (SizeInBytes, SIZE_1MB));
    } else if (SizeInBytes >= SIZE_2KB) {
      Print (L"%ldKB", DivU64x32 (SizeInBytes, SIZE_1KB));
    }
    Print (L")\n");
  }

  return EFI_SUCCESS;
}


/**
  Initializes the memory test ranges

**/
EFI_STATUS
MtRangesConstructor (
  )
{
  return ReadMemoryRanges ();
}


/**
  Decontructs the memory test ranges data structures

**/
EFI_STATUS
MtRangesDeconstructor (
  )
{
  UINTN Loop;

  if (mMemoryMap != NULL) {
    for (Loop = 0; Loop < mMemoryMapCount; Loop++) {
      gBS->FreePages (
        mMemoryMap[Loop].PhysicalStart,
        (UINTN) mMemoryMap[Loop].NumberOfPages
        );
    }
    FreePool (mMemoryMap);
    mMemoryMap = NULL;
  }
  return EFI_SUCCESS;
}


