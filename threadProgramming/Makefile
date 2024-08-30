TARGET: myprogram
myprogram: threadcancelling.o
	g++ -g threadcancelling.o -o myprogram -lpthread -lstdc++
threadcancelling.o: threadcancelling.cpp
	g++ -g -c threadcancelling.cpp -o threadcancelling.o

clean: 
	rm threadcancelling.o
	rm myprogram
