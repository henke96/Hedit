# NMake makefile
!include Configuration.mk

temp_common_sources = $(common_sources) $(common_windows_sources)
temp_main_sources = $(main_sources) $(main_windows_sources)
temp_test_sources = $(test_sources) $(test_windows_sources)

!IF $(cpp)
win_all_common_sources = $(temp_common_sources:.cpp=.c)
win_all_main_sources = $(temp_main_sources:.cpp=.c)
win_all_test_sources = $(temp_test_sources:.cpp=.c)
source_ext = cpp
clang_command = clang++
!ELSE
win_all_common_sources = $(temp_common_sources)
win_all_main_sources = $(temp_main_sources)
win_all_test_sources = $(temp_test_sources)
source_ext = c
clang_command = clang
!ENDIF
win_all_sources = $(win_all_common_sources) $(win_all_main_sources) $(win_all_test_sources)

temp_common = $(win_all_common_sources:src/=build/)
temp_main = $(win_all_main_sources:src/=build/)
temp_test = $(win_all_test_sources:src/=build/)

msvc_common_objs = $(temp_common:.c=.msvc.obj)
msvc_main_objs = $(temp_main:.c=.msvc.obj)
msvc_test_objs = $(temp_test:.c=.msvc.obj)
msvc_d_common_objs = $(temp_common:.c=.msvc_d.obj)
msvc_d_main_objs = $(temp_main:.c=.msvc_d.obj)
msvc_d_test_objs = $(temp_test:.c=.msvc_d.obj)
msvc_allflags = $(msvc_flags) $(msvc_release_flags)
msvc_d_allflags = $(msvc_flags) $(msvc_debug_flags)

win_clang_common_objs = $(temp_common:.c=.win_clang.o)
win_clang_main_objs = $(temp_main:.c=.win_clang.o)
win_clang_test_objs = $(temp_test:.c=.win_clang.o)
win_clang_d_common_objs = $(temp_common:.c=.win_clang_d.o)
win_clang_d_main_objs = $(temp_main:.c=.win_clang_d.o)
win_clang_d_test_objs = $(temp_test:.c=.win_clang_d.o)
win_clang_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(clang_flags)
win_clang_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(clang_flags)

debug: bin\debug-msvc.exe
release: bin\release-msvc.exe
debug-clang: bin\debug-clang.exe
release-clang: bin\release-clang.exe
test-debug: bin\test-debug-msvc.exe
test-release: bin\test-release-msvc.exe
test-debug-clang: bin\test-debug-clang.exe
test-release-clang: bin\test-release-clang.exe

all: debug test-debug release test-release debug-clang test-debug-clang release-clang test-release-clang

clean:
    buildsystem\clean.bat

# Main binaries
bin\debug-msvc.exe: $(msvc_d_common_objs) $(msvc_d_main_objs)
    cl /Fe$@ $(msvc_d_allflags) $**

bin\release-msvc.exe: $(msvc_common_objs) $(msvc_main_objs)
    cl /Fe$@ $(msvc_allflags) $**

bin\debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_main_objs)
	$(clang_command) -o $@ $(win_clang_d_allflags) $**

bin\release-clang.exe: $(win_clang_common_objs) $(win_clang_main_objs)
	$(clang_command) -o $@ $(win_clang_allflags) $**

# Test binaries
bin\test-debug-msvc.exe: $(msvc_d_common_objs) $(msvc_d_test_objs)
    cl /Fe$@ $(msvc_d_allflags) $**

bin\test-release-msvc.exe: $(msvc_common_objs) $(msvc_test_objs)
    cl /Fe$@ $(msvc_allflags) $**

bin\test-debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_test_objs)
	$(clang_command) -o $@ $(win_clang_d_allflags) $(win_clang_d_common_objs) $(win_clang_d_test_objs)

bin\test-release-clang.exe: $(win_clang_common_objs) $(win_clang_test_objs)
	$(clang_command) -o $@ $(win_clang_allflags) $(win_clang_common_objs) $(win_clang_test_objs)

# Dependencies
temp_all = $(win_all_sources:src/=)
all_files = $(temp_all:.c=)

!IF [buildsystem\NMakeHelper.bat "$(all_files)" \
"$(msvc_d_allflags)" \
"$(msvc_allflags)" \
"$(win_clang_d_allflags)" \
"$(win_clang_allflags)" \
$(source_ext) \
$(clang_command) \
]
!ENDIF

!INCLUDE buildsystem\NMakeTemp.dep

!IF [del buildsystem\NMakeTemp.dep]
!ENDIF
