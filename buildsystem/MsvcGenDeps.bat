@echo off
if %2==1 (
    echo build\%~1.msvc.obj build\%~1.msvc.asm: build\%~1.msvc.dep \
) else (
    echo build\%~1.msvc_d.obj: build\%~1.msvc_d.dep \
)
for /F "tokens=1,2,3,*" %%A in ('"%4 /nologo /c src\%~1 /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F "tokens=1,2,3,*" %%A in ('"%4 /nologo /c src\%~1 /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D":
)


