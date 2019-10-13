@echo off
echo build\%~1.msvc.dep: src\%~1.c
echo.    %0 %1 %2 %3 ^> build\%~1.msvc.dep

echo build\%~1.msvc_d.obj: build\%~1.msvc.dep \
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.c /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F %%^" in ("""") do echo(%%~"    cl /Fo$@ /c %~2 src\%~1.c

echo build\%~1.msvc.obj: build\%~1.msvc.dep \
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.c /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F %%^" in ("""") do echo(%%~"    cl /Fo$@ /c %~3 src\%~1.c
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.c /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D":
)


