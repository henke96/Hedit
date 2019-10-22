!include Configuration.mk
.SUFFIXES :
!INCLUDE build\NMakeHelp.mk

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

# Assembly (not used for building)
asm: $(msvc_asm)
asm-clang: $(win_clang_asm)
