# NMake makefile
!include Configuration.mk
temp = $(sources:src/=bin/objects/)
msvc_objs = $(temp:.c=_msvc.o)
msvc_d_objs = $(temp:.c=_msvc_d.o)
clang_objs = $(temp:.c=_clang.o)
clang_d_objs = $(temp:.c=_clang_d.o)
temp2 = $(sources:src/=)
files = $(temp2:.c=)

debug: bin\debug-msvc.exe
release: bin\release-msvc.exe
debug-clang: bin\debug-clang.exe
release-clang: bin\release-clang.exe

all: debug release debug-clang release-clang

clean:
    buildsystem\clean.bat

bin\debug-msvc.exe: $(msvc_d_objs)
    link /OUT:$@ /PDB:bin\debug-msvc.pdb /DEBUG $** /NOLOGO

bin\release-msvc.exe: $(msvc_objs)
    link /OUT:$@ $** /NOLOGO

bin\debug-clang.exe: $(clang_d_objs)
    lld-link /OUT:$@ /PDB:bin\debug-clang.pdb /DEBUG $** /DEFAULTLIB:libcmt

bin\release-clang.exe: $(clang_objs)
    lld-link /OUT:$@ $** /DEFAULTLIB:libcmt

!IF [buildsystem\NMakeHelper.bat "$(files)" \
"$(msvc_flags) $(msvc_debug_flags)" \
"$(msvc_flags) $(msvc_release_flags)" \
"$(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags)" \
"$(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags)"]
!ENDIF

!INCLUDE bin\tmp\NMakeDeps.mk