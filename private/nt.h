//
// MIT License
// 
// Copyright (c) 2024 Huang Qinjin (huangqinjin@gmail.com)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winternl.h>

#define _NTDEF_
#define PNTSTATUS NTSTATUS*

#include <winnt.h>

#pragma warning(push)
#pragma warning(disable: 4005)
#include <ntstatus.h>
#pragma warning(pop)

// https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/gflags-flag-table
#define FLG_APPLICATION_VERIFIER 0x0100

#define RTL_USER_PROC_SECURE_PROCESS 0x80000000UL


#define ARCHIDX (sizeof(void*) == 8)

#define DECLARE(S) struct Self : S { \
    BYTE& operator[](ULONG_PTR offset) { return reinterpret_cast<BYTE*>(this)[offset]; } \
    BYTE& _(ULONG_PTR const (&offset)[2]) { return operator[](offset[ARCHIDX]); } \
}

#define DEFINE2(T, m, n) struct { \
    operator T&() { return reinterpret_cast<T&>(reinterpret_cast<Self*>(this)->n); } \
    T& operator->() { return operator T&(); } \
    T* operator&() { return &operator T&(); } \
} m

#define DEFINE1(T, m) DEFINE2(T, m, m)

// https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/api/pebteb/rtl_user_process_parameters.htm
union _RTL_USER_PROCESS_PARAMETERS_EX
{
    DECLARE(_RTL_USER_PROCESS_PARAMETERS);
    DEFINE2(ULONG, Flags, _({ 0x08, 0x08 }));
};

// https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/api/pebteb/peb/index.htm
union _PEBEX
{
    DECLARE(_PEB);
    DEFINE1(_RTL_USER_PROCESS_PARAMETERS_EX*, ProcessParameters);
    DEFINE2(ULONG, NtGlobalFlag, _({ 0x68, 0xbc }));
};

union _TEBEX
{
    DECLARE(_TEB);
    DEFINE1(_PEBEX*, ProcessEnvironmentBlock);
};

#undef DEFINE1
#undef DEFINE2
#undef DECLARE
#undef ARCHIDX

static inline _TEBEX* NtCurrentTebEx()
{
    return (_TEBEX*)NtCurrentTeb();
}

#undef NtCurrentTeb
#define NtCurrentTeb() NtCurrentTebEx()
