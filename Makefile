# NMake makefile
!include Configuration.mk

temp_test_sources = $(TEST_SOURCES) $(TEST_WINDOWS_SOURCES)

!IF [buildsystem\NMakeHelper.bat "$(COMMON_SOURCES) $(COMMON_WINDOWS_SOURCES)" "$(MAIN_SOURCES) $(MAIN_WINDOWS_SOURCES)" "$(temp_test_sources)"]
!ENDIF
!INCLUDE build\NMakeHelp.mk

debug: bin\debug-msvc.exe
release: bin\release-msvc.exe
debug-clang: bin\debug-clang.exe
release-clang: bin\release-clang.exe
test-debug: bin\test-debug-msvc.exe
test-release: bin\test-release-msvc.exe
test-debug-clang: bin\test-debug-clang.exe
test-release-clang: bin\test-release-clang.exe

!IF "$(temp_test_sources)"==" "
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
    $(MSVC_COMMAND) /Fe$@ $(MSVC_DEBUG_FLAGS) $** $(MSVC_DEBUG_LINK_FLAGS)

bin\release-msvc.exe: $(msvc_common_objs) $(msvc_main_objs)
    $(MSVC_COMMAND) /Fe$@ $(MSVC_RELEASE_FLAGS) $** $(MSVC_RELEASE_LINK_FLAGS)

bin\debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_main_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_CLANG_DEBUG_FLAGS) $** $(WIN_CLANG_DEBUG_LINK_FLAGS)

bin\release-clang.exe: $(win_clang_common_objs) $(win_clang_main_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_CLANG_RELEASE_FLAGS) $** $(WIN_CLANG_RELEASE_LINK_FLAGS)

# Test binaries
bin\test-debug-msvc.exe: $(msvc_d_common_objs) $(msvc_d_test_objs)
    $(MSVC_COMMAND) /Fe$@ $(MSVC_DEBUG_FLAGS) $** $(MSVC_DEBUG_LINK_FLAGS)

bin\test-release-msvc.exe: $(msvc_common_objs) $(msvc_test_objs)
    $(MSVC_COMMAND) /Fe$@ $(MSVC_RELEASE_FLAGS) $** $(MSVC_RELEASE_LINK_FLAGS)

bin\test-debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_test_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_CLANG_DEBUG_FLAGS) $** $(WIN_CLANG_DEBUG_LINK_FLAGS)

bin\test-release-clang.exe: $(win_clang_common_objs) $(win_clang_test_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_CLANG_RELEASE_FLAGS) $**  $(WIN_CLANG_RELEASE_LINK_FLAGS)
