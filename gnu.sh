usage() {
    echo "Usage:"
    echo "./gnu.sh <target> <platform> <mode> [compiler] [cflags]"
    echo ""
    echo "<target>   = Name of source file minus \`.c\` extension."
    echo "<platform> = Target platform, \`linux\`, \`windows\` or \`other\`."
    echo "<mode>     = Build mode, \`debug\`, \`release\` or \`release_small\`."
    echo "[compiler] = Compiler to use. Default is gcc."
    echo "[cflags]   = Extra flags to pass to the compiler."
}

if test $# -lt 3; then
    usage
    exit 1
fi

flags="-Iinclude -DHEDIT_UNREACHABLE=__builtin_unreachable() -std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -fno-pie -no-pie $5"
if test $2 = "linux"; then
    flags="$flags -DHEDIT_PLATFORM_LINUX"
    output_extension=""
elif test $2 = "windows"; then
    flags="$flags -DHEDIT_PLATFORM_WINDOWS"
    output_extension=".exe"
elif test $2 = "other"; then
    flags="$flags -DHEDIT_PLATFORM_OTHER"
    output_extension=".bin"
else
    usage
    exit 1
fi

if test $3 = "debug"; then
    flags="$flags -g"
elif test $3 = "release"; then
    flags="$flags -DNDEBUG -O3 -Wl,-s"
elif test $3 = "release_small"; then
    flags="$flags -DNDEBUG -Os -Wl,-s"
else
    usage
    exit 1
fi
compiler=${4-gcc}

"$compiler" $flags -o "$1_$3_$(basename $compiler)$output_extension" "$1.c"