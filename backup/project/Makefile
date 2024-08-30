serversrc = mathserver/src/server.c
clientsrc = clients/client.c
serverobj = mathserver/object/server.o
clientobj = mathserver/object/client.o
matrixobj = mathserver/object/matrix_inverse.o
kmeansobj = mathserver/object/kmeans.o
matrixsrc = mathserver/src/matrix_inverse.c
kmeanssrc = mathserver/src/kmeans.c

all: server client

server: $(serverobj) $(matrixobj) $(kmeansobj)
	gcc $(serverobj) $(matrixobj) $(kmeansobj) -o server -lpthread

$(serverobj): $(serversrc)
	gcc -w -O2 -c $(serversrc) -o $(serverobj)

$(matrixobj): $(matrixsrc)
	gcc -w -O2 -c $(matrixsrc) -o $(matrixobj)

$(kmeansobj): $(kmeanssrc)
	gcc -w -O2 -c $(kmeanssrc) -o $(kmeansobj)

client: $(clientobj)
	gcc $(clientobj) -o client

$(clientobj): $(clientsrc)
	gcc -w -O2 -c $(clientsrc) -o $(clientobj)

clean:
	rm -f server
	rm -f client
	rm -f $(serverobj)
	rm -f $(clientobj)
	rm -f $(matrixobj)
	rm -f $(kmeansobj)
