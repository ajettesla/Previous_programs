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
#include <pthread.h>
#include <unistd.h>

std::string recvBuffer(int *sock);

void* sendClient(void *arg);

void* recvClient(void *arg);

void* tranfercall(void* arg);

void sendBuffer(int *socketfd,const char* mess);

pthread_mutex_t vect;

typedef struct clientDataStruct{
    int csockefd;
    int clientid;
    std::string nickName;
    std::string recvData;
    std::string sendData;
    sockaddr_in clientip;
}clientDataStruct;

void* runClient(void *arg);

void* clientThread(void *arg);

std::vector<std::string> split(std::string sString,std::string delimiter);

void gsready(std::string &ip, int port, struct sockaddr_in *ipv4,struct sockaddr_in6 *ipv6, int* ipstatus);

void printclientip4(struct sockaddr_in clientAddr);

void create_client(int client);

std::vector<clientDataStruct*> clientDataVector;

fd_set readfds;

int clientID = 0;

int main(int argc, char *argv[]){
  pthread_mutex_init(&vect, 0);
 std::string delimiter = ":";
 std::vector<std::string> outputString = split(argv[1],":"); 
 int mastersocketfd;
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
  
  if(bind(mastersocketfd, (struct sockaddr*)ipv4, sizeof(struct sockaddr_in)) < 0){
      std::cout << "Error while bind with ip and port " << std::endl;
      exit(1);
      }

    if(listen(mastersocketfd, 5) < 0){
      std::cout << "error with listen function" << std::endl;
      return 1;
    }

  socklen_t addrlen = sizeof(struct sockaddr);

  if(listen(mastersocketfd, 5) < 0){
      std::cout <<" listen failed " << std::endl;      
      exit(1);}
 
  
  while(1){
    FD_ZERO(&readfds);
    FD_SET(mastersocketfd, &readfds);
    std::cout << "Blocked at SELECT SYSTEM CALL" << std::endl;
    select(mastersocketfd + 1, &readfds, NULL, NULL, NULL);
    if(FD_ISSET(mastersocketfd, &readfds)){
      int csocketfd = accept(mastersocketfd, (struct sockaddr*)&clientAddr, &addrlen);
      if(csocketfd < 0){std::cout << "Accept ERROR" << std::endl;}

      printclientip4(clientAddr);
      std::cout << " is connected " << std::endl;

      struct timeval timeout;
      timeout.tv_sec = 100;
      timeout.tv_usec = 0;
      setsockopt(csocketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

      clientDataStruct *client = new clientDataStruct;
      client->clientid = clientID;
      client->clientip = clientAddr;
      client->csockefd = csocketfd;
      clientDataVector.push_back(client);
      create_client(client->clientid);

      clientID++;
    }//FD_SET IFLOOP

  }//whileloopout

}//ipv4 out

  for(size_t i =0 ; i< clientDataVector.size();i++){
        if(clientDataVector[i] != NULL){
       delete (clientDataVector[i]);
       clientDataVector[i] = NULL;}
    }
    delete ipv4;
    delete ipv6; 
    delete ipstatus;
    return 0;
}


void *clientFunction(void *arg){
    int clientid = *(int*)arg;
    int vectorid;
    int sent_recv_bytes;
    while(1){
      //checking vector id;
        pthread_mutex_lock(&vect);
        if(clientDataVector[clientid]->clientid != clientid){
         for(int i=0; i<clientDataVector.size(); i++){
         if(clientDataVector[clientid] != NULL){
           vectorid = i;
            break;}}}
         else{
          vectorid = clientid;
             }
          int socket = clientDataVector[vectorid]->csockefd;
         pthread_mutex_unlock(&vect);
       //end of checking vector id;


      std::string buffer;
      pthread_mutex_lock(&vect);
      if(clientDataVector[vectorid]->sendData != ""){
        buffer = clientDataVector[vectorid]->sendData;
        clientDataVector[vectorid]->sendData = "";}
      pthread_mutex_unlock(&vect);
      if(buffer != ""){
        std::cout <<".";
        sleep(1);
      sent_recv_bytes = send(socket,buffer.c_str(),buffer.length(),0);
      if(sent_recv_bytes < 0){
        perror("error with send\n");
        close(socket);
        break;
      }}

      char buf[300];
      sent_recv_bytes = recv(socket,buf,sizeof(buf), 0);
      if(sent_recv_bytes < 0){
        perror("error with recv\n");
      }
      buffer[sent_recv_bytes-1] = '\0';
      std::string b(buf);
      pthread_mutex_lock(&vect);
      if(b == "TERMINATE\n"){
        close(socket);
         printclientip4(clientDataVector[vectorid]->clientip);
        std::cout << " is disconnecting " << std::endl;
        if (vectorid >= 0 && vectorid < clientDataVector.size()) {
        clientDataVector.erase(clientDataVector.begin() + vectorid);}
        break;}
      else{
        for(int j=0; j<clientDataVector.size(); j++){
          if(clientDataVector[j]->sendData == ""){
            if(clientDataVector[j]->clientid != clientDataVector[vectorid]->clientid){
              clientDataVector[j]->sendData = b;
            }
          }         
        }
      }
      pthread_mutex_unlock(&vect);
      sleep(1);
    }
     pthread_exit(NULL);

  return NULL;
}  

void create_client(int client){
  pthread_t handel;
  int *clientid = new int;
  *clientid = client;
  pthread_create(&handel, NULL, clientFunction, (void*)clientid);
}



void printclientip4(struct sockaddr_in clientAddr){
       char ipv4buffer[INET_ADDRSTRLEN];
      const char *result = inet_ntop(AF_INET,&(clientAddr.sin_addr),ipv4buffer,sizeof(ipv4buffer));

      if(result !=NULL){
        std::cout<<"ip " << ipv4buffer << " port " << ntohs(clientAddr.sin_port);}
      else{
        perror("inet_ntop");}
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
