# NMake makefile
!include Configuration.mk

temp_test_sources = $(TEST_SOURCES) $(TEST_WINDOWS_SOURCES)

!IF EXIST(build\NMakeHelp.mk)
!INCLUDE build\NMakeHelp.mk
!ENDIF

build\NMakeHelp.mk: Configuration.mk
    buildsystem\NMakeHelper.bat "$(COMMON_SOURCES) $(COMMON_WINDOWS_SOURCES)" "$(MAIN_SOURCES) $(MAIN_WINDOWS_SOURCES)" "$(temp_test_sources)"

!CMDSWITCHES +S
debug: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\debug-msvc.exe
release: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\release-msvc.exe
debug-clang: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\debug-clang.exe
release-clang: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\release-clang.exe
test-debug: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\test-debug-msvc.exe
test-release: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\test-release-msvc.exe
test-debug-clang: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\test-debug-clang.exe
test-release-clang: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\test-release-clang.exe

!IF "$(temp_test_sources)"==" "
test_targets =
test_clang_targets =
!ELSE
test_targets = bin\test-debug-msvc.exe bin\test-release-msvc.exe
test_clang_targets = bin\test-debug-clang.exe bin\test-release-clang.exe
!ENDIF

all: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\debug-msvc.exe bin\release-msvc.exe $(test_targets)
all-clang: build\NMakeHelp.mk
    $(MAKE) /NOLOGO bin\debug-clang.exe bin\release-clang.exe $(test_clang_targets)
!CMDSWITCHES -S

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
