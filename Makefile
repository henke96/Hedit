# NMake makefile
!include Configuration.mk

temp_common_sources = $(COMMON_SOURCES) $(COMMON_WINDOWS_SOURCES)
temp_main_sources = $(MAIN_SOURCES) $(MAIN_WINDOWS_SOURCES)
temp_test_sources = $(TEST_SOURCES) $(TEST_WINDOWS_SOURCES)

!IF $(CPP)
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

win_clang_common_objs = $(temp_common:.c=.win_clang.o)
win_clang_main_objs = $(temp_main:.c=.win_clang.o)
win_clang_test_objs = $(temp_test:.c=.win_clang.o)
win_clang_d_common_objs = $(temp_common:.c=.win_clang_d.o)
win_clang_d_main_objs = $(temp_main:.c=.win_clang_d.o)
win_clang_d_test_objs = $(temp_test:.c=.win_clang_d.o)

debug: bin\debug-msvc.exe
release: bin\release-msvc.exe
debug-clang: bin\debug-clang.exe
release-clang: bin\release-clang.exe
test-debug: bin\test-debug-msvc.exe
test-release: bin\test-release-msvc.exe
test-debug-clang: bin\test-debug-clang.exe
test-release-clang: bin\test-release-clang.exe

!IF "$(win_all_test_sources)"==" "
test_targets =
test_clang_targets =
!ELSE
test_targets = test-debug test-release
test_clang_targets = test-debug-clang test-release-clang
!ENDIF

all: debug release $(test_targets)
all-clang: debug-clang release-clang $(test_clang_targets)

clean:
    buildsystem\clean.bat

# Main binaries
bin\debug-msvc.exe: $(msvc_d_common_objs) $(msvc_d_main_objs)
    cl /Fe$@ $(MSVC_DEBUG_FLAGS) $** $(MSVC_DEBUG_LINK_FLAGS)

bin\release-msvc.exe: $(msvc_common_objs) $(msvc_main_objs)
    cl /Fe$@ $(MSVC_RELEASE_FLAGS) $** $(MSVC_RELEASE_LINK_FLAGS)

bin\debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_main_objs)
	$(clang_command) -o $@ $(WIN_CLANG_DEBUG_FLAGS) $** $(WIN_CLANG_DEBUG_LINK_FLAGS)

bin\release-clang.exe: $(win_clang_common_objs) $(win_clang_main_objs)
	$(clang_command) -o $@ $(WIN_CLANG_RELEASE_FLAGS) $** $(WIN_CLANG_RELEASE_LINK_FLAGS)

# Test binaries
bin\test-debug-msvc.exe: $(msvc_d_common_objs) $(msvc_d_test_objs)
    cl /Fe$@ $(MSVC_DEBUG_FLAGS) $** $(MSVC_DEBUG_LINK_FLAGS)

bin\test-release-msvc.exe: $(msvc_common_objs) $(msvc_test_objs)
    cl /Fe$@ $(MSVC_RELEASE_FLAGS) $** $(MSVC_RELEASE_LINK_FLAGS)

bin\test-debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_test_objs)
	$(clang_command) -o $@ $(WIN_CLANG_DEBUG_FLAGS) $** $(WIN_CLANG_DEBUG_LINK_FLAGS)

bin\test-release-clang.exe: $(win_clang_common_objs) $(win_clang_test_objs)
	$(clang_command) -o $@ $(WIN_CLANG_RELEASE_FLAGS) $**  $(WIN_CLANG_RELEASE_LINK_FLAGS)

# Dependencies
temp_all = $(win_all_sources:src/=)
all_files = $(temp_all:.c=)

!IF [buildsystem\NMakeHelper.bat "$(all_files)" \
"$(MSVC_RELEASE_FLAGS)" \
"$(WIN_CLANG_RELEASE_FLAGS)" \
$(source_ext) \
$(clang_command) \
]
!ENDIF

!INCLUDE buildsystem\NMakeTemp.dep

!IF [del buildsystem\NMakeTemp.dep]
!ENDIF
