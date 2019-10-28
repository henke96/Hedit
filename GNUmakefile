# GNU makefile
include Configuration.mk

all_common_sources = $(COMMON_SOURCES) $(COMMON_LINUX_SOURCES)
all_main_sources = $(MAIN_SOURCES) $(MAIN_LINUX_SOURCES)
all_test_sources = $(TEST_SOURCES) $(TEST_LINUX_SOURCES)
all_sources = $(all_common_sources) $(all_main_sources) $(all_test_sources)
win_all_common_sources = $(COMMON_SOURCES) $(COMMON_WINDOWS_SOURCES)
win_all_main_sources = $(MAIN_SOURCES) $(MAIN_WINDOWS_SOURCES)
win_all_test_sources = $(TEST_SOURCES) $(TEST_WINDOWS_SOURCES)
win_all_sources = $(win_all_common_sources) $(win_all_main_sources) $(win_all_test_sources)

debug: bin/debug-gcc
release: bin/release-gcc
debug-clang: bin/debug-clang
release-clang: bin/release-clang
test-debug: bin/test-debug-gcc
test-release: bin/test-release-gcc
test-debug-clang: bin/test-debug-clang
test-release-clang: bin/test-release-clang

ifneq ($(strip $(all_test_sources)),)
test_targets = test-debug test-release
test_clang_targets = test-debug-clang test-release-clang
else
test_targets =
test_clang_targets =
endif

all: debug release $(test_targets)
all-clang: debug-clang release-clang $(test_clang_targets)

win-debug: bin/debug-mingw.exe
win-release: bin/release-mingw.exe
win-debug-clang: bin/debug-gnu-clang.exe
win-release-clang: bin/release-gnu-clang.exe
win-test-debug: bin/test-debug-mingw.exe
win-test-release: bin/test-release-mingw.exe
win-test-debug-clang: bin/test-debug-gnu-clang.exe
win-test-release-clang: bin/test-release-gnu-clang.exe

ifneq ($(strip $(win_all_test_sources)),)
win_test_targets = win-test-debug win-test-release
win_test_clang_targets = win-test-debug-clang win-test-release-clang
else
win_test_targets =
win_test_clang_targets =
endif

win-all: win-debug win-release $(win_test_targets)
win-all-clang: win-debug-clang win-release-clang $(win_test_clang_targets)

ifeq ($(OS), Windows_NT)
$(shell buildsystem\createDirs.bat)
clean:
	buildsystem\clean.bat
else
$(shell mkdir -p bin; cd src && find . -type d -exec mkdir -p -- ../build/{} \;)
clean:
	rm -rf bin
	rm -rf build
endif

need_gcc_d = debug test-debug all
need_gcc = release test-release all asm
need_clang_d = debug-clang test-debug-clang all-clang
need_clang = release-clang test-release-clang all-clang asm-clang
need_mingw_d = win-debug win-test-debug win-all
need_mingw = win-release win-test-release win-all win-asm
need_win_clang_d = win-debug-clang win-test-debug-clang win-all-clang
need_win_clang = win-release-clang win-test-release-clang win-all-clang win-asm-clang

ifneq ($(filter $(MAKECMDGOALS),$(need_gcc_d)),)
gcc_d_deps = $(all_sources:%=build/%.gcc_d.dep)
gcc_d_common_objs = $(all_common_sources:%=build/%.gcc_d.o)
gcc_d_main_objs = $(all_main_sources:%=build/%.gcc_d.o)
gcc_d_test_objs = $(all_test_sources:%=build/%.gcc_d.o)

bin/debug-gcc: $(gcc_d_common_objs) $(gcc_d_main_objs)
	$(GCC_COMMAND) -o $@ $(GCC_DEBUG_FLAGS) $^ $(GCC_DEBUG_LINK_FLAGS)
bin/test-debug-gcc: $(gcc_d_common_objs) $(gcc_d_test_objs)
	$(GCC_COMMAND) -o $@ $(GCC_DEBUG_FLAGS) $^ $(GCC_DEBUG_LINK_FLAGS)
