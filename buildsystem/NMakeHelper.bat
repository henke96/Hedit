@echo off
break > bin\tmp\NMakeDeps.mk
where cl >nul 2>&1
if %ERRORLEVEL%==0 (
    for %%X in (%~1) do (
        if not exist "bin\objects\%%X_msvc.dep" (
            buildsystem\MsvcGenDeps.bat "%%X" %2 %3 > "bin\objects\%%X_msvc.dep"
            @echo off
        )
        echo !INCLUDE bin\objects\%%X_msvc.dep >> bin\tmp\NMakeDeps.mk
    )
)
exit
where clang >nul 2>&1
if %ERRORLEVEL%==0 (
    for %%X in (%~1) do (
        if not exist "bin\objects\%%X_clang.dep" (
            buildsystem\ClangGenDeps.bat "%%X" %4 %5 > "bin\objects\%%X_clang.dep"
            @echo off
        )
        echo !INCLUDE bin\objects\%%X_clang.dep >> bin\tmp\NMakeDeps.mk
    )
)