@echo off
echo build\%~1.msvc_d.obj build\%~1.msvc.obj: build\%~1.msvc.dep \
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.%3 /Zs /showIncludes %~2"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%~1.%3 /Zs /showIncludes %~2"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D":
)


