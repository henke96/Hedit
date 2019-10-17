@echo off
echo build\%~1.msvc.dep: src\%~1.%3 Configuration.mk
echo.    %0 %1 "$(MSVC_RELEASE_FLAGS)" %3 ^> build\%~1.msvc.dep

echo build\%~1.msvc_d.obj: build\%~1.msvc.dep \
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.%3 /Zs /showIncludes %~2"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F %%^" in ("""") do echo(%%~"    cl /Fo$@ /c $(MSVC_DEBUG_FLAGS) src\%~1.%3

echo build\%~1.msvc.obj: build\%~1.msvc.dep \
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.%3 /Zs /showIncludes %~2"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F %%^" in ("""") do echo(%%~"    cl /Fo$@ /c $(MSVC_RELEASE_FLAGS) src\%~1.%3
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.%3 /Zs /showIncludes %~2"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D":
)


