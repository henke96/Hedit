# GNU makefile
include BuildConfiguration.mk
windows_main_sources = $(sources) $(windows_sources) $(main_sources)
windows_test_sources = $(sources) $(windows_sources) $(test_sources)
linux_main_sources = $(sources) $(linux_sources) $(main_sources)
linux_test_sources = $(sources) $(linux_sources) $(test_sources)

debug: bin/debug-gcc
release: bin/release-gcc
debug-clang: bin/debug-clang
release-clang: bin/release-clang
win-debug: bin/debug-mingw.exe
win-release: bin/release-mingw.exe
win-debug-clang: bin/debug-clang.exe
win-release-clang: bin/release-clang.exe

test: bin/test-gcc
test-clang: bin/test-clang
win-test: bin/test-mingw.exe
win-test-clang: bin/test-clang.exe

all: debug release debug-clang release-clang win-debug win-release test test-clang win-test
win-all: win-debug win-release win-debug-clang win-release-clang win-test win-test-clang

clean:
	rm -rf bin/*

bin/debug-gcc: $(headers) $(linux_main_sources)
	gcc -o $@ -DHEDIT_LINUX $(gnu_flags) $(gnu_debug_flags) $(gcc_flags) $(linux_main_sources)

bin/release-gcc: $(headers) $(linux_main_sources)
	gcc -o $@ -DHEDIT_LINUX $(gnu_flags) $(gnu_release_flags) $(gcc_flags) $(gcc_release_flags) $(linux_main_sources)

bin/debug-clang: $(headers) $(linux_main_sources)
	clang -o $@ -DHEDIT_LINUX $(gnu_flags) $(gnu_debug_flags) $(linux_main_sources)

bin/release-clang: $(headers) $(linux_main_sources)
	clang -o $@ -DHEDIT_LINUX $(gnu_flags) $(gnu_release_flags) $(linux_main_sources)

bin/debug-mingw.exe: $(headers) $(windows_main_sources)
	x86_64-w64-mingw32-gcc -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_debug_flags) $(gcc_flags) $(mingw_flags) $(windows_main_sources)

bin/release-mingw.exe: $(headers) $(windows_main_sources)
	x86_64-w64-mingw32-gcc -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_release_flags) $(gcc_flags) $(gcc_release_flags) $(mingw_flags) $(windows_main_sources)

bin/debug-clang.exe: $(headers) $(windows_main_sources)
	clang -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_debug_flags) $(windows_main_sources)

bin/release-clang.exe: $(headers) $(windows_main_sources)
	clang -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_release_flags) $(windows_main_sources)

bin/test-gcc: $(headers) $(test_headers) $(linux_test_sources)
	gcc -o $@ -DHEDIT_LINUX $(gnu_flags) $(gnu_test_flags) $(gcc_flags) $(linux_test_sources)

bin/test-clang: $(headers) $(test_headers) $(linux_test_sources)
	clang -o $@ -DHEDIT_LINUX $(gnu_flags) $(gnu_test_flags) $(linux_test_sources)

bin/test-mingw.exe: $(headers) $(test_headers) $(windows_test_sources)
	x86_64-w64-mingw32-gcc -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_test_flags) $(gcc_flags) $(mingw_flags) $(windows_test_sources)

bin/test-clang.exe: $(headers) $(test_headers) $(windows_test_sources)
	clang -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_test_flags) $(windows_test_sources)