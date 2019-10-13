# GNU makefile
include Configuration.mk
dep_flags = -MMD -MP -MF $<

all_common_sources = $(common_sources) $(common_linux_sources)
all_main_sources = $(main_sources) $(main_linux_sources)
all_test_sources = $(test_sources) $(test_linux_sources)
all_sources = $(all_common_sources) $(all_main_sources) $(all_test_sources)
win_all_common_sources = $(common_sources) $(common_windows_sources)
win_all_main_sources = $(main_sources) $(main_windows_sources)
win_all_test_sources = $(test_sources) $(test_windows_sources)
win_all_sources = $(win_all_common_sources) $(win_all_main_sources) $(win_all_test_sources)

gcc_deps = $(all_sources:src/%.c=build/%.gcc.dep)
gcc_common_objs = $(all_common_sources:src/%.c=build/%.gcc.o)
gcc_main_objs = $(all_main_sources:src/%.c=build/%.gcc.o)
gcc_test_objs = $(all_test_sources:src/%.c=build/%.gcc.o)
gcc_d_common_objs = $(all_common_sources:src/%.c=build/%.gcc_d.o)
gcc_d_main_objs = $(all_main_sources:src/%.c=build/%.gcc_d.o)
gcc_d_test_objs = $(all_test_sources:src/%.c=build/%.gcc_d.o)
gcc_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_linux_flags) $(gcc_flags)
gcc_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_linux_flags) $(gcc_flags)

clang_deps = $(all_sources:src/%.c=build/%.clang.dep)
clang_common_objs = $(all_common_sources:src/%.c=build/%.clang.o)
clang_main_objs = $(all_main_sources:src/%.c=build/%.clang.o)
clang_test_objs = $(all_test_sources:src/%.c=build/%.clang.o)
clang_d_common_objs = $(all_common_sources:src/%.c=build/%.clang_d.o)
clang_d_main_objs = $(all_main_sources:src/%.c=build/%.clang_d.o)
clang_d_test_objs = $(all_test_sources:src/%.c=build/%.clang_d.o)
clang_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_linux_flags) $(clang_flags)
clang_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_linux_flags) $(clang_flags)

mingw_deps = $(win_all_sources:src/%.c=build/%.mingw.dep)
mingw_common_objs = $(win_all_common_sources:src/%.c=build/%.mingw.o)
mingw_main_objs = $(win_all_main_sources:src/%.c=build/%.mingw.o)
mingw_test_objs = $(win_all_test_sources:src/%.c=build/%.mingw.o)
mingw_d_common_objs = $(win_all_common_sources:src/%.c=build/%.mingw_d.o)
mingw_d_main_objs = $(win_all_main_sources:src/%.c=build/%.mingw_d.o)
mingw_d_test_objs = $(win_all_test_sources:src/%.c=build/%.mingw_d.o)
mingw_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(gcc_flags) $(mingw_flags)
mingw_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(gcc_flags) $(mingw_flags)

win_clang_deps = $(win_all_sources:src/%.c=build/%.win_clang.dep)
win_clang_common_objs = $(win_all_common_sources:src/%.c=build/%.win_clang.o)
win_clang_main_objs = $(win_all_main_sources:src/%.c=build/%.win_clang.o)
win_clang_test_objs = $(win_all_test_sources:src/%.c=build/%.win_clang.o)
win_clang_d_common_objs = $(win_all_common_sources:src/%.c=build/%.win_clang_d.o)
win_clang_d_main_objs = $(win_all_main_sources:src/%.c=build/%.win_clang_d.o)
win_clang_d_test_objs = $(win_all_test_sources:src/%.c=build/%.win_clang_d.o)
win_clang_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(clang_flags)
win_clang_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(clang_flags)

debug: bin/debug-gcc
release: bin/release-gcc
debug-clang: bin/debug-clang
release-clang: bin/release-clang
test-debug: bin/test-debug-gcc
test-release: bin/test-release-gcc
test-debug-clang: bin/test-debug-clang
test-release-clang: bin/test-release-clang

all: debug test-debug release test-release debug-clang test-debug-clang release-clang test-release-clang win-debug win-test-debug win-release win-test-release

win-debug: bin/debug-mingw.exe
win-release: bin/release-mingw.exe
win-debug-clang: bin/debug-clang.exe
win-release-clang: bin/release-clang.exe
win-test-debug: bin/test-debug-mingw.exe
win-test-release: bin/test-release-mingw.exe
win-test-debug-clang: bin/test-debug-clang.exe
win-test-release-clang: bin/test-release-clang.exe

win-all: win-debug win-test-debug win-release win-test-release win-debug-clang win-test-debug-clang win-release-clang win-test-release-clang

clean:
	find bin/ -maxdepth 1 -type f -exec rm "{}" \;
	find build/ -maxdepth 1 -type f -exec rm "{}" \;
	find build/ -type f -name "*.o" -exec rm "{}" \;
	find build/ -type f -name "*.dep" -exec rm "{}" \;

win-clean:
	buildsystem\clean.bat

bin:
	mkdir bin

# Main binaries
bin/debug-gcc: $(gcc_d_common_objs) $(gcc_d_main_objs) | bin
	gcc -o $@ $(gcc_d_allflags) $(gcc_d_common_objs) $(gcc_d_main_objs)

bin/release-gcc: $(gcc_common_objs) $(gcc_main_objs) | bin
	gcc -o $@ $(gcc_allflags) $(gcc_common_objs) $(gcc_main_objs)

