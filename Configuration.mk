# Language? C (0) or C++ (1)
cpp = 0

# Common sources
common_sources =\
src/main/buffer.c\
src/main/file/fileWriter.c

common_windows_sources =\
src/main/file/windows/fileMapping.c

common_linux_sources =\
src/main/file/linux/fileMapping.c

# Main sources
main_sources =\
src/main/main.c

main_windows_sources =
main_linux_sources =

# Test sources
test_sources =\
src/test/test.c\
src/test/buffer_test.c

test_windows_sources =
test_linux_sources =

# General flags
gnu_flags = -Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable\(\) -std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie
msvc_flags = /Iinclude /DHEDIT_UNREACHABLE=__assume\(0\) /DHEDIT_WINDOWS /W4 /wd4204 /wd4127 /nologo

# Platform specific flags
gnu_windows_flags = -DHEDIT_WINDOWS
gnu_linux_flags = -DHEDIT_LINUX

# Debug/Release flags
gnu_debug_flags = -g
gnu_release_flags = -O3 -DNDEBUG -s
msvc_debug_flags = /Z7
msvc_release_flags = /O2 /DNDEBUG

# Compiler specifc flags
gcc_flags = -no-pie
mingw_flags = -Wno-pedantic-ms-format
clang_flags = -Wno-unused-command-line-argument

# Link flags
linux_debug_link_flags = 
linux_release_link_flags = 
mingw_windows_debug_link_flags =
mingw_windows_release_link_flags =
clang_windows_debug_link_flags =
clang_windows_release_link_flags =
msvc_debug_link_flags = 
msvc_release_link_flags = 