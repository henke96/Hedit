# NMake makefile
!include Configuration.mk
sources = $(common_sources) $(windows_sources)
temp = $(sources:src/=bin/objects/)
msvc_objs = $(temp:.c=.msvc.o)
msvc_d_objs = $(temp:.c=.msvc_d.o)
clang_objs = $(temp:.c=.win_clang.o)
clang_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(clang_flags)
clang_d_objs = $(temp:.c=.win_clang_d.o)
clang_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(clang_flags)
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
	clang -o $@ $** $(clang_d_allflags)

bin\release-clang.exe: $(clang_objs)
	clang -o $@ $** $(clang_allflags)

!IF [buildsystem\NMakeHelper.bat "$(files)" \
"$(msvc_flags) $(msvc_debug_flags)" \
"$(msvc_flags) $(msvc_release_flags)" \
"$(clang_d_allflags)" \
"$(clang_allflags)"]
!ENDIF

!INCLUDE buildsystem\tmp\NMakeDeps.mk