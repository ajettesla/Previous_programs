# This is handled in bash script as well

lexyacc-code_path = ./lexyacc-code
src_path = ./src
lib_path = ./lib
path = ./bin

exe_functionlib:
	gcc -g -c $(lexyacc-code_path)/y.tab.c -o $(lexyacc-code_path)/y.tab.o
	gcc -g -c $(lexyacc-code_path)/lex.yy.c -o $(lexyacc-code_path)/lex.yy.o
	gcc -g $(lexyacc-code_path)/y.tab.o $(lexyacc-code_path)/lex.yy.o $(lexyacc-code_path)/calc3i.c -o $(path)/calc3i.exe
	gcc -g -c $(src_path)/asmlib.s -o $(lib_path)/asmlib.o
