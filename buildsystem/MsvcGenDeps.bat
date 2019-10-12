@echo off
echo bin\objects\%~1_msvc.dep: src\%~1.c
echo.    %0 %1 %2 %3 ^> bin\objects\%~1_msvc.dep

echo bin\objects\%~1_msvc_d.o: bin\objects\%~1_msvc.dep \
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.c /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F %%^" in ("""") do echo(%%~"    cl /Fo$@ /c %~2 src\%~1.c

echo bin\objects\%~1_msvc.o: bin\objects\%~1_msvc.dep \
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.c /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F %%^" in ("""") do echo(%%~"    cl /Fo$@ /c %~3 src\%~1.c
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.c /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D":
)


