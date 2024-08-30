path = lexyacc-code
path_o = object
path_l = lib
path_s = src

ci : tab
	gcc -g $(path_o)/y.tab.o $(path_o)/lex.yy.o $(path)/calc3i.c -o $(path)/calc3i.exe
tab : lex
	gcc -g -c $(path)/y.tab.c -o $(path_o)/y.tab.o
lex : fun
	gcc -g -c $(path)/lex.yy.c -o $(path_o)/lex.yy.o
fun : 
	gcc -g -c $(path_s)/functions.s -o $(path_l)/functions.o
