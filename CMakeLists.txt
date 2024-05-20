#
# MIT License
# 
# Copyright (c) 2024 Huang Qinjin (huangqinjin@gmail.com)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
cmake_minimum_required(VERSION 3.28)

project(ucrt
    VERSION 10.0.22621.0
    DESCRIPTION "Windows Universal C Runtime"
    HOMEPAGE_URL https://github.com/huangqinjin/ucrt
    LANGUAGES C CXX ASM_MASM
)

if(NOT DEFINED VCToolsInstallDir)
    find_path(VCToolsInstallDir REQUIRED
        NAMES
            modules/modules.json
        HINTS
            ENV VCToolsInstallDir
        PATHS
            ${CMAKE_CXX_COMPILER}/../../../.. # VCToolsInstallDir layout
            ${CMAKE_CXX_COMPILER}/../../..    # msvc-wine layout
        NO_DEFAULT_PATH
        NO_CACHE
    )
    file(REAL_PATH ${VCToolsInstallDir}/modules/.. VCToolsInstallDir)  # CMake 3.28+ resolves symlinks.
    set(VCToolsInstallDir ${VCToolsInstallDir} CACHE PATH "MSVC Toolchain Location")
endif()

if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_CXX_STANDARD 17)

# https://gitlab.kitware.com/cmake/cmake/-/issues/23537
string(APPEND CMAKE_ASM_MASM_FLAGS " /nologo /quiet")

# https://gitlab.kitware.com/cmake/cmake/-/issues/20610
string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-umbrella-libraries
# UCRT depends on API sets instead of classical system libraries (e.g. kernel32.lib).
set(CMAKE_CXX_STANDARD_LIBRARIES "OneCore_apiset.lib")

if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES [[x64]])
    set(arch amd64)
elseif(CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES [[X86]])
    set(arch i386)
elseif(CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES [[ARM64]])
    set(arch arm64)
elseif(CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES [[ARM]])
    set(arch arm)
else()
    message(FATAL_ERROR "Unknown architecture: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
endif()

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    private
    include
    inc
    inc/${arch}
    ${VCToolsInstallDir}/crt/src/vcruntime
)

target_compile_options(${PROJECT_NAME} PRIVATE
    # Force to include config.h for every source file.
    $<$<COMPILE_LANGUAGE:C,CXX>:/FIconfig.h>
    # Disable C++ exception.
    $<$<COMPILE_LANGUAGE:CXX>:/EHs->
    # Disable C++ RTTI.
    $<$<COMPILE_LANGUAGE:CXX>:/GR->
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX d

    # https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features#c-runtime-lib-files
    # Use -MT[d] to link to static runtime libraries:
    # 1. libcmt[d]. Includes CRT startup functions and supports /GS and /RTC.
    # 2. libvcruntime[d]. Defines some CRT functions e.g. memset and strchr.
    # 3. libucrt[d]. Provides all other obj files (e.g. math related) not being built.
    MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>
)

if(BUILD_SHARED_LIBS)
    # Export functions from asm sources, libcmt, libvcruntime and libucrt.
    target_sources(${PROJECT_NAME} PRIVATE private/ucrtbase.def)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ucrtbase
        DEFINE_SYMBOL CRTDLL
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        LINKER:/ENTRY:__acrt_DllMain
        LINKER:/NODEFAULTLIB:uuid.lib
        LINKER:/NODEFAULTLIB:kernel32.lib
        # Need to set VERBOSE=1 before invoking link.exe through cmake.
        LINKER:/VERBOSE
    )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ucrt
        PREFIX lib
    )
endif()

list(APPEND modules
    conio
    convert
    dll
    env
    exec
    filesystem
    heap
    initializers
    internal
    locale
    lowio
    mbstring
    misc
    startup
    stdio
    stdlib
    string
    time
    tran
)

if(NOT BUILD_SHARED_LIBS)
    list(REMOVE_ITEM modules dll)
endif()

foreach(m IN LISTS modules)
    file(GLOB ${m}_SRCS RELATIVE ${PROJECT_SOURCE_DIR}
        ${m}/*.c
        ${m}/*.cpp
    )
    file(GLOB ${m}_ARCH_SRCS RELATIVE ${PROJECT_SOURCE_DIR}
        ${m}/${arch}/*.asm
        ${m}/${arch}/*.c
        ${m}/${arch}/*.cpp
    )
    list(APPEND ${m}_SRCS ${${m}_ARCH_SRCS})
    list(TRANSFORM ${m}_ARCH_SRCS REPLACE [[^.+/([^/]+)\.[^.]+$]] [[\1]])
    list(JOIN ${m}_ARCH_SRCS | ${m}_ARCH_SRCS)
    list(FILTER ${m}_SRCS EXCLUDE REGEX "^${m}/(${${m}_ARCH_SRCS})\\.(c|cpp)$")
    unset(${m}_ARCH_SRCS)

    target_sources(${PROJECT_NAME} PRIVATE ${${m}_SRCS})
endforeach()

set_property(SOURCE dll/oswrappers.cpp
    APPEND PROPERTY COMPILE_OPTIONS
    /FA /Fa${PROJECT_BINARY_DIR}/oswrappers.asm # generate an assembler listing file for mangled wrapper names
)

set_source_files_properties(
    ${mbstring_SRCS}
    convert/ismbstr.cpp
    PROPERTIES COMPILE_DEFINITIONS _MBCS
)

set_source_files_properties(
    filesystem/splitpath.cpp
    PROPERTIES COMPILE_OPTIONS /wd4838 # conversion from 'int' to 'size_t' requires a narrowing conversion
)
