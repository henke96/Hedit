@echo off
echo build\%~1.nm_win_clang.dep: src\%~1.c
echo.    %0 %1 %2 %3 ^> build\%~1.nm_win_clang.dep

clang -MM -MP -MT "build\%~1.win_clang_d.o build\%~1.win_clang.o" -c %~3 src\%~1.c
echo build\%~1.win_clang_d.o: build\%~1.nm_win_clang.dep
for /F %%^" in ("""") do echo(%%~"    clang -o $@ -c %~2 src\%~1.c

echo build\%~1.win_clang.o: build\%~1.nm_win_clang.dep
for /F %%^" in ("""") do echo(%%~"    clang -o $@ -c %~3 src\%~1.c