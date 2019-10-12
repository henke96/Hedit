@echo off
del bin\* /Q
del bin\objects\*.o /S /Q > nul 2> nul
del bin\objects\*.dep /S /Q > nul 2> nul
del bin\objects\*.pdb /S /Q > nul 2> nul