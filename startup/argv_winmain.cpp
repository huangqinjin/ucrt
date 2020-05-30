/***
*wincmdln.c - process command line for WinMain
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Prepare command line to be passed to [w]WinMain.
*
*******************************************************************************/

#include <corecrt_internal.h>
#include <mbstring.h>



// In the function below, we need to ensure that we've initialized the mbc table
// before we start performing character transformations.
static void do_locale_initialization(char)    throw() { __acrt_initialize_multibyte(); }
static void do_locale_initialization(wchar_t) throw() { /* no-op */                    }

static char*    get_command_line(char)    throw() { return _acmdln; }
static wchar_t* get_command_line(wchar_t) throw() { return _wcmdln; }

static bool __cdecl should_copy_another_character(char const c) throw()
{
    return _ismbblead(c) != 0;
}

static bool __cdecl should_copy_another_character(wchar_t) throw()
{
    return false;
}



/***
*_[w]wincmdln
*
*Purpose:
*       Extract the command line tail to be passed to WinMain.
*
*       Be warned! This code was originally implemented by the NT group and
*       has remained pretty much unchanged since 12-91. It should be changed
*       only with extreme care since there are undoubtedly many apps which
*       depend on its historical behavior.
*
*Entry:
*       The global variable _[a|w]cmdln is set to point at the complete
*       command line.
*
*Exit:
*       Returns a pointer to the command line tail.
*
*Exceptions:
*
*******************************************************************************/
template <typename Character>
static Character* __cdecl common_wincmdln() throw()
{
    do_locale_initialization(Character());

    static Character empty_string[] = { '\0' };

    Character* command_line = get_command_line(Character()) == nullptr
        ? empty_string
        : get_command_line(Character());

    // Skip past the program name (the first token in the command line) and
    // check for and handle a quoted program name:
    bool in_double_quotes = false;
    while (*command_line > ' ' || (*command_line != '\0' && in_double_quotes))
    {
        // Toggle the in_double_quotes flag if the current character is '"'
        if (*command_line == '"')
            in_double_quotes = !in_double_quotes;

        if (should_copy_another_character(*command_line))
            ++command_line;

        ++command_line;
    }

    // Skip past any whitespace preceding the next token:
    while (*command_line != '\0' && *command_line <= ' ')
        ++command_line;

    return command_line;
}



extern "C" char* __cdecl _get_narrow_winmain_command_line()
{
    return common_wincmdln<char>();
}

extern "C" wchar_t* __cdecl _get_wide_winmain_command_line()
{
    return common_wincmdln<wchar_t>();
}
