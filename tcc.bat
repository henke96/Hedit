@echo off
setlocal

if "%3"=="" (
    call :usage
    exit /b 1
)
set "flags=-Iinclude -DHEDIT_UNREACHABLE -Wall -Wwrite-strings %~5"

if "%2"=="linux" (
    set "flags=%flags% -DHEDIT_PLATFORM_LINUX"
    set output_extension=
) else (
    if "%2"=="windows" (
        set "flags=%flags% -DHEDIT_PLATFORM_WINDOWS"
        set output_extension=.exe
    ) else (
        if "%2"=="other" (
            set "flags=%flags% -DHEDIT_PLATFORM_OTHER"
            set output_extension=.bin
        ) else (
            call :usage
            exit /b 1
        )
    )
)

if "%3"=="debug" (
    set "flags=%flags% -g"
) else (
    if "%3"=="release" (
        set "flags=%flags% -DNDEBUG"
    ) else (
        call :usage
        exit /b 1
    )
)

if "%~4"=="" (
    set "compiler=tcc.exe"
) else (
    set "compiler=%~4"
)

for /f "delims=" %%i in ("%compiler%") do set "compilername=%%~ni"

@echo on
"%compiler%" %flags% -o "%1_%3_%compilername%%output_extension%" "%~1.c"
@echo off
exit /b 0

:usage
echo Usage:
echo tcc.bat ^<target^> ^<platform^> ^<mode^> [compiler] [cflags]
echo.
echo ^<target^>   = Name of source file minus `.c` extension.
echo ^<platform^> = Target platform, `linux`, `windows` or `other`.
echo ^<mode^>     = Build mode, `debug` or `release`.
echo [compiler] = Compiler to use. Default is tcc.
echo [cflags]   = Extra flags to pass to the compiler.
exit /b
