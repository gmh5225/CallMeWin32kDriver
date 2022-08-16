#pragma once

#include <fltKernel.h>

#define MAX_PATH 260

#define dprintf(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, __VA_ARGS__)

FORCEINLINE
wchar_t *
strcat_w(wchar_t *dest, const wchar_t *src)
{
    if ((dest == 0) || (src == 0))
        return dest;

    while (*dest != 0)
        dest++;

    while (*src != 0)
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = 0;
    return dest;
}
