filename="fact"
filenamewithS="$filename.s"

as -o lib/asmfun.o src/asmfun.s
ar rcs lib/asmlib.a lib/asmfun.o
as -o  lib/"$filename.o" $filenamewithS
ld -o $filename lib/"$filename.o" -Llib -l:asmlib.a
rm lib/"$filename.o"
rm lib/asmfun.o
