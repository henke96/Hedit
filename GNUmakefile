# GNU makefile
include Configuration.mk
dep_flags = -MMD -MP -MF $<

mingw_deps = $(sources:src/%.c=bin/objects/%_mingw.dep)
mingw_objs = $(sources:src/%.c=bin/objects/%_mingw.o)
mingw_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(gcc_flags) $(mingw_flags)
mingw_d_objs = $(sources:src/%.c=bin/objects/%_mingw_d.o)
mingw_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(gcc_flags) $(mingw_flags)

clang_deps = $(sources:src/%.c=bin/objects/%_clang.dep)
clang_objs = $(sources:src/%.c=bin/objects/%_clang.o)
clang_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags)
clang_d_objs = $(sources:src/%.c=bin/objects/%_clang_d.o)
clang_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags)

win-debug: bin/debug-mingw.exe
win-release: bin/release-mingw.exe
win-debug-clang: bin/debug-clang.exe
win-release-clang: bin/release-clang.exe

win-all: win-debug win-release win-debug-clang win-release-clang

win-clean:
	buildsystem\clean.bat

bin/debug-mingw.exe: $(mingw_d_objs)
	x86_64-w64-mingw32-gcc -o $@ $^ -g

bin/release-mingw.exe: $(mingw_objs)
	x86_64-w64-mingw32-gcc -o $@ $^

bin/debug-clang.exe: $(clang_d_objs)
	clang -o $@ $^ -g

bin/release-clang.exe: $(clang_objs)
	clang -o $@ $^


bin/objects/%_mingw_d.o: bin/objects/%_mingw.dep
	x86_64-w64-mingw32-gcc -o $@ -c $(mingw_d_allflags) src/$*.c

bin/objects/%_mingw.o: bin/objects/%_mingw.dep
	x86_64-w64-mingw32-gcc -o $@ -c $(mingw_allflags) src/$*.c

bin/objects/%_clang_d.o: bin/objects/%_clang.dep
	clang -o $@ -c $(clang_d_allflags) src/$*.c

bin/objects/%_clang.o: bin/objects/%_clang.dep
	clang -o $@ -c $(clang_allflags) src/$*.c

bin/objects/%_mingw.dep: src/%.c
	x86_64-w64-mingw32-gcc -c -MM -MP -MF $@ -MT "bin/objects/$*_mingw.o bin/objects/$*_mingw_d.o" $(mingw_allflags) src/$*.c

bin/objects/%_clang.dep: src/%.c
	clang -c -MM -MP -MF $@ -MT "bin/objects/$*_clang.o bin/objects/$*_clang_d.o" $(clang_allflags) src/$*.c

$(mingw_deps):
$(clang_deps):

include $(wildcard $(mingw_deps))
include $(wildcard $(clang_deps))
