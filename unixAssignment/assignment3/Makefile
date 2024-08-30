target: lib/asmfunlib.so
	
lib/asmfunlib.so: lib/asmfunlib.o
	gcc -c -fPIC -no-pie lib/asmfunlib.o -shared -o lib/asmfunlib.so
lib/asmfunlib.o: src/asmfun.s
	gcc -c -fPIC -no-pie src/asmfunlib.s -o lib/asmfunlib.o
