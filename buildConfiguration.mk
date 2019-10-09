headers =\
include/*.h\
include/file/*.h

sources =\
src/buffer.c\
src/file/fileWriter.c

linux_sources =\
src/file/linux/fileMapping.c

windows_sources =\
src/file/windows/fileMapping.c

main_sources =\
src/main.c

test_sources = \
test/src/test.c\
test/src/test_buffer.c

gnu_flags = -Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable\(\) -std=c99 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie
gnu_debug_flags = -g
gnu_release_flags = -O3 -s -DNDEBUG
gnu_test_flags = -Itest/include
gcc_flags = -no-pie -Wno-pedantic-ms-format
clang_flags =

msvc_flags = /Iinclude /DHEDIT_UNREACHABLE=__assume\(0\)
msvc_debug_flags = 
msvc_release_flags = 
msvc_test_flags = /Itest/include