@echo off
del /Q bin\*
del /Q build\*
del /S /Q build\*.o > nul 2> nul
del /S /Q build\*.obj > nul 2> nul
del /S /Q build\*.dep > nul 2> nul
del /S /Q  build\*.pdb> nul 2> nul