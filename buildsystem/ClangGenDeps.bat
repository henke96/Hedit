@echo off
echo bin\objects\%~1_clang.dep: src\%~1.c
echo.    %0 %1 %2 %3 ^> bin\objects\%~1_clang.dep

clang -MM -MP -MT "%~1_clang_d.o %~1_clang.o" -c %~3 src\%~1.c
echo bin\objects\%~1_clang_d.o: bin\objects\%~1_clang.dep
for /F %%^" in ("""") do echo(%%~"    clang -c -o $@ %~2 src\%~1.c

echo bin\objects\%~1_clang.o: bin\objects\%~1_clang.dep
for /F %%^" in ("""") do echo(%%~"    clang -c -o $@ %~3 src\%~1.c