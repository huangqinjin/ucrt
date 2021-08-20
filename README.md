# Universal C Runtime

## What the UCRT is
Visual C++ C Runtime (CRT) was shipped with and versioned by each releases of Visual Studio (2002, 2003, 2005, 2008,
2010, 2012, and 2013) before. Since Visual Studio 2015, the CRT has been split into two logical parts: The **VCRuntime**,
which contained the compiler support functionality required for things like process startup and exception handling, and
a "stable" part named **Universal CRT**, which can be considered as the Windows' equivalent of the GNU C Library (glibc)
in the Linux world, that contained all of the purely library parts of the CRT including C99 and POSIX functionality and
some extensions.

## How the UCRT exsits
A C++ program built with Visual Studio 2015 or later will depend on the Universal CRT. The UCRT DLLs are named
ucrtbase.dll (release) and ucrtbased.dll (debug); they do not include a version number and become components of Windows
Operating System.
- The UCRT is a part of Windows 10.
- There are Windows Update MSU packages that distribute the UCRT for Windows Vista, 7 and 8.x.
- On Windows XP, the VCRedist will deploy the UCRT itself.

## Where the UCRT sources
- **Include:** `C:\Program Files (x86)\Windows Kits\10\Include\%Version%\ucrt`.
- **Source:** `C:\Program Files (x86)\Windows Kits\10\Source\%Version%\ucrt`.
- **Redist DLLs:** `C:\Program Files (x86)\Windows Kits\10\Redist\%Version%\ucrt\DLLs\%ARCH%`.
- **Debug DLL:** `C:\Program Files (x86)\Windows Kits\10\bin\%Version%\%ARCH%\ucrt`.

## References
1. [Introducing the Universal CRT](https://devblogs.microsoft.com/cppblog/introducing-the-universal-crt/).
1. [Determining Which DLLs to Redistribute](https://docs.microsoft.com/en-us/cpp/windows/determining-which-dlls-to-redistribute).
1. [Update for Universal C Runtime in Windows](https://support.microsoft.com/en-us/help/2999226/update-for-universal-c-runtime-in-windows).
