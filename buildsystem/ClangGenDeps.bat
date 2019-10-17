@echo off
echo build\%~1.win_clang.dep: src\%~1.%3 Configuration.mk
echo.    %0 %1 "$(WIN_CLANG_RELEASE_FLAGS)" %3 %4 ^> build\%~1.win_clang.dep

%4 -MM -MP -MT "build\%~1.win_clang_d.o build\%~1.win_clang.o" -c %~2 src\%~1.%3
echo build\%~1.win_clang_d.o: build\%~1.win_clang.dep
for /F %%^" in ("""") do echo(%%~"    %4 -o $@ -c $(WIN_CLANG_DEBUG_FLAGS) src\%~1.%3

echo build\%~1.win_clang.o: build\%~1.win_clang.dep
for /F %%^" in ("""") do echo(%%~"    %4 -o $@ -c $(WIN_CLANG_RELEASE_FLAGS) src\%~1.%3