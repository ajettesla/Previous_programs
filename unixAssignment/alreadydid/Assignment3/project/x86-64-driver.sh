#! /bin/bash

function build_calc_file {
    bison -y -d ./lexyacc-code/calc3.y -o ./lexyacc-code/y.tab.c
    flex -o ./lexyacc-code/lex.yy.c ./lexyacc-code/calc3.l
    gcc  -o ./lexyacc-code/y.tab.o -c ./lexyacc-code/y.tab.c
    gcc  -o ./lexyacc-code/lex.yy.o -c ./lexyacc-code/lex.yy.c
    gcc ./lexyacc-code/y.tab.o ./lexyacc-code/lex.yy.o ./lexyacc-code/calc3i.c -o ./bin/calc3i
}


function create_executable {
    gcc -c -fPIC -no-pie ./src/asmlib.s -o ./lib/asmlib.o
    gcc -c -fPIC -no-pie ./lib/asmlib.o -shared -o ./lib/asmlib.so
    gcc -c ./build/$1.s -o ./lib/$1.o
    gcc -no-pie ./lib/asmlib.o ./lib/$1.o -o ./bin/$1
    rm ./lib/$1.o
    rm ./lib/asmlib.o
}


function data_segement {
    echo -e ".data" | cat >> $1
    echo $'\t''format: '$'\t' '.ascii' $'\t' '"%d\n"' | cat >> $1

    echo -e ".bss" | cat >> $1

    chars=( {a..z} )
    n=26

    for ((i=0; i<n; i++ ))
    do
        echo -e "\t ${chars[i]}: \t .quad\t0"  | cat >> $1
    done
}

function header_segement {
    echo -e ".text" | cat >> $1
    echo -e ".globl main \n" | cat >> $1
    echo -e "main:" | cat >> $1
}

function exit_segment {
    echo -e "\tmovq\t\$1,%rdi" | cat >> $1
    echo -e "\tcall exit" | cat >> $1
}

function basic_compiler {
    # $1: full filename with path
    # $2: filename with extension

    # extract filename without an extension

    local filename=$(echo "${2%%.calc*}")

    # create a file with .s as extension
    local filename_with_s="$filename.s"

    # build an excutable for the calc3i.c file compiler
    build_calc_file

    echo "" | cat > "./build/$filename_with_s"
    data_segement "./build/$filename_with_s"
    header_segement "./build/$filename_with_s"

    ./bin/calc3i < "$1" | cat >> "./build/$filename_with_s"

    # exit code
    exit_segment "./build/$filename_with_s"
    create_executable "$filename"
}



# The workflow starts from here!
echo ""
echo "Compiler for assembler code, accepts a file with '.calc' as extension to produce a
assembler code and its executable program!"
echo ""
echo ""

# Load file into the script (The path can contain spaces "Test Script" in dir names)
input_file_path="$@"

# Validate if the path exists
if [[ ! -f "$input_file_path" ]]; then
    echo " ERROR: $input_file_path does not exist in provided path."
    exit 1
fi

# get filename along with extension
filename_with_extension=$(echo "$input_file_path" | grep -o '[^\/]*$')

# validate if inputed filename has '.calc' as extension
if [ "${filename_with_extension: -5}" != ".calc" ]
then
    echo " ERROR: Please provide a filename with .calc as suffix"
    exit 1
fi


# generate a x86-64 assembly code
basic_compiler "$input_file_path" "$filename_with_extension"

# succesfully completed
exit 0
