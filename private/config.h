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

#define _CRTBLD
#define _CORECRT_BUILD
#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

#ifdef CRTDLL
#define _CRTIMP __declspec(dllexport)
#define _CRT_GLOBAL_STATE_ISOLATION
#else
#define _CRTIMP
#endif


#define __WARNING_DEREF_NULL_PTR 6011
#define __WARNING_RETVAL_IGNORED_FUNC_COULD_FAIL 6031
#define __WARNING_MISSING_ZERO_TERMINATION2 6054
#define __WARNING_IGNOREDBYCOMMA 6319
#define __WARNING_BUFFER_OVERFLOW 6386
#define __WARNING_INCORRECT_ANNOTATION 26007
#define __WARNING_INCORRECT_VALIDATION 26014
#define __WARNING_POTENTIAL_BUFFER_OVERFLOW_HIGH_PRIORITY 26015
#define __WARNING_POTENTIAL_BUFFER_OVERFLOW_NULLTERMINATED 26018
#define __WARNING_PRECONDITION_NULLTERMINATION_VIOLATION 26035
#define __WARNING_POSTCONDITION_NULLTERMINATION_VIOLATION 26036
#define __WARNING_HIGH_PRIORITY_OVERFLOW_POSTCONDITION 26045
#define __WARNING_RANGE_PRECONDITION_VIOLATION 26060
#define __WARNING_NOT_SATISFIED 28020
#define __WARNING_RETURNING_BAD_RESULT 28196
#define __WARNING_BANNED_API_USAGE 28719
#define __WARNING_SETUNHANDLEDEXCEPTIONFILTER_USE 28725
#define __WARNING_BANNED_API_USAGEL2 28726
#define __WARNING_REDUNDANT_POINTER_TEST 28922
#define __WARNING_UNUSED_POINTER_ASSIGNMENT 28930
#define __WARNING_UNUSED_ASSIGNMENT 28931
#define __WARNING_W2A_BEST_FIT 38021
