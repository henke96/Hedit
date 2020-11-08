gnu_flags="-Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable() -std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie"
gcc_debug_flags="${gnu_flags} -no-pie -DHEDIT_LINUX -g"
gcc_release_flags="${gnu_flags} -no-pie -DHEDIT_LINUX -DNDEBUG -O3 -Wl,-s"

gcc ${gcc_release_flags} -o release_gcc $1