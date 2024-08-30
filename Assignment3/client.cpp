#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void gsready(std::string &ip, int port, sockaddr_in *ipv4, sockaddr_in6 *ipv6,int* ipstatus);\

std::vector<std::string> split(std::string sString,std::string delimiter);

fd_set readfds;

int main(int argc, char* argv[]){

 std::string delimiter = ":";
 std::vector<std::string> outputString = split(argv[1],":"); 
 std::string ipString = "";
 int port;
 if(outputString.size() > 2){
  port = atoi(outputString[outputString.size()-1].c_str());
  for(int i=0; i < 8 ; i++){
   ipString = ipString + outputString[i];
  }
}
else{
   port = atoi(outputString[1].c_str());
   ipString = outputString[0];
}
struct sockaddr_in6 *ipv6 = new struct sockaddr_in6;

struct sockaddr_in *ipv4 = new struct sockaddr_in;

int *ipstatus = new int;

gsready(ipString ,port, ipv4, ipv6, ipstatus);


if(*ipstatus == 1){
FD_ZERO(&readfds);
int maxfd;
FD_SET(0,&readfds);
int clientsocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
if(clientsocket < 0){
    std::cout << "error with socket\n";
    exit(0);
}
if(connect(clientsocket, (struct sockaddr*)ipv4, sizeof(sockaddr_in)) < 0){
    std::cout << "error with connect \n";
    exit(0);
}
FD_SET(clientsocket,&readfds);

maxfd = clientsocket;

while(1){
    std::cout << ".";
    int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    std::cout << std::endl;
    if(ready < 0){
        std::cout << "error with select \n";
        exit(0);}
    
    if(FD_ISSET(clientsocket, &readfds)){
        char buffer[300];
        int send_recv_bytes = recv(clientsocket, buffer, sizeof(buffer),0);
           if(send_recv_bytes < 0){
                std::cout << "error with send string \n";
                fflush(stdout);
            }
        buffer[send_recv_bytes - 1] = '\0';
        std::cout << buffer << std::endl;
    }
      if(FD_ISSET(0, &readfds)){
            std::cout << "we are in stdin " << std::flush;
            char sendstring[300];
            int sent_recv_bytes = read(0, sendstring, sizeof(sendstring));
            if(sent_recv_bytes < 0){
                std::cout << "read problem " << std::endl;
                exit(0);
            }
            else if(sent_recv_bytes == 0){
                std::cout << sent_recv_bytes << std::endl;
                fflush(stdout);
                exit(0);
            }
            else if(sent_recv_bytes > 0){
            int send_recv_bytes = send(clientsocket, sendstring, strlen(sendstring),0);
            if(send_recv_bytes < 0){
                std::cout << "error with send string \n";
                fflush(stdout);
                exit(0);
            }}            
        }
    
}



}





    return 0;
}


void gsready(std::string &ip, int port, sockaddr_in *ipv4, sockaddr_in6 *ipv6,int* ipstatus){
struct addrinfo hint, *output, *temp;
memset(&hint, 0, sizeof(hint));
hint.ai_family = AF_UNSPEC;
hint.ai_socktype = SOCK_STREAM;
int status = getaddrinfo(ip.c_str(), NULL, &hint, &output);
if(status != 0){
std::cout << "There is problem in getting getaddrinfo" << std::endl;

}

for(temp=output; temp != NULL;temp->ai_addr){

if(temp->ai_family == AF_INET){
ipv4->sin_family = AF_INET;
ipv4->sin_port = htons(port);
ipv4->sin_addr.s_addr = ((struct sockaddr_in*)temp->ai_addr)->sin_addr.s_addr;
*ipstatus = 1;
break;
                            }
                                              
else if(temp->ai_family == AF_INET6){
ipv6->sin6_family = AF_INET6;
ipv6->sin6_port = htons(port);
ipv6->sin6_addr = ((struct sockaddr_in6*)temp->ai_addr)->sin6_addr;
*ipstatus = 2;
break;

}

}
freeaddrinfo(output);
}


std::vector<std::string> split(std::string sString,std::string delimiter){

std::vector<std::string> nString;
std::string temp;

for(int i=0; i < static_cast<int>(sString.length());i++){
  int  count = 0;
  if(sString[i] == delimiter[0]){
        count++;
        nString.push_back(temp);
        temp  = "";
    }
  else{
        temp = temp +  sString[i];
         }

  if(count==0 && (i == static_cast<int>(sString.length()-1))){
         nString.push_back(temp);}               }



return nString;
}
