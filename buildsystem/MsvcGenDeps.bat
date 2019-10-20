@echo off
echo build\%~1.%2.obj: build\%~1.%2.dep \
for /F "tokens=1,2,3,*" %%A in ('"%4 /nologo /c src\%~1 /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
)
echo. 
for /F "tokens=1,2,3,*" %%A in ('"%4 /nologo /c src\%~1 /Zs /showIncludes %~3"') do (
    if "%%A"=="Note:" if "%%B"=="including" if "%%C"=="file:" echo "%%D":
)


