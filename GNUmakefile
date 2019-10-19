# GNU makefile
include Configuration.mk

ifndef CPP
$(error CPP must be defined)
endif

ifneq ($(CPP), 0)
gcc_command = g++
clang_command = clang++
mingw_command = x86_64-w64-mingw32-g++
source_ext = cpp
else
gcc_command = gcc
clang_command = clang
mingw_command = x86_64-w64-mingw32-gcc
source_ext = c
endif

all_common_sources = $(COMMON_SOURCES) $(COMMON_LINUX_SOURCES)
all_main_sources = $(MAIN_SOURCES) $(MAIN_LINUX_SOURCES)
all_test_sources = $(TEST_SOURCES) $(TEST_LINUX_SOURCES)
all_sources = $(all_common_sources) $(all_main_sources) $(all_test_sources)
win_all_common_sources = $(COMMON_SOURCES) $(COMMON_WINDOWS_SOURCES)
win_all_main_sources = $(MAIN_SOURCES) $(MAIN_WINDOWS_SOURCES)
win_all_test_sources = $(TEST_SOURCES) $(TEST_WINDOWS_SOURCES)
win_all_sources = $(win_all_common_sources) $(win_all_main_sources) $(win_all_test_sources)

gcc_deps = $(all_sources:src/%.$(source_ext)=build/%.gcc.dep)
gcc_common_objs = $(all_common_sources:src/%.$(source_ext)=build/%.gcc.o)
gcc_main_objs = $(all_main_sources:src/%.$(source_ext)=build/%.gcc.o)
gcc_test_objs = $(all_test_sources:src/%.$(source_ext)=build/%.gcc.o)
gcc_d_common_objs = $(all_common_sources:src/%.$(source_ext)=build/%.gcc_d.o)
gcc_d_main_objs = $(all_main_sources:src/%.$(source_ext)=build/%.gcc_d.o)
gcc_d_test_objs = $(all_test_sources:src/%.$(source_ext)=build/%.gcc_d.o)

clang_deps = $(all_sources:src/%.$(source_ext)=build/%.clang.dep)
clang_common_objs = $(all_common_sources:src/%.$(source_ext)=build/%.clang.o)
clang_main_objs = $(all_main_sources:src/%.$(source_ext)=build/%.clang.o)
clang_test_objs = $(all_test_sources:src/%.$(source_ext)=build/%.clang.o)
clang_d_common_objs = $(all_common_sources:src/%.$(source_ext)=build/%.clang_d.o)
clang_d_main_objs = $(all_main_sources:src/%.$(source_ext)=build/%.clang_d.o)
clang_d_test_objs = $(all_test_sources:src/%.$(source_ext)=build/%.clang_d.o)

mingw_deps = $(win_all_sources:src/%.$(source_ext)=build/%.mingw.dep)
mingw_common_objs = $(win_all_common_sources:src/%.$(source_ext)=build/%.mingw.o)
mingw_main_objs = $(win_all_main_sources:src/%.$(source_ext)=build/%.mingw.o)
mingw_test_objs = $(win_all_test_sources:src/%.$(source_ext)=build/%.mingw.o)
mingw_d_common_objs = $(win_all_common_sources:src/%.$(source_ext)=build/%.mingw_d.o)
mingw_d_main_objs = $(win_all_main_sources:src/%.$(source_ext)=build/%.mingw_d.o)
mingw_d_test_objs = $(win_all_test_sources:src/%.$(source_ext)=build/%.mingw_d.o)

win_clang_deps = $(win_all_sources:src/%.$(source_ext)=build/%.win_gnu_clang.dep)
win_clang_common_objs = $(win_all_common_sources:src/%.$(source_ext)=build/%.win_gnu_clang.o)
win_clang_main_objs = $(win_all_main_sources:src/%.$(source_ext)=build/%.win_gnu_clang.o)
win_clang_test_objs = $(win_all_test_sources:src/%.$(source_ext)=build/%.win_gnu_clang.o)
win_clang_d_common_objs = $(win_all_common_sources:src/%.$(source_ext)=build/%.win_gnu_clang_d.o)
win_clang_d_main_objs = $(win_all_main_sources:src/%.$(source_ext)=build/%.win_gnu_clang_d.o)
win_clang_d_test_objs = $(win_all_test_sources:src/%.$(source_ext)=build/%.win_gnu_clang_d.o)

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
$(shell mkdir -p bin; find src -type d -exec mkdir -p -- build/{} \;)

