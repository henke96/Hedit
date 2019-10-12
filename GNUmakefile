# GNU makefile
include Configuration.mk
dep_flags = -MMD -MP -MF $<

mingw_objs = $(sources:src/%.c=bin/objects/%_mingw.o)
mingw_deps = $(sources:src/%.c=bin/objects/%_mingw.dep)
mingw_allflags = $(gnu_flags) $(gnu_release_flags) $(gnu_windows_flags) $(gcc_flags) $(gcc_release_flags) $(mingw_flags)
mingw_d_objs = $(sources:src/%.c=bin/objects/%_mingw_d.o)
mingw_d_deps = $(sources:src/%.c=bin/objects/%_mingw_d.dep)
mingw_d_allflags = $(gnu_flags) $(gnu_debug_flags) $(gnu_windows_flags) $(gcc_flags) $(mingw_flags)

win-debug: bin/debug-mingw.exe
win-release: bin/release-mingw.exe

win-all: win-debug win-release

win-clean:
	buildsystem\clean.bat

bin/debug-mingw.exe: $(mingw_d_objs)
	x86_64-w64-mingw32-gcc -o $@ $^ $(mingw_d_allflags)

bin/release-mingw.exe: $(mingw_objs)
	x86_64-w64-mingw32-gcc -o $@ $^ $(mingw_allflags)

bin/objects/%_mingw_d.o: bin/objects/%_mingw_d.dep src/%.c
	x86_64-w64-mingw32-gcc -o $@ -c $(dep_flags) $(mingw_d_allflags) src/$*.c

bin/objects/%_mingw.o: bin/objects/%_mingw.dep src/%.c
	x86_64-w64-mingw32-gcc -o $@ -c $(dep_flags) $(mingw_allflags) src/$*.c

$(mingw_deps):
$(mingw_d_deps):
include $(wildcard $(mingw_deps))
include $(wildcard $(mingw_d_deps))