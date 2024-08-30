#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <stdlib.h>


std::string recvBuffer(int *sock);

typedef struct datatoclient{
    int csockefd;
    std::string recvData;
    std::string sendData;
}datatoclient;

void* runClient(void *arg);

std::vector<std::string> split(std::string sString,std::string delimiter);

void gsready(std::string &ip, int port, struct sockaddr_in *ipv4,struct sockaddr_in6 *ipv6, int* ipstatus);


std::vector<datatoclient*> totalData;


void* clientComm(void *arg);

void sendBuffer(int *socketfd,const char* mess);

int main(int argc, char *argv[]){

fd_set readfds;

std::string delimiter = ":";

std::vector<std::string> outputString = split(argv[1],":"); 

std::string nickName = argv[2];

int csocketfd, mastersocketfd;

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

    struct sockaddr_in clientAddr;
     
     mastersocketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

     if( mastersocketfd < 0){
        std::cerr << "Error in socket creation" << std::endl;
        return 1;}

     if(bind(mastersocketfd, (struct sockaddr*)ipv4, sizeof(struct sockaddr)) < 0){
      std::cout << "Error while bind with ip and port " << std::endl;
      exit(1);
      }
     
      socklen_t addrlen = sizeof(struct sockaddr);
      
      if(listen(mastersocketfd, 5) < 0){
      std::cout <<" listen failed " << std::endl;      
      exit(1);}

      while(1){

      FD_ZERO(&readfds);
      FD_SET(mastersocketfd, &readfds);
      select(mastersocketfd + 1, &readfds, NULL, NULL, NULL);

      if(FD_ISSET(mastersocketfd, &readfds)){
       csocketfd = accept(mastersocketfd, (struct sockaddr*)&clientAddr, &addrlen);}
     
       if(csocketfd < 0){
      std::cout << "Accept ERROR" << std::endl;}
      
      struct timeval timeout;
      timeout.tv_sec = 10;
      timeout.tv_usec = 0;
      
      setsockopt(csocketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

      pthread_t handle;

      datatoclient *clientData = new datatoclient;
      clientData->csockefd = csocketfd;
      clientData->recvData = "";  
      clientData->sendData = "";
      totalData.push_back(clientData); 

      int rc = pthread_create(&handle, NULL, runClient,(void*)clientData); 

      }
     

}
    for(size_t i =0 ; i< totalData.size();i++){
       delete totalData[i];
    }
    delete ipv4, ipv6; ipstatus ;
    return 0; 
}


void* runClient(void *arg){
      std::string nickName;
      datatoclient *cData = (datatoclient*)arg;
      int socket = cData->csockefd;
      char buffer[300];
      memset(buffer, 0, sizeof(buffer));
      char intialMessage[] = "TEXT TCP 1.0\n";
      while(1){
      int sent_recv_bytes = send(socket, intialMessage, sizeof(intialMessage),0);
      sent_recv_bytes = recv(socket, buffer, sizeof(buffer),0);
      std::string a(buffer);
      std::cout << a << std::endl;
      }
    close(socket);
    delete cData;
    pthread_exit(NULL);
    return nullptr;
}


void sendBuffer(int *socketfd,const char* mess){

int sent_recv_bytes = send(*socketfd, mess, strlen(mess),0);

if(sent_recv_bytes == -1){
std::cout << "There is error while sending " << std::endl;
perror("send");}

}

std::string recvBuffer(int *sock){
    int buffersize = 300;
    char recvBuffer[buffersize];
    int sent_recv_bytes = recv(*sock, recvBuffer, buffersize, 0);
    if(sent_recv_bytes == -1){
      std::cout << "There is error while receving data" << std::endl;
      std::exit(1);}
    recvBuffer[sent_recv_bytes] = '\0';
    std::string recvBuf(recvBuffer);
    return recvBuf;}


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