bin/debug-clang: $(clang_d_common_objs) $(clang_d_main_objs) | bin
	clang -o $@ $(clang_d_allflags) $(clang_d_common_objs) $(clang_d_main_objs)

bin/release-clang: $(clang_common_objs) $(clang_main_objs) | bin
	clang -o $@ $(clang_allflags) $(clang_common_objs) $(clang_main_objs)

bin/debug-mingw.exe: $(mingw_d_common_objs) $(mingw_d_main_objs) | bin
	x86_64-w64-mingw32-gcc -o $@ $(mingw_d_allflags) $(mingw_d_common_objs) $(mingw_d_main_objs)

bin/release-mingw.exe: $(mingw_common_objs) $(mingw_main_objs) | bin
	x86_64-w64-mingw32-gcc -o $@ $(mingw_allflags) $(mingw_common_objs) $(mingw_main_objs)

bin/debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_main_objs) | bin
	clang -o $@ $(win_clang_d_allflags) $(win_clang_d_common_objs) $(win_clang_d_main_objs)

bin/release-clang.exe: $(win_clang_common_objs) $(win_clang_main_objs) | bin
	clang -o $@ $(win_clang_allflags) $(win_clang_common_objs) $(win_clang_main_objs)

# Test binaries
bin/test-debug-gcc: $(gcc_d_common_objs) $(gcc_d_test_objs) | bin
	gcc -o $@ $(gcc_d_allflags) $(gcc_d_common_objs) $(gcc_d_test_objs)

bin/test-release-gcc: $(gcc_common_objs) $(gcc_test_objs) | bin
	gcc -o $@ $(gcc_allflags) $(gcc_common_objs) $(gcc_test_objs)

bin/test-debug-clang: $(clang_d_common_objs) $(clang_d_test_objs) | bin
	clang -o $@ $(clang_d_allflags) $(clang_d_common_objs) $(clang_d_test_objs)

bin/test-release-clang: $(clang_common_objs) $(clang_test_objs) | bin
	clang -o $@ $(clang_allflags) $(clang_common_objs) $(clang_test_objs)

bin/test-debug-mingw.exe: $(mingw_d_common_objs) $(mingw_d_test_objs) | bin
	x86_64-w64-mingw32-gcc -o $@ $(mingw_d_allflags) $(mingw_d_common_objs) $(mingw_d_test_objs)

bin/test-release-mingw.exe: $(mingw_common_objs) $(mingw_test_objs) | bin
	x86_64-w64-mingw32-gcc -o $@ $(mingw_allflags) $(mingw_common_objs) $(mingw_test_objs)

bin/test-debug-clang.exe: $(win_clang_d_common_objs) $(win_clang_d_test_objs) | bin
	clang -o $@ $(win_clang_d_allflags) $(win_clang_d_common_objs) $(win_clang_d_test_objs)

bin/test-release-clang.exe: $(win_clang_common_objs) $(win_clang_test_objs) | bin
	clang -o $@ $(win_clang_allflags) $(win_clang_common_objs) $(win_clang_test_objs)

# Objects
build/%.gcc_d.o: build/%.gcc.dep
	gcc -o $@ -c $(gcc_d_allflags) src/$*.c

build/%.gcc.o: build/%.gcc.dep
	gcc -o $@ -c $(gcc_allflags) src/$*.c

build/%.clang_d.o: build/%.clang.dep
	clang -o $@ -c $(clang_d_allflags) src/$*.c

build/%.clang.o: build/%.clang.dep
	clang -o $@ -c $(clang_allflags) src/$*.c

build/%.mingw_d.o: build/%.mingw.dep
	x86_64-w64-mingw32-gcc -o $@ -c $(mingw_d_allflags) src/$*.c

build/%.mingw.o: build/%.mingw.dep
	x86_64-w64-mingw32-gcc -o $@ -c $(mingw_allflags) src/$*.c

build/%.win_clang_d.o: build/%.win_clang.dep
	clang -o $@ -c $(win_clang_d_allflags) src/$*.c

build/%.win_clang.o: build/%.win_clang.dep
	clang -o $@ -c $(win_clang_allflags) src/$*.c

# Dependencies
build/%.gcc.dep: src/%.c
	gcc -c -MM -MP -MF $@ -MT "build/$*.gcc.o build/$*.gcc_d.o" $(gcc_allflags) src/$*.c

build/%.clang.dep: src/%.c
	clang -c -MM -MP -MF $@ -MT "build/$*.clang.o build/$*.clang_d.o" $(clang_allflags) src/$*.c

build/%.mingw.dep: src/%.c
	x86_64-w64-mingw32-gcc -c -MM -MP -MF $@ -MT "build/$*.mingw.o build/$*.mingw_d.o" $(mingw_allflags) src/$*.c

build/%.win_clang.dep: src/%.c
	clang -c -MM -MP -MF $@ -MT "build/$*.win_clang.o build/$*.win_clang_d.o" $(win_clang_allflags) src/$*.c

$(gcc_deps):
$(clang_deps):
$(mingw_deps):
$(win_clang_deps):

include_gcc_deps = debug test-debug release test-release all
include_clang_deps = debug-clang test-debug-clang release-clang test-release-clang all
include_mingw_deps = win-debug win-test-debug win-release win-test-release win-all
include_win_clang_deps = win-debug-clang win-test-debug-clang win-release-clang win-test-release-clang win-all

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
