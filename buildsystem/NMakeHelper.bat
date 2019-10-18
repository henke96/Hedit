@echo off
mkdir bin 2> NUL
break > buildsystem\NMakeTemp.dep
for %%X in (%~1) do (
    echo build\%%X.msvc.dep: src\%%X.%4 Configuration.mk>> buildsystem\NMakeTemp.dep
    echo.    buildsystem\MsvcGenDeps.bat "%%X" "$(MSVC_RELEASE_FLAGS)" %4^> build\%%X.msvc.dep>> buildsystem\NMakeTemp.dep

    echo build\%%X.msvc_d.obj: build\%%X.msvc.dep \>> buildsystem\NMakeTemp.dep
    echo. >> buildsystem\NMakeTemp.dep
    echo.    cl /Fo$@ /c $^(MSVC_DEBUG_FLAGS^) src\%%X.%4>> buildsystem\NMakeTemp.dep

    echo build\%%X.msvc.obj: build\%%X.msvc.dep \>> buildsystem\NMakeTemp.dep
    echo. >> buildsystem\NMakeTemp.dep
    echo.    cl /Fo$@ /c $^(MSVC_RELEASE_FLAGS^) src\%%X.%4>> buildsystem\NMakeTemp.dep

    echo !IF EXIST^(build\%%X.msvc.dep^)>> buildsystem\NMakeTemp.dep
    echo !INCLUDE build\%%X.msvc.dep>> buildsystem\NMakeTemp.dep
    echo !ENDIF>> buildsystem\NMakeTemp.dep
)

for %%X in (%~1) do (
    echo build\%%X.win_clang.dep: src\%%X.%4 Configuration.mk>> buildsystem\NMakeTemp.dep
    echo.    %5 -MM -MP -MT "build\%%X.win_clang_d.o build\%%X.win_clang.o" -c $^(WIN_CLANG_RELEASE_FLAGS^) src\%%X.%4^> build\%%X.win_clang.dep >> buildsystem\NMakeTemp.dep

    echo build\%%X.win_clang_d.o: build\%%X.win_clang.dep>> buildsystem\NMakeTemp.dep
    echo.    %5 -o $@ -c $^(WIN_CLANG_DEBUG_FLAGS^) src\%%X.%4>> buildsystem\NMakeTemp.dep

    echo build\%%X.win_clang.o: build\%%X.win_clang.dep>> buildsystem\NMakeTemp.dep
    echo.    %5 -o $@ -c $^(WIN_CLANG_RELEASE_FLAGS^) src\%%X.%4>> buildsystem\NMakeTemp.dep

    echo !IF EXIST^(build\%%X.win_clang.dep^)>> buildsystem\NMakeTemp.dep
    echo !INCLUDE build\%%X.win_clang.dep>> buildsystem\NMakeTemp.dep
    echo !ENDIF>> buildsystem\NMakeTemp.dep
)