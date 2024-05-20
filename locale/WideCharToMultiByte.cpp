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

// https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
static DWORD filter_wcstomb_flags(UINT const code_page, DWORD const initial_flags)
{
    switch (code_page)
    {
    // For the code pages listed below, dwFlags must be 0.
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

    case CP_UTF8:
    case 54936:
        return 0;

    // Note that this flag only applies when CodePage is specified as CP_UTF8 or 54936.
    // It cannot be used with other code page values.
    default:
        return initial_flags & ~WC_ERR_INVALID_CHARS;
    }
}

extern "C" int __cdecl __acrt_WideCharToMultiByte(
    UINT    CodePage,
    DWORD   dwFlags,
    LPCWSTR lpWideCharStr,
    int     cchWideChar,
    LPSTR   lpMultiByteStr,
    int     cbMultiByte,
    LPCSTR  lpDefaultChar,
    LPBOOL  lpUsedDefaultChar
    )
{
    bool is_unicode_codepage = CodePage == CP_UTF7 || CodePage == CP_UTF8;
    DWORD filtered_flags = filter_wcstomb_flags(CodePage, dwFlags);
    LPBOOL filtered_used_default_char = is_unicode_codepage ? nullptr : lpUsedDefaultChar;
    LPCSTR filtered_default_char = is_unicode_codepage ? nullptr : lpDefaultChar;
    if (is_unicode_codepage && lpUsedDefaultChar) lpUsedDefaultChar = FALSE;

    return WideCharToMultiByte(
        CodePage,
        filtered_flags,
        lpWideCharStr,
        cchWideChar,
        lpMultiByteStr,
        cbMultiByte,
        filtered_default_char,
        filtered_used_default_char
    );
}