clean:
	rm -rf bin
	rm -rf build
endif

# Main binaries
bin/debug-gcc: $(gcc_d_common_objs) $(gcc_d_main_objs)
	$(gcc_command) -o $@ $(GCC_DEBUG_FLAGS) $^ $(GCC_DEBUG_LINK_FLAGS)

bin/release-gcc: $(gcc_common_objs) $(gcc_main_objs)
	$(gcc_command) -o $@ $(GCC_RELEASE_FLAGS) $^ $(GCC_RELEASE_LINK_FLAGS)

bin/debug-clang: $(clang_d_common_objs) $(clang_d_main_objs)
	$(clang_command) -o $@ $(CLANG_DEBUG_FLAGS) $^ $(CLANG_DEBUG_LINK_FLAGS)

bin/release-clang: $(clang_common_objs) $(clang_main_objs)
	$(clang_command) -o $@ $(CLANG_RELEASE_FLAGS) $^ $(CLANG_RELEASE_LINK_FLAGS)

bin/debug-mingw.exe: $(mingw_d_common_objs) $(mingw_d_main_objs)
	$(mingw_command) -o $@ $(MINGW_DEBUG_FLAGS) $^ $(MINGW_DEBUG_LINK_FLAGS)

bin/release-mingw.exe: $(mingw_common_objs) $(mingw_main_objs)
	$(mingw_command) -o $@ $(MINGW_RELEASE_FLAGS) $^ $(MINGW_RELEASE_LINK_FLAGS)

bin/debug-gnu-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_main_objs)
	$(clang_command) -o $@ $(WIN_GNU_CLANG_DEBUG_FLAGS) $^ $(WIN_GNU_CLANG_DEBUG_LINK_FLAGS)

bin/release-gnu-clang.exe: $(win_clang_common_objs) $(win_clang_main_objs)
	$(clang_command) -o $@ $(WIN_GNU_CLANG_RELEASE_FLAGS) $^ $(WIN_GNU_CLANG_RELEASE_LINK_FLAGS)

# Test binaries
bin/test-debug-gcc: $(gcc_d_common_objs) $(gcc_d_test_objs)
	$(gcc_command) -o $@ $(GCC_DEBUG_FLAGS) $^ $(GCC_DEBUG_LINK_FLAGS)

bin/test-release-gcc: $(gcc_common_objs) $(gcc_test_objs)
	$(gcc_command) -o $@ $(GCC_RELEASE_FLAGS) $^ $(GCC_RELEASE_LINK_FLAGS)

bin/test-debug-clang: $(clang_d_common_objs) $(clang_d_test_objs)
	$(clang_command) -o $@ $(CLANG_DEBUG_FLAGS) $^ $(CLANG_DEBUG_LINK_FLAGS)

bin/test-release-clang: $(clang_common_objs) $(clang_test_objs)
	$(clang_command) -o $@ $(CLANG_RELEASE_FLAGS) $^ $(CLANG_RELEASE_LINK_FLAGS)

bin/test-debug-mingw.exe: $(mingw_d_common_objs) $(mingw_d_test_objs)
	$(mingw_command) -o $@ $(MINGW_DEBUG_FLAGS) $^ $(MINGW_DEBUG_LINK_FLAGS)

bin/test-release-mingw.exe: $(mingw_common_objs) $(mingw_test_objs)
	$(mingw_command) -o $@ $(MINGW_RELEASE_FLAGS) $^ $(MINGW_RELEASE_LINK_FLAGS)

bin/test-debug-gnu-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_test_objs)
	$(clang_command) -o $@ $(WIN_GNU_CLANG_DEBUG_FLAGS) $^ $(WIN_GNU_CLANG_DEBUG_LINK_FLAGS)

