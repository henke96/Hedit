# NMake makefile
!include Configuration.mk
temp = $(sources:src/=bin/objects/)
temp2 = $(sources:src/=)
msvc_objs = $(temp:.c=_msvc.o)
msvc_d_objs = $(temp:.c=_msvc_d.o)
files = $(temp2:.c=)

debug: bin\debug-msvc.exe
release: bin\release-msvc.exe

clean:
    del bin\* /Q
    del bin\objects\*.o /S /Q > nul
    del bin\objects\*.dep /S /Q > nul
    del bin\objects\*.pdb /S /Q > nul

bin\debug-msvc.exe: $(msvc_d_objs)
    link /OUT:$@ /PDB:bin\debug-msvc.pdb /DEBUG $(msvc_d_objs)

bin\release-msvc.exe: $(msvc_objs)
    link /OUT:$@ $(msvc_objs)

!IF [buildsystem\NMakeHelper.bat "$(files)" "$(msvc_flags) $(msvc_debug_flags)" "$(msvc_flags) $(msvc_release_flags)"]
!ENDIF

!INCLUDE bin\tmp\NMakeDeps.mk