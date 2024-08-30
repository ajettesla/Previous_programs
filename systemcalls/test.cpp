#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <signal.h>


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

#define MAXCLIENTCOUNT 50 

int clientFD[MAXCLIENTCOUNT];

fd_set readfds;

void clearfd(){
    FD_ZERO(&readfds);
    for(int i=0; i< MAXCLIENTCOUNT;i++){
        clientFD[i] = -1;
    }
}

int getmax(){
    int max = 0;
    for(int i=0; i < MAXCLIENTCOUNT; i++){
        if(clientFD[i] > max){
            max = clientFD[i];
        }
    }
    return max;
}

void re_inti_readfd(fd_set *readfds){
    FD_ZERO(readfds);
    for(int i=0; i < MAXCLIENTCOUNT; i++){
        if(clientFD[i] != -1){
        FD_SET(clientFD[i], readfds);}
    }
}

void setclientfd(int socket){
    for(int i=0; i < MAXCLIENTCOUNT; i++){
        if(clientFD[i] == -1){
            clientFD[i] = socket;
            break;
        }
        
    }
}

void removeclientfd(int socket){
    for(int i =0; i < MAXCLIENTCOUNT; i++){
        if(clientFD[i] == socket){
            clientFD[i] = -1;
        }
    }
}




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

clearfd();

if(*ipstatus == 1){
      int mastersocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
       setclientfd(mastersocket);
      if(bind(mastersocket, (struct sockaddr*)ipv4, sizeof(struct sockaddr_in))< 0){
        std::cout << "error with bind \n";
        exit(0);
      }
      if(listen(mastersocket, 5) < 0){
        std::cout << "error with listen " << std::endl;
        exit(0);
      }

      struct timeval timeout;
      timeout.tv_sec = 5;
      timeout.tv_usec =0;
      setclientfd(STDIN_FILENO);
while(1){
    re_inti_readfd(&readfds);
    std::cout << "BLOCKED ON SELECT SYSTEM CALL" << std::endl;
    int rc = select(getmax() + 1, &readfds, NULL, NULL, NULL);
    if(rc < 0){
        std::cout << "error with " << std::endl;
        exit(0);}

          if(FD_ISSET(mastersocket, &readfds)){
            struct sockaddr_in clientaddr;
            socklen_t addlen = sizeof(clientaddr);
            char ip_str[INET_ADDRSTRLEN];
            int commsocket = accept(mastersocket,(struct sockaddr*)&clientaddr,&addlen);
            if(commsocket < 0){
                std::cout << "fail to create the socket " << std::endl;
                close(commsocket);
                exit(0);
            }
            if(inet_ntop(AF_INET,&(clientaddr.sin_addr.s_addr), ip_str, INET_ADDRSTRLEN) == NULL){
                std::cout << "error with conversiton\n";
                exit(0);
            }
           std::cout << ip_str << " is connected with port "  << ntohs(clientaddr.sin_port) << std::endl;
           if(setsockopt(commsocket, SOL_SOCKET, SO_RCVTIMEO,&timeout, sizeof(timeout)) < 0){
            std::cout << "error with set socket " << std::endl;
            close(commsocket);
           }
           else{
            setclientfd(commsocket);}
          }
          else if(FD_ISSET(STDIN_FILENO, &readfds)){
             char input[256];
                if (fgets(input, sizeof(input), stdin) != nullptr) {
                  std::cout << "string is " <<  input << std::endl; 
                }
            
          }
          else{

       for(int fd =0; fd < MAXCLIENTCOUNT; fd++){
            if(FD_ISSET(clientFD[fd], &readfds)){
             int commfd = clientFD[fd];
                while(1){
            char buffer[100];
            memset(buffer,0,sizeof(buffer));
            std::cout << "BLOCKED AT RECV FUNCTION" << std::endl;
            int sent_recv_bytes = recv(commfd, buffer, sizeof(buffer),0);
            if(sent_recv_bytes < 0){
                if(errno == EAGAIN){
                    removeclientfd(commfd);
                    break;
                }
                std::cout << "error with sent" << std::endl;
                exit(0);}
            std::cout << buffer << std::endl;
            std::string bu;
            std::getline(std::cin, bu);
            sent_recv_bytes = send(commfd, bu.c_str(), bu.length(),0);
            if(sent_recv_bytes < 0){
                std::cout << "error with send" << std::endl;
                exit(0);}
                std::cout << ".";
                }
             close(commfd);
            removeclientfd(commfd);}
        }
}
    
}

}

    return 0;
}


