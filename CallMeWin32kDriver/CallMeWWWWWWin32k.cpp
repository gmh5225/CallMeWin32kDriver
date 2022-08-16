#include "Common.h"
#include <ntimage.h>

EXTERN_C
__int64 __fastcall fnMmLoadSystemImage(
    struct _UNICODE_STRING *DrvName,
    const UNICODE_STRING *a2,
    UNICODE_STRING *a3,
    unsigned int a4_win32k_status,
    ULONG64 *SectionPointer,
    ULONG64 *BaseAddress);

EXTERN_C
PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader(PVOID Base);

__declspec(
    noinline) long CallMeWin32kDriver(wchar_t *DriverPath, unsigned long DriverPathLength, bool NeedPrefix /*= true*/)
{
    long lStatus = -1;

    unsigned long uDriverPathAllLength = DriverPathLength;

    do
    {
        if (!DriverPath || !DriverPathLength)
        {
            break;
        }

        wchar_t wszDriverPath[MAX_PATH + 1];
        RtlSecureZeroMemory(wszDriverPath, sizeof(wszDriverPath));

        if (NeedPrefix)
        {
            strcat_w(wszDriverPath, (L"\\??\\"));
        }

        if (DriverPathLength < MAX_PATH * sizeof(wchar_t))
        {
            strcat_w(wszDriverPath, DriverPath);
            if (NeedPrefix)
            {
                uDriverPathAllLength += 4 * sizeof(wchar_t);
            }
        }

        UNICODE_STRING usDriverName;
        usDriverName.Buffer = wszDriverPath;
        usDriverName.Length = (USHORT)uDriverPathAllLength;
        usDriverName.MaximumLength = MAX_PATH * sizeof(wchar_t);

        UNICODE_STRING usMmLoadSystemImage;
        RtlInitUnicodeString(&usMmLoadSystemImage, L"MmLoadSystemImage");

        auto pMmLoadSystemImage = (decltype(&fnMmLoadSystemImage))(MmGetSystemRoutineAddress(&usMmLoadSystemImage));
        if (pMmLoadSystemImage)
        {
            ULONG64 uSectionPointer = 0, uBaseAddress = 0;
            lStatus = pMmLoadSystemImage(&usDriverName, 0, 0, 1, &uSectionPointer, &uBaseAddress);
            if (NT_SUCCESS(lStatus) && uBaseAddress)
            {
                auto pImageNtHeader = RtlImageNtHeader((void *)uBaseAddress);
                if (pImageNtHeader)
                {
                    // Fake Driver Object
                    auto pNewDrvObj = (PDRIVER_OBJECT)ExAllocatePool(NonPagedPool, 0x1000);
                    if (!pNewDrvObj)
                    {
                        __asm {int 3 }
                    }

                    RtlSecureZeroMemory(pNewDrvObj, 0x1000);
                    pNewDrvObj->DriverStart = (PVOID)uBaseAddress;

                    // Call OEP
                    auto pOEP = (PDRIVER_INITIALIZE)(uBaseAddress + pImageNtHeader->OptionalHeader.AddressOfEntryPoint);
                    lStatus = pOEP(pNewDrvObj, nullptr);
                }
            }
        }
        else
        {
            lStatus = STATUS_NOT_SUPPORTED;
        }

    } while (0);

    return lStatus;
}
