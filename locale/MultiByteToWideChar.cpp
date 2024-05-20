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
#include <corecrt_internal.h>

// https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
static DWORD filter_mbtowcs_flags(UINT const code_page, DWORD const initial_flags)
{
    switch (code_page)
    {
    // For the code pages listed below, dwFlags must be set to 0.
    // Otherwise, the function fails with ERROR_INVALID_FLAGS.
    case 57002:
    case 57003:
    case 57004:
    case 57005:
    case 57006:
    case 57007:
    case 57008:
    case 57009:
    case 57010:
    case 57011:
    
    case 50220:
    case 50221:
    case 50222:
    case 50225:
    case 50227:
    case 50229:

    case CP_SYMBOL:
    case CP_UTF7:
        return 0;

    // For UTF-8 or code page 54936 , dwFlags must be set to either 0 or MB_ERR_INVALID_CHARS.
    // Otherwise, the function fails with ERROR_INVALID_FLAGS.
    case CP_UTF8:
    case 54936:
        return initial_flags & MB_ERR_INVALID_CHARS;

    default:
        return initial_flags;
    }
}

extern "C" int __cdecl __acrt_MultiByteToWideChar(
    UINT   CodePage,
    DWORD  dwFlags,
    LPCSTR lpMultiByteStr,
    int    cbMultiByte,
    LPWSTR lpWideCharStr,
    int    cchWideChar
    )
{
    DWORD filtered_flags = filter_mbtowcs_flags(CodePage, dwFlags);
    return MultiByteToWideChar(
        CodePage,
        filtered_flags,
        lpMultiByteStr,
        cbMultiByte,
        lpWideCharStr,
        cchWideChar
    );
}
