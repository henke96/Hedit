# NMake makefile
!include BuildConfiguration.mk
windows_main_sources = $(sources) $(windows_sources) $(main_sources)
windows_test_sources = $(sources) $(windows_sources) $(test_sources)

debug: bin\debug-msvc.exe
release: bin\release-msvc.exe
debug-clang: bin\debug-clang.exe
release-clang: bin\release-clang.exe

test: bin\test-msvc.exe
test-clang: bin\test-clang.exe

all: debug release debug-clang release-clang test test-clang

clean:
	del bin\* /Q

bin\debug-msvc.exe: $(headers) $(windows_main_sources)
	cl /Fe$@ /Fobin\ /DHEDIT_WINDOWS $(msvc_flags) $(msvc_debug_flags) $(windows_main_sources)

bin\release-msvc.exe: $(headers) $(windows_main_sources)
	cl /Fe$@ /Fobin\ /DHEDIT_WINDOWS $(msvc_flags) $(msvc_release_flags) $(windows_main_sources)

bin\debug-clang.exe: $(headers) $(windows_main_sources)
	clang -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_debug_flags) $(windows_main_sources)

bin\release-clang.exe: $(headers) $(windows_main_sources)
	clang -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_release_flags) $(windows_main_sources)

bin\test-msvc.exe: $(headers) $(windows_test_sources)
	cl /Fe$@ /Fobin\ /DHEDIT_WINDOWS $(msvc_flags) $(msvc_test_flags) $(windows_test_sources)

bin\test-clang.exe: $(headers) $(windows_test_sources)
	clang -o $@ -DHEDIT_WINDOWS $(gnu_flags) $(gnu_test_flags) $(windows_test_sources)