bin/test-release-gnu-clang.exe: $(win_clang_common_objs) $(win_clang_test_objs)
	$(clang_command) -o $@ $(WIN_GNU_CLANG_RELEASE_FLAGS) $^ $(WIN_GNU_CLANG_RELEASE_LINK_FLAGS)

# Objects
build/%.gcc_d.o: build/%.gcc.dep
	$(gcc_command) -o $@ -c $(GCC_DEBUG_FLAGS) src/$*.$(source_ext)

build/%.gcc.o: build/%.gcc.dep
	$(gcc_command) -o $@ -c $(GCC_RELEASE_FLAGS) src/$*.$(source_ext)

build/%.clang_d.o: build/%.clang.dep
	$(clang_command) -o $@ -c $(CLANG_DEBUG_FLAGS) src/$*.$(source_ext)

build/%.clang.o: build/%.clang.dep
	$(clang_command) -o $@ -c $(CLANG_RELEASE_FLAGS) src/$*.$(source_ext)

build/%.mingw_d.o: build/%.mingw.dep
	$(mingw_command) -o $@ -c $(MINGW_DEBUG_FLAGS) src/$*.$(source_ext)

build/%.mingw.o: build/%.mingw.dep
	$(mingw_command) -o $@ -c $(MINGW_RELEASE_FLAGS) src/$*.$(source_ext)

build/%.win_gnu_clang_d.o: build/%.win_gnu_clang.dep
	$(clang_command) -o $@ -c $(WIN_GNU_CLANG_DEBUG_FLAGS) src/$*.$(source_ext)

build/%.win_gnu_clang.o: build/%.win_gnu_clang.dep
	$(clang_command) -o $@ -c $(WIN_GNU_CLANG_RELEASE_FLAGS) src/$*.$(source_ext)

# Dependencies
build/%.gcc.dep: src/%.$(source_ext) Configuration.mk
	$(gcc_command) -c -MM -MP -MF $@ -MT "build/$*.gcc.o build/$*.gcc_d.o" $(GCC_RELEASE_FLAGS) src/$*.$(source_ext)

build/%.clang.dep: src/%.$(source_ext) Configuration.mk
	$(clang_command) -c -MM -MP -MF $@ -MT "build/$*.clang.o build/$*.clang_d.o" $(CLANG_RELEASE_FLAGS) src/$*.$(source_ext)

build/%.mingw.dep: src/%.$(source_ext) Configuration.mk
	$(mingw_command) -c -MM -MP -MF $@ -MT "build/$*.mingw.o build/$*.mingw_d.o" $(MINGW_RELEASE_FLAGS) src/$*.$(source_ext)

build/%.win_gnu_clang.dep: src/%.$(source_ext) Configuration.mk
	$(clang_command) -c -MM -MP -MF $@ -MT "build/$*.win_gnu_clang.o build/$*.win_gnu_clang_d.o" $(WIN_GNU_CLANG_RELEASE_FLAGS) src/$*.$(source_ext)

$(gcc_deps):
$(clang_deps):
$(mingw_deps):
$(win_clang_deps):

include_gcc_deps = debug test-debug release test-release all
include_clang_deps = debug-clang test-debug-clang release-clang test-release-clang all-clang
include_mingw_deps = win-debug win-test-debug win-release win-test-release win-all
include_win_clang_deps = win-debug-clang win-test-debug-clang win-release-clang win-test-release-clang win-all-clang

ifneq ($(filter $(MAKECMDGOALS),$(include_gcc_deps)),)
include $(wildcard $(gcc_deps))
endif
ifneq ($(filter $(MAKECMDGOALS),$(include_clang_deps)),)
include $(wildcard $(clang_deps))
endif
ifneq ($(filter $(MAKECMDGOALS),$(include_mingw_deps)),)
include $(wildcard $(mingw_deps))
endif
ifneq ($(filter $(MAKECMDGOALS),$(include_win_clang_deps)),)
include $(wildcard $(win_clang_deps))
endif