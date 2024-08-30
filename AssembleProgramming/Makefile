target: output

output.o: hello.s
	as  hello.s -o output.o
output: output.o
	ld output.o -o output
	
clean:
	rm output.o
	rm output
