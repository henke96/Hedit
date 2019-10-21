# NMake makefile
!CMDSWITCHES +S
!include Configuration.mk

temp_test_sources = $(TEST_SOURCES) $(TEST_WINDOWS_SOURCES)

build\NMakeHelp.mk: Configuration.mk
    buildsystem\NMakeHelper.bat "$(COMMON_SOURCES) $(COMMON_WINDOWS_SOURCES)" "$(MAIN_SOURCES) $(MAIN_WINDOWS_SOURCES)" "$(temp_test_sources)"

debug: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\debug-msvc.exe
release: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\release-msvc.exe
debug-clang: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\debug-clang.exe
release-clang: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\release-clang.exe
test-debug: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\test-debug-msvc.exe
test-release: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\test-release-msvc.exe
test-debug-clang: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\test-debug-clang.exe
test-release-clang: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\test-release-clang.exe

!IF "$(temp_test_sources)"==" "
test_targets =
test_clang_targets =
!ELSE
test_targets = bin\test-debug-msvc.exe bin\test-release-msvc.exe
test_clang_targets = bin\test-debug-clang.exe bin\test-release-clang.exe
!ENDIF

all: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\debug-msvc.exe bin\release-msvc.exe $(test_targets)
all-clang: build\NMakeHelp.mk
    $(MAKE) /F buildsystem\Makefile /NOLOGO bin\debug-clang.exe bin\release-clang.exe $(test_clang_targets)
!CMDSWITCHES -S

clean:
    buildsystem\clean.bat
