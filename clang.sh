gnu_flags="-Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable() -std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie"
clang_debug_flags="${gnu_flags} -no-pie -DHEDIT_LINUX -g"
clang_release_flags="${gnu_flags} -no-pie -DHEDIT_LINUX -DNDEBUG -O3 -Wl,-s"

clang ${clang_release_flags} -o release_clang $1