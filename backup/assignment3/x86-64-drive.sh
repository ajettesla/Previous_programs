#! /bin/bash

exefilename=$1
filename=$(echo $1 | cut -d"." -f1)
filenamewithS="$filename.s"

rm $filenamewithS > /dev/null 2>&1


function frontsegment {
    echo ".bss" >> $filenamewithS
    avar=({a..z})
        for ((i = 0; i < 26; i++)); do
        echo -e "\t${avar[i]}:\t.quad 0" >> $filenamewithS
    done
    echo ".data" >> $filenamewithS
    echo "mydata:" >> $filenamewithS
    echo -e "\t.space 100" >> $filenamewithS
    echo "mydatapos:" >> $filenamewithS
    echo -e "\t.space 8" >> $filenamewithS
    echo ".text">> $filenamewithS
    echo -e ".globl _start" >> $filenamewithS
    echo "_start:" >> $filenamewithS
}

function mainsegment {
    ./bin/calc3i.exe < testprogs/$exefilename >> $filenamewithS
}

function endsegment {
    echo -e "\tmovq \$60, %rax" >> $filenamewithS
    echo -e "\tmovq \$0, %rdi" >> $filenamewithS
    echo -e "\tsyscall" >> $filenamewithS
}

function creatingexe {
    gcc -c -fPIC -no-pie src/asmfun.s -o lib/asmfunlib.o
    gcc -c -fPIC -no-pie lib/asmfunlib.o -shared -o lib/asmfunlib.so
    gcc -c $filenamewithS -o lib/"$filename.o"
    gcc -no-pie lib/asmfunlib.so lib/"$filename.o" -o $filename  
}

function cexe {
    as -o lib/asmfun.o src/asmfun.s
    ar rcs lib/asmlib.a lib/asmfun.o
    as -o  lib/"$filename.o" $filenamewithS
    ld -o $filename lib/"$filename.o" -Llib -l:asmlib.a
    rm lib/"$filename.o"
    rm lib/asmfun.o
}





make
frontsegment
mainsegment
endsegment
cexe
#creatingexe