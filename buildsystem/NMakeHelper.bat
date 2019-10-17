@echo off
mkdir bin 2> NUL
break > buildsystem\NMakeTemp.dep
where cl >nul 2>&1
if %ERRORLEVEL%==0 (
    for %%X in (%~1) do (
        if not exist "build\%%X.msvc.dep" (
            call buildsystem\MsvcGenDeps.bat "%%X" %2 %4 > "build\%%X.msvc.dep"
        )
        echo !INCLUDE build\%%X.msvc.dep >> buildsystem\NMakeTemp.dep
    )
)
where clang >nul 2>&1
if %ERRORLEVEL%==0 (
    for %%X in (%~1) do (
        if not exist "build\%%X.win_clang.dep" (
            call buildsystem\ClangGenDeps.bat "%%X" %3 %4 %5 > "build\%%X.win_clang.dep"
        )
        echo !INCLUDE build\%%X.win_clang.dep >> buildsystem\NMakeTemp.dep
    )
)