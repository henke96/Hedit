# Compile Examples
`./gnu.sh gcc linux release hedit`
`./gnu.sh x86_64-w64-mingw32-gcc windows release hedit`
`./gnu.sh clang linux release hedit`
`./gnu.sh clang windows release hedit "-target x86_64-pc-windows-gnu"`
`./tcc.sh tcc linux release hedit`

`./gnu.sh gcc linux release hedit "-S -masm=intel -fverbose-asm"`
`./gnu.sh clang linux release hedit "-S -masm=intel -fverbose-asm"`