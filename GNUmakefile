# GNU makefile
include Configuration.mk
dep_flags = -MMD -MP -MF $<

sources = $(common_sources) $(linux_sources)
win_sources = $(common_sources) $(windows_sources)

gcc_deps = $(sources:src/%.c=bin/objects/%.gcc.dep)
gcc_objs = $(sources:src/%.c=bin/objects/%.gcc.o)
gcc_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_linux_flags) $(gcc_flags)
gcc_d_objs = $(sources:src/%.c=bin/objects/%.gcc_d.o)
gcc_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_linux_flags) $(gcc_flags)

clang_deps = $(sources:src/%.c=bin/objects/%.clang.dep)
clang_objs = $(sources:src/%.c=bin/objects/%.clang.o)
clang_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_linux_flags) $(clang_flags)
clang_d_objs = $(sources:src/%.c=bin/objects/%.clang_d.o)
clang_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_linux_flags) $(clang_flags)

mingw_deps = $(win_sources:src/%.c=bin/objects/%.mingw.dep)
mingw_objs = $(win_sources:src/%.c=bin/objects/%.mingw.o)
mingw_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(gcc_flags) $(mingw_flags)
mingw_d_objs = $(win_sources:src/%.c=bin/objects/%.mingw_d.o)
mingw_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(gcc_flags) $(mingw_flags)

win_clang_deps = $(win_sources:src/%.c=bin/objects/%.win_clang.dep)
win_clang_objs = $(win_sources:src/%.c=bin/objects/%.win_clang.o)
win_clang_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(clang_flags)
win_clang_d_objs = $(win_sources:src/%.c=bin/objects/%.win_clang_d.o)
win_clang_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(clang_flags)

debug: bin/debug-gcc
release: bin/release-gcc
debug-clang: bin/debug-clang
release-clang: bin/release-clang

all: debug release debug-clang release-clang win-debug win-release

win-debug: bin/debug-mingw.exe
win-release: bin/release-mingw.exe
win-debug-clang: bin/debug-clang.exe
win-release-clang: bin/release-clang.exe

win-all: win-debug win-release win-debug-clang win-release-clang

clean:
	find bin/ -maxdepth 1 -type f -exec rm "{}" \;
	find bin/ -type f -name "*.o" -exec rm "{}" \;
	find bin/ -type f -name "*.dep" -exec rm "{}" \;

win-clean:
	buildsystem\clean.bat


bin/debug-gcc: $(gcc_d_objs)
	gcc -o $@ $^ $(gcc_d_allflags)

bin/release-gcc: $(gcc_objs)
	gcc -o $@ $^ $(gcc_allflags)

bin/debug-clang: $(clang_d_objs)
	clang -o $@ $^ $(clang_d_allflags)

bin/release-clang: $(clang_objs)
	clang -o $@ $^ $(clang_allflags)

bin/debug-mingw.exe: $(mingw_d_objs)
	x86_64-w64-mingw32-gcc -o $@ $^ $(mingw_d_allflags)

bin/release-mingw.exe: $(mingw_objs)
	x86_64-w64-mingw32-gcc -o $@ $^ $(mingw_allflags)

bin/debug-clang.exe: $(win_clang_d_objs)
	clang -o $@ $^ $(win_clang_d_allflags)

bin/release-clang.exe: $(win_clang_objs)
	clang -o $@ $^ $(win_clang_allflags)


bin/objects/%.gcc_d.o: bin/objects/%.gcc.dep
	gcc -o $@ -c $(gcc_d_allflags) src/$*.c

bin/objects/%.gcc.o: bin/objects/%.gcc.dep
	gcc -o $@ -c $(gcc_allflags) src/$*.c

bin/objects/%.clang_d.o: bin/objects/%.clang.dep
	clang -o $@ -c $(clang_d_allflags) src/$*.c

bin/objects/%.clang.o: bin/objects/%.clang.dep
	clang -o $@ -c $(clang_allflags) src/$*.c

bin/objects/%.mingw_d.o: bin/objects/%.mingw.dep
	x86_64-w64-mingw32-gcc -o $@ -c $(mingw_d_allflags) src/$*.c

bin/objects/%.mingw.o: bin/objects/%.mingw.dep
	x86_64-w64-mingw32-gcc -o $@ -c $(mingw_allflags) src/$*.c

bin/objects/%.win_clang_d.o: bin/objects/%.win_clang.dep
	clang -o $@ -c $(win_clang_d_allflags) src/$*.c

bin/objects/%.win_clang.o: bin/objects/%.win_clang.dep
	clang -o $@ -c $(win_clang_allflags) src/$*.c


bin/objects/%.gcc.dep: src/%.c
	gcc -c -MM -MP -MF $@ -MT "bin/objects/$*.gcc.o bin/objects/$*.gcc_d.o" $(gcc_allflags) src/$*.c

bin/objects/%.clang.dep: src/%.c
	clang -c -MM -MP -MF $@ -MT "bin/objects/$*.clang.o bin/objects/$*.clang_d.o" $(clang_allflags) src/$*.c

bin/objects/%.mingw.dep: src/%.c
	x86_64-w64-mingw32-gcc -c -MM -MP -MF $@ -MT "bin/objects/$*.mingw.o bin/objects/$*.mingw_d.o" $(mingw_allflags) src/$*.c

bin/objects/%.win_clang.dep: src/%.c
	clang -c -MM -MP -MF $@ -MT "bin/objects/$*.win_clang.o bin/objects/$*.win_clang_d.o" $(win_clang_allflags) src/$*.c

$(gcc_deps):
$(clang_deps):
$(mingw_deps):
$(win_clang_deps):

include_gcc_deps = debug release all
include_clang_deps = debug-clang release-clang all
include_mingw_deps = win-debug win-release win-all
include_win_clang_deps = win-debug-clang win-release-clang win-all

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
