@echo off
break > buildsystem\tmp\NMakeDeps.mk
where cl >nul 2>&1
if %ERRORLEVEL%==0 (
    for %%X in (%~1) do (
        if not exist "bin\objects\%%X.msvc.dep" (
            call buildsystem\MsvcGenDeps.bat "%%X" %2 %3 > "bin\objects\%%X.msvc.dep"
            @echo off
        )
        echo !INCLUDE bin\objects\%%X.msvc.dep >> buildsystem\tmp\NMakeDeps.mk
    )
)
where clang >nul 2>&1
if %ERRORLEVEL%==0 (
    for %%X in (%~1) do (
        if not exist "bin\objects\%%X.nm_clang.dep" (
            call buildsystem\ClangGenDeps.bat "%%X" %4 %5 > "bin\objects\%%X.nm_clang.dep"
            @echo off
        )
        echo !INCLUDE bin\objects\%%X.nm_clang.dep >> buildsystem\tmp\NMakeDeps.mk
    )
)