build/%.gcc_d.o: build/%.gcc_d.dep
	$(GCC_COMMAND) -o $@ -c $(GCC_DEBUG_FLAGS) src/$*
build/%.gcc_d.dep: src/% Configuration.mk
	$(GCC_COMMAND) -c -MM -MP -MF $@ -MT build/$*.gcc_d.o $(GCC_DEBUG_FLAGS) src/$*
$(gcc_d_deps)
include $(wildcard $(gcc_d_deps))
endif

ifneq ($(filter $(MAKECMDGOALS),$(need_gcc)),)
gcc_deps = $(all_sources:%=build/%.gcc.dep)
gcc_asm = $(win_all_sources:%=build/%.gcc.asm)
gcc_common_objs = $(all_common_sources:%=build/%.gcc.o)
gcc_main_objs = $(all_main_sources:%=build/%.gcc.o)
gcc_test_objs = $(all_test_sources:%=build/%.gcc.o)

asm: $(gcc_asm)

bin/release-gcc: $(gcc_common_objs) $(gcc_main_objs)
	$(GCC_COMMAND) -o $@ $(GCC_RELEASE_FLAGS) $^ $(GCC_RELEASE_LINK_FLAGS)
bin/test-release-gcc: $(gcc_common_objs) $(gcc_test_objs)
	$(GCC_COMMAND) -o $@ $(GCC_RELEASE_FLAGS) $^ $(GCC_RELEASE_LINK_FLAGS)
build/%.gcc.asm: build/%.gcc.dep
	$(GCC_COMMAND) -o $@ -S -masm=intel -fverbose-asm $(GCC_RELEASE_FLAGS) src/$*
build/%.gcc.o: build/%.gcc.dep
	$(GCC_COMMAND) -o $@ -c $(GCC_RELEASE_FLAGS) src/$*
build/%.gcc.dep: src/% Configuration.mk
	$(GCC_COMMAND) -c -MM -MP -MF $@ -MT "build/$*.gcc.o build/$*.gcc.asm" $(GCC_RELEASE_FLAGS) src/$*
$(gcc_deps):
include $(wildcard $(gcc_deps))
endif

ifneq ($(filter $(MAKECMDGOALS),$(need_clang_d)),)
clang_d_deps = $(all_sources:%=build/%.clang_d.dep)
clang_d_common_objs = $(all_common_sources:%=build/%.clang_d.o)
clang_d_main_objs = $(all_main_sources:%=build/%.clang_d.o)
clang_d_test_objs = $(all_test_sources:%=build/%.clang_d.o)

bin/debug-clang: $(clang_d_common_objs) $(clang_d_main_objs)
	$(CLANG_COMMAND) -o $@ $(CLANG_DEBUG_FLAGS) $^ $(CLANG_DEBUG_LINK_FLAGS)
bin/test-debug-clang: $(clang_d_common_objs) $(clang_d_test_objs)
	$(CLANG_COMMAND) -o $@ $(CLANG_DEBUG_FLAGS) $^ $(CLANG_DEBUG_LINK_FLAGS)
build/%.clang_d.o: build/%.clang_d.dep
	$(CLANG_COMMAND) -o $@ -c $(CLANG_DEBUG_FLAGS) src/$*
build/%.clang_d.dep: src/% Configuration.mk
	$(CLANG_COMMAND) -c -MM -MP -MF $@ -MT build/$*.clang_d.o $(CLANG_DEBUG_FLAGS) src/$*
$(clang_d_deps):
include $(wildcard $(clang_d_deps))
endif

ifneq ($(filter $(MAKECMDGOALS),$(need_clang)),)
clang_deps = $(all_sources:%=build/%.clang.dep)
clang_asm = $(win_all_sources:%=build/%.clang.asm)
clang_common_objs = $(all_common_sources:%=build/%.clang.o)
clang_main_objs = $(all_main_sources:%=build/%.clang.o)
clang_test_objs = $(all_test_sources:%=build/%.clang.o)

