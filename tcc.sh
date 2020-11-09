usage() {
    echo "Usage:"
    echo "./gnu.sh <compiler> <platform> <mode> <target> [cflags]"
    echo ""
    echo "<compiler> = Compiler to use. Needs to be compatible with tcc."
    echo "<platform> = Target platform, \`linux\`, \`windows\` or \`other\`."
    echo "<mode>     = Build mode, \`debug\` or \`release\`."
    echo "<target>   = Name of source file minus \`.c\` extension."
    echo "[cflags]   = Extra flags to pass to the compiler."
}

if test $# -lt 4; then
    usage
    exit 1
fi

flags="-Iinclude -DHEDIT_UNREACHABLE= -Wall -Wwrite-strings $5"
output_extension=""
if test $2 = "linux"; then
    flags="${flags} -DHEDIT_PLATFORM_LINUX"
elif test $2 = "windows"; then
    flags="${flags} -DHEDIT_PLATFORM_WINDOWS"
    output_extension=".exe"
elif test $2 = "other"; then
    flags="${flags} -DHEDIT_PLATFORM_OTHER"
    output_extension=".bin"
else
    usage
    exit 1
fi

if test $3 = "debug"; then
    flags="${flags} -g"
elif test $3 = "release"; then
    flags="${flags} -DNDEBUG"
else
    usage
    exit 1
fi

"$1" ${flags} -o "$4_$1_$3$output_extension" "$4.c"