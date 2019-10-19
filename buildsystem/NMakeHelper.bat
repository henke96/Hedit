@echo off
call buildsystem\createDirs.bat
break > build\NMakeTemp.mk
for %%X in (%~1) do (
    echo build\%%X.msvc.dep: src\%%X.%4 Configuration.mk>> build\NMakeTemp.mk
    echo.    buildsystem\MsvcGenDeps.bat "%%X" "$(MSVC_RELEASE_FLAGS)" %4^> build\%%X.msvc.dep>> build\NMakeTemp.mk

    echo build\%%X.msvc_d.obj: build\%%X.msvc.dep \>> build\NMakeTemp.mk
    echo. >> build\NMakeTemp.mk
    echo.    cl /Fo$@ /c $^(MSVC_DEBUG_FLAGS^) src\%%X.%4>> build\NMakeTemp.mk

    echo build\%%X.msvc.obj: build\%%X.msvc.dep \>> build\NMakeTemp.mk
    echo. >> build\NMakeTemp.mk
    echo.    cl /Fo$@ /c $^(MSVC_RELEASE_FLAGS^) src\%%X.%4>> build\NMakeTemp.mk

    echo !IF EXIST^(build\%%X.msvc.dep^)>> build\NMakeTemp.mk
    echo !INCLUDE build\%%X.msvc.dep>> build\NMakeTemp.mk
    echo !ENDIF>> build\NMakeTemp.mk
)

for %%X in (%~1) do (
    echo build\%%X.win_clang.dep: src\%%X.%4 Configuration.mk>> build\NMakeTemp.mk
    echo.    %5 -MM -MP -MT "build\%%X.win_clang_d.o build\%%X.win_clang.o" -c $^(WIN_CLANG_RELEASE_FLAGS^) src\%%X.%4^> build\%%X.win_clang.dep >> build\NMakeTemp.mk

    echo build\%%X.win_clang_d.o: build\%%X.win_clang.dep>> build\NMakeTemp.mk
    echo.    %5 -o $@ -c $^(WIN_CLANG_DEBUG_FLAGS^) src\%%X.%4>> build\NMakeTemp.mk

    echo build\%%X.win_clang.o: build\%%X.win_clang.dep>> build\NMakeTemp.mk
    echo.    %5 -o $@ -c $^(WIN_CLANG_RELEASE_FLAGS^) src\%%X.%4>> build\NMakeTemp.mk

    echo !IF EXIST^(build\%%X.win_clang.dep^)>> build\NMakeTemp.mk
    echo !INCLUDE build\%%X.win_clang.dep>> build\NMakeTemp.mk
    echo !ENDIF>> build\NMakeTemp.mk
)