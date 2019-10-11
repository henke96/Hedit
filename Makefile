# NMake makefile
!include Configuration.mk
temp = $(sources:src/=bin/objects/)
temp2 = $(sources:src/=)
build_objs = $(temp:.c=.o)
build_deps = $(temp:.c=.dep)
files = $(temp2:.c=)

debug: bin\debug-msvc.exe

clean:
    del bin\* /Q
    del bin\objects\*.o /S /Q > nul
    del bin\objects\*.dep /S /Q > nul
    del bin\objects\*.pdb /S /Q > nul

bin\debug-msvc.exe: $(build_objs)
    link /OUT:$@ /PDB:bin\debug-msvc.pdb /DEBUG $(build_objs)

!IF [buildsystem\NMakeHelper.bat "$(files)" "$(msvc_flags) $(msvc_debug_flags)"]
!ENDIF

!INCLUDE bin\tmp\NMakeDeps.mk