asm-clang: $(clang_asm)

bin/release-clang: $(clang_common_objs) $(clang_main_objs)
	$(CLANG_COMMAND) -o $@ $(CLANG_RELEASE_FLAGS) $^ $(CLANG_RELEASE_LINK_FLAGS)
bin/test-release-clang: $(clang_common_objs) $(clang_test_objs)
	$(CLANG_COMMAND) -o $@ $(CLANG_RELEASE_FLAGS) $^ $(CLANG_RELEASE_LINK_FLAGS)
build/%.clang.asm: build/%.clang.dep
	$(CLANG_COMMAND) -o $@ -S -masm=intel -fverbose-asm $(CLANG_RELEASE_FLAGS) src/$*
build/%.clang.o: build/%.clang.dep
	$(CLANG_COMMAND) -o $@ -c $(CLANG_RELEASE_FLAGS) src/$*
build/%.clang.dep: src/% Configuration.mk
	$(CLANG_COMMAND) -c -MM -MP -MF $@ -MT "build/$*.clang.o build/$*.clang.asm" $(CLANG_RELEASE_FLAGS) src/$*
$(clang_deps):
include $(wildcard $(clang_deps))
endif

ifneq ($(filter $(MAKECMDGOALS),$(need_mingw_d)),)
mingw_d_deps = $(win_all_sources:%=build/%.mingw_d.dep)
mingw_d_common_objs = $(win_all_common_sources:%=build/%.mingw_d.o)
mingw_d_main_objs = $(win_all_main_sources:%=build/%.mingw_d.o)
mingw_d_test_objs = $(win_all_test_sources:%=build/%.mingw_d.o)

bin/debug-mingw.exe: $(mingw_d_common_objs) $(mingw_d_main_objs)
	$(MINGW_COMMAND) -o $@ $(MINGW_DEBUG_FLAGS) $^ $(MINGW_DEBUG_LINK_FLAGS)
bin/test-debug-mingw.exe: $(mingw_d_common_objs) $(mingw_d_test_objs)
	$(MINGW_COMMAND) -o $@ $(MINGW_DEBUG_FLAGS) $^ $(MINGW_DEBUG_LINK_FLAGS)
build/%.mingw_d.o: build/%.mingw_d.dep
	$(MINGW_COMMAND) -o $@ -c $(MINGW_DEBUG_FLAGS) src/$*
build/%.mingw_d.dep: src/% Configuration.mk
	$(MINGW_COMMAND) -c -MM -MP -MF $@ -MT build/$*.mingw_d.o $(MINGW_DEBUG_FLAGS) src/$*
$(mingw_d_deps):
include $(wildcard $(mingw_d_deps))
endif

ifneq ($(filter $(MAKECMDGOALS),$(need_mingw)),)
mingw_deps = $(win_all_sources:%=build/%.mingw.dep)
mingw_asm = $(win_all_sources:%=build/%.mingw.asm)
mingw_common_objs = $(win_all_common_sources:%=build/%.mingw.o)
mingw_main_objs = $(win_all_main_sources:%=build/%.mingw.o)
mingw_test_objs = $(win_all_test_sources:%=build/%.mingw.o)

win-asm: $(mingw_asm)

bin/release-mingw.exe: $(mingw_common_objs) $(mingw_main_objs)
	$(MINGW_COMMAND) -o $@ $(MINGW_RELEASE_FLAGS) $^ $(MINGW_RELEASE_LINK_FLAGS)
bin/test-release-mingw.exe: $(mingw_common_objs) $(mingw_test_objs)
	$(MINGW_COMMAND) -o $@ $(MINGW_RELEASE_FLAGS) $^ $(MINGW_RELEASE_LINK_FLAGS)
build/%.mingw.asm: build/%.mingw.dep
	$(MINGW_COMMAND) -o $@ -S -masm=intel -fverbose-asm $(MINGW_RELEASE_FLAGS) src/$*
