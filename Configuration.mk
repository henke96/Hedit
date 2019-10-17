# Language? C (0) or C++ (1)
CPP = 0

# Common sources
COMMON_SOURCES =\
src/main/buffer.c\
src/main/file/fileWriter.c

COMMON_WINDOWS_SOURCES =\
src/main/file/windows/fileMapping.c

COMMON_LINUX_SOURCES =\
src/main/file/linux/fileMapping.c

# Main sources
MAIN_SOURCES =\
src/main/main.c

MAIN_WINDOWS_SOURCES =
MAIN_LINUX_SOURCES =

# Test sources
TEST_SOURCES =\
src/test/test.c\
src/test/buffer_test.c

TEST_WINDOWS_SOURCES =
TEST_LINUX_SOURCES =

gnu_flags = -Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable\(\) -DHEDIT_LINUX -std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie -no-pie

# Linux compile flags
GCC_DEBUG_FLAGS = $(gnu_flags) -DHEDIT_LINUX -g
GCC_RELEASE_FLAGS = $(gnu_flags) -DHEDIT_LINUX -O3

CLANG_DEBUG_FLAGS = $(gnu_flags) -DHEDIT_LINUX -g
CLANG_RELEASE_FLAGS = $(gnu_flags) -DHEDIT_LINUX -O3

# Windows compile flags
MINGW_DEBUG_FLAGS = $(gnu_flags) -DHEDIT_WINDOWS -g
MINGW_RELEASE_FLAGS = $(gnu_flags) -DHEDIT_WINDOWS -O3

WIN_GNU_CLANG_DEBUG_FLAGS = $(gnu_flags) -DHEDIT_WINDOWS -g
WIN_GNU_CLANG_RELEASE_FLAGS = $(gnu_flags) -DHEDIT_WINDOWS -O3

WIN_CLANG_DEBUG_FLAGS = $(gnu_flags) -DHEDIT_WINDOWS -g
WIN_CLANG_RELEASE_FLAGS = $(gnu_flags) -DHEDIT_WINDOWS -O3

MSVC_DEBUG_FLAGS = /Iinclude /DHEDIT_UNREACHABLE=__assume\(0\) /DHEDIT_WINDOWS /std:c++latest /W4 /nologo /Z7
MSVC_RELEASE_FLAGS = /Iinclude /DHEDIT_UNREACHABLE=__assume\(0\) /DHEDIT_WINDOWS /std:c++latest /W4 /nologo /O2

# Linux link flags
GCC_DEBUG_LINK_FLAGS =
GCC_RELEASE_LINK_FLAGS = -Wl,-s

CLANG_DEBUG_LINK_FLAGS =
CLANG_RELEASE_LINK_FLAGS = -Wl,-s

# Windows link flags
MINGW_DEBUG_LINK_FLAGS =
MINGW_RELEASE_LINK_FLAGS = -Wl,-s

WIN_GNU_CLANG_DEBUG_LINK_FLAGS =
WIN_GNU_CLANG_RELEASE_LINK_FLAGS = -Wl,-s

WIN_CLANG_DEBUG_LINK_FLAGS =
WIN_CLANG_RELEASE_LINK_FLAGS =

MSVC_DEBUG_LINK_FLAGS =
MSVC_RELEASE_LINK_FLAGS =