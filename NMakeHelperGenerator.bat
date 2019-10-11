@echo off
for %%X in (%~1) do (
    echo build\%%X.o: src\%%X.c \
    for /F "tokens=1,2,3,*" %%A in ('"cl /nologo /c src\%%X.c /Zs /showIncludes %~2"') do if "%%A"=="Note:" (
        if "%%B"=="including" if "%%C"=="file:" echo "%%D" \
    )
    echo. 
    for /F %%^" in ("""") do echo(%%~"    cl /c /Fo$@ %~2 src\%%X.c
)