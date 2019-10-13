@echo off
echo build\%~1.nm_win_clang.dep: src\%~1.%4 Configuration.mk
echo.    %0 %1 %2 %3 %4 %5 ^> build\%~1.nm_win_clang.dep

%5 -MM -MP -MT "build\%~1.win_clang_d.o build\%~1.win_clang.o" -c %~3 src\%~1.%4
echo build\%~1.win_clang_d.o: build\%~1.nm_win_clang.dep
for /F %%^" in ("""") do echo(%%~"    %5 -o $@ -c %~2 src\%~1.%4

echo build\%~1.win_clang.o: build\%~1.nm_win_clang.dep
for /F %%^" in ("""") do echo(%%~"    %5 -o $@ -c %~3 src\%~1.%4