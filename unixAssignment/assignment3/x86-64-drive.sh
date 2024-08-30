#! /bin/bash

exefilename=$1
filename=$(echo $1 | cut -d"." -f1)
filenamewithS="$filename.s"

rm $filenamewithS > /dev/null 2>&1


function frontsegment {
    echo ".bss" >> $filenamewithS
    echo -e "\tdata resb 100" >> $filenamewithS
    echo -e "\tdatapos resb 8" >> $filenamewithS
    avar=({a..z})
        for ((i = 0; i < 26; i++)); do
        echo -e "\t${avar[i]}:\t.quad 0" >> $filenamewithS
    done
    echo ".text">> $filenamewithS
    echo -e "\tglobal _start" >> $filenamewithS
    echo "_start:" >> $filenamewithS
}

function mainsegment {
    ./bin/calc3i.exe < testprogs/$exefilename >> $filenamewithS
}

function endsegment {
    echo -e "\tmov rax, 60" >> $filenamewithS
    echo -e "\tmov rdi, 0" >> $filenamewithS
    echo -e "\tsyscall" >> $filenamewithS
}

function creatingexe {
    gcc -c $filenamewithS -o "$filename.o"
    gcc -no-pie lib/asmfunlib.o "$filename.o" -o $filename
    rm lib/"$filename.o"
    rm lib/asmfunlib.o
}




./lexyacc-code_lab3/build
frontsegment
mainsegment
endsegment
make
creatingexe