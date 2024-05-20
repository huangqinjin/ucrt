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

extern "C" static __crt_state_management::dual_state_global<_UserMathErrorFunctionPointer> user_matherr;

extern "C" void __cdecl __acrt_initialize_user_matherr(void* encoded_null)
{
    user_matherr.initialize((_UserMathErrorFunctionPointer)encoded_null);
}

extern "C" void __cdecl __setusermatherr(_UserMathErrorFunctionPointer pf)
{
    user_matherr.value() = __crt_fast_encode_pointer(pf);
}

extern "C" bool __cdecl __acrt_has_user_matherr()
{
    return __crt_fast_decode_pointer(user_matherr.value()) != nullptr;
}

extern "C" int  __cdecl __acrt_invoke_user_matherr(_exception* math_exception)
{
    _UserMathErrorFunctionPointer local_matherr = __crt_fast_decode_pointer(user_matherr.value());
    if (local_matherr) return local_matherr(math_exception);
    return 0;
}
