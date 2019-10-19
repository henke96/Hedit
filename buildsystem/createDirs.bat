@echo off
mkdir bin 2> NUL
xcopy /i /t /e "src" "build"