common_sources =\
src/buffer.c\
src/file/fileWriter.c

windows_sources =\
src/file/windows/fileMapping.c

sources =\
src/main.c\
$(common_sources)\
$(windows_sources)

gnu_flags = -Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable\(\) -std=c99 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie
gnu_debug_flags = -g
gnu_release_flags = -O3 -DNDEBUG
gnu_windows_flags = -DHEDIT_WINDOWS
gcc_flags = -no-pie
mingw_flags = -Wno-pedantic-ms-format

msvc_flags = /Iinclude /DHEDIT_UNREACHABLE=__assume\(0\) /DHEDIT_WINDOWS /W4 /wd4204 /wd4127 /nologo
msvc_debug_flags = /Z7
msvc_release_flags = /O2