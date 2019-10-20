@echo off
call buildsystem\createDirs.bat
break > build\NMakeHelp.mk
>> "build\NMakeHelp.mk" (
    setlocal EnableDelayedExpansion
    for %%i in (%~1) do (
        set msvc_common_objs=build\%%i.msvc.obj !msvc_common_objs!
        set msvc_d_common_objs=build\%%i.msvc_d.obj !msvc_d_common_objs!
        set win_clang_common_objs=build\%%i.win_clang.o !win_clang_common_objs!
        set win_clang_d_common_objs=build\%%i.win_clang_d.o !win_clang_d_common_objs!
    )
    for %%i in (%~2) do (
        set msvc_main_objs=build\%%i.msvc.obj !msvc_main_objs!
        set msvc_d_main_objs=build\%%i.msvc_d.obj !msvc_d_main_objs!
        set win_clang_main_objs=build\%%i.win_clang.o !win_clang_main_objs!
        set win_clang_d_main_objs=build\%%i.win_clang_d.o !win_clang_d_main_objs!
    )
    for %%i in (%~3) do (
        set msvc_test_objs=build\%%i.msvc.obj !msvc_test_objs!
        set msvc_d_test_objs=build\%%i.msvc_d.obj !msvc_d_test_objs!
        set win_clang_test_objs=build\%%i.win_clang.o !win_clang_test_objs!
        set win_clang_d_test_objs=build\%%i.win_clang_d.o !win_clang_d_test_objs!
    )
    echo msvc_common_objs = !msvc_common_objs!
    echo msvc_d_common_objs = !msvc_d_common_objs!
    echo msvc_main_objs = !msvc_main_objs!
    echo msvc_d_main_objs = !msvc_d_main_objs!
    echo msvc_test_objs = !msvc_test_objs!
    echo msvc_d_test_objs = !msvc_d_test_objs!
    echo win_clang_common_objs = !win_clang_common_objs!
    echo win_clang_d_common_objs = !win_clang_d_common_objs!
    echo win_clang_main_objs = !win_clang_main_objs!
    echo win_clang_d_main_objs = !win_clang_d_main_objs!
    echo win_clang_test_objs = !win_clang_test_objs!
    echo win_clang_d_test_objs = !win_clang_d_test_objs!
    endlocal

    for %%X in (%~1 %~2 %~3) do (
        echo build\%%X.msvc.dep: src\%%X Configuration.mk
        echo.    buildsystem\MsvcGenDeps.bat "%%X" msvc "$(MSVC_RELEASE_FLAGS)"^> build\%%X.msvc.dep

        echo build\%%X.msvc_d.dep: src\%%X Configuration.mk
        echo.    buildsystem\MsvcGenDeps.bat "%%X" msvc_d "$(MSVC_DEBUG_FLAGS)"^> build\%%X.msvc_d.dep

        echo build\%%X.msvc_d.obj: build\%%X.msvc_d.dep
        echo.    cl /Fo$@ /c $^(MSVC_DEBUG_FLAGS^) src\%%X

        echo build\%%X.msvc.obj: build\%%X.msvc.dep
        echo.    cl /Fo$@ /c $^(MSVC_RELEASE_FLAGS^) src\%%X

        echo !IF EXIST^(build\%%X.msvc.dep^)
        echo !INCLUDE build\%%X.msvc.dep
        echo !ENDIF

        echo !IF EXIST^(build\%%X.msvc_d.dep^)
        echo !INCLUDE build\%%X.msvc_d.dep
        echo !ENDIF

        echo build\%%X.win_clang.dep: src\%%X Configuration.mk
        echo.    $^(clang_command^) -MM -MP -MT build\%%X.win_clang.o -c $^(WIN_CLANG_RELEASE_FLAGS^) src\%%X^> build\%%X.win_clang.dep

        echo build\%%X.win_clang_d.dep: src\%%X Configuration.mk
        echo.    $^(clang_command^) -MM -MP -MT build\%%X.win_clang_d.o -c $^(WIN_CLANG_DEBUG_FLAGS^) src\%%X^> build\%%X.win_clang_d.dep

        echo build\%%X.win_clang_d.o: build\%%X.win_clang_d.dep
        echo.    $^(clang_command^) -o $@ -c $^(WIN_CLANG_DEBUG_FLAGS^) src\%%X

        echo build\%%X.win_clang.o: build\%%X.win_clang.dep
        echo.    $^(clang_command^) -o $@ -c $^(WIN_CLANG_RELEASE_FLAGS^) src\%%X

        echo !IF EXIST^(build\%%X.win_clang.dep^)
        echo !INCLUDE build\%%X.win_clang.dep
        echo !ENDIF

        echo !IF EXIST^(build\%%X.win_clang_d.dep^)
        echo !INCLUDE build\%%X.win_clang_d.dep
        echo !ENDIF
    )
)