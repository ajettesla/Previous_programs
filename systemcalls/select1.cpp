#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <errno.h>


fd_set readfd;

int main(){

struct sockaddr_in server;

server.sin_addr.s_addr = INADDR_ANY;
server.sin_family = AF_INET;
server.sin_port = htons(5000);

int mastersocketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

FD_ZERO(&readfd);
FD_SET(mastersocketfd, &readfd);

select(mastersocketfd + 1, &readfd, NULL, NULL, NULL);








    return 0;
}