@echo off
break > bin\tmp\NMakeDeps.mk
for %%X in (%~1) do (
    if not exist "bin\objects\%%X.dep" (
        buildsystem\MsvcGenDeps.bat "%%X" %2 %3 > "bin\objects\%%X.dep"
        @echo off
    )
    echo !INCLUDE bin\objects\%%X.dep >> bin\tmp\NMakeDeps.mk
)