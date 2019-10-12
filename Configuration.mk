common_sources =\
src/main.c\
src/buffer.c\
src/file/fileWriter.c

windows_sources =\
src/file/windows/fileMapping.c

linux_sources =\
src/file/linux/fileMapping.c

gnu_flags = -Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable\(\) -std=c99 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie
gnu_debug_flags = -g
gnu_release_flags = -O3 -DNDEBUG -s
gnu_windows_flags = -DHEDIT_WINDOWS
gnu_linux_flags = -DHEDIT_LINUX
gcc_flags = -no-pie
mingw_flags = -Wno-pedantic-ms-format
clang_flags = -Wno-unused-command-line-argument

msvc_flags = /Iinclude /DHEDIT_UNREACHABLE=__assume\(0\) /DHEDIT_WINDOWS /W4 /wd4204 /wd4127 /nologo
msvc_debug_flags = /Z7
msvc_release_flags = /O2