build/%.mingw.o: build/%.mingw.dep
	$(MINGW_COMMAND) -o $@ -c $(MINGW_RELEASE_FLAGS) src/$*
build/%.mingw.dep: src/% Configuration.mk
	$(MINGW_COMMAND) -c -MM -MP -MF $@ -MT "build/$*.mingw.o build/$*.mingw.asm" $(MINGW_RELEASE_FLAGS) src/$*
$(mingw_deps):
include $(wildcard $(mingw_deps))
endif

ifneq ($(filter $(MAKECMDGOALS),$(need_win_clang_d)),)
win_clang_d_deps = $(win_all_sources:%=build/%.win_gnu_clang_d.dep)
win_clang_d_common_objs = $(win_all_common_sources:%=build/%.win_gnu_clang_d.o)
win_clang_d_main_objs = $(win_all_main_sources:%=build/%.win_gnu_clang_d.o)
win_clang_d_test_objs = $(win_all_test_sources:%=build/%.win_gnu_clang_d.o)

bin/debug-gnu-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_main_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_GNU_CLANG_DEBUG_FLAGS) $^ $(WIN_GNU_CLANG_DEBUG_LINK_FLAGS)
bin/test-debug-gnu-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_test_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_GNU_CLANG_DEBUG_FLAGS) $^ $(WIN_GNU_CLANG_DEBUG_LINK_FLAGS)
build/%.win_gnu_clang_d.o: build/%.win_gnu_clang_d.dep
	$(CLANG_COMMAND) -o $@ -c $(WIN_GNU_CLANG_DEBUG_FLAGS) src/$*
build/%.win_gnu_clang_d.dep: src/% Configuration.mk
	$(CLANG_COMMAND) -c -MM -MP -MF $@ -MT build/$*.win_gnu_clang_d.o $(WIN_GNU_CLANG_DEBUG_FLAGS) src/$*
$(win_clang_d_deps):
include $(wildcard $(win_clang_d_deps))
endif

ifneq ($(filter $(MAKECMDGOALS),$(need_win_clang)),)
win_clang_deps = $(win_all_sources:%=build/%.win_gnu_clang.dep)
win_clang_asm = $(win_all_sources:%=build/%.win_gnu_clang.asm)
win_clang_common_objs = $(win_all_common_sources:%=build/%.win_gnu_clang.o)
win_clang_main_objs = $(win_all_main_sources:%=build/%.win_gnu_clang.o)
win_clang_test_objs = $(win_all_test_sources:%=build/%.win_gnu_clang.o)

win-asm-clang: $(win_clang_asm)

bin/release-gnu-clang.exe: $(win_clang_common_objs) $(win_clang_main_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_GNU_CLANG_RELEASE_FLAGS) $^ $(WIN_GNU_CLANG_RELEASE_LINK_FLAGS)
bin/test-release-gnu-clang.exe: $(win_clang_common_objs) $(win_clang_test_objs)
	$(CLANG_COMMAND) -o $@ $(WIN_GNU_CLANG_RELEASE_FLAGS) $^ $(WIN_GNU_CLANG_RELEASE_LINK_FLAGS)
build/%.win_gnu_clang.asm: build/%.win_gnu_clang.dep
	$(CLANG_COMMAND) -o $@ -S -masm=intel -fverbose-asm $(WIN_GNU_CLANG_RELEASE_FLAGS) src/$*
build/%.win_gnu_clang.o: build/%.win_gnu_clang.dep
	$(CLANG_COMMAND) -o $@ -c $(WIN_GNU_CLANG_RELEASE_FLAGS) src/$*
build/%.win_gnu_clang.dep: src/% Configuration.mk
	$(CLANG_COMMAND) -c -MM -MP -MF $@ -MT "build/$*.win_gnu_clang.o build/$*.win_gnu_clang.asm" $(WIN_GNU_CLANG_RELEASE_FLAGS) src/$*
$(win_clang_deps):
include $(wildcard $(win_clang_deps))
endif