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

void createclient();

void sendBuffer(int *socketfd,const char* mess);

pthread_mutex_t vect;

typedef struct clientDataStruct{
    int csockefd;
    std::string nickName;
    std::string recvData;
    std::string sendData;
    int vectorNumber;
    pthread_t recvhandel;
    pthread_t sendhandel;
}clientDataStruct;

void* runClient(void *arg);

void* clientThread(void *arg);

int maxfd;

std::vector<std::string> split(std::string sString,std::string delimiter);

void gsready(std::string &ip, int port, struct sockaddr_in *ipv4,struct sockaddr_in6 *ipv6, int* ipstatus);

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
  
  if(bind(mastersocketfd, (struct sockaddr*)ipv4, sizeof(struct sockaddr)) < 0){
      std::cout << "Error while bind with ip and port " << std::endl;
      exit(1);
      }
  socklen_t addrlen = sizeof(struct sockaddr);

  if(listen(mastersocketfd, 5) < 0){
      std::cout <<" listen failed " << std::endl;      
      exit(1);}

    FD_ZERO(&readfds);
    FD_SET(mastersocketfd, &readfds);
 
  while(1){

    select(mastersocketfd + 1, &readfds, NULL, NULL, NULL);

    if(FD_ISSET(mastersocketfd, &readfds)){
      int csocketfd = accept(mastersocketfd, (struct sockaddr*)&clientAddr, &addrlen);
     
      if(csocketfd < 0){std::cout << "Accept ERROR" << std::endl;}
      
      char ipv4buffer[INET_ADDRSTRLEN];
      const char *result = inet_ntop(AF_INET,&(clientAddr.sin_addr),ipv4buffer,sizeof(ipv4buffer));

      if(result !=NULL){
        std::cout<< "client connected with " << ipv4buffer << " port " << ntohs(clientAddr.sin_port) << std::endl;}
      else{
        perror("inet_ntop");}

      struct timeval timeout;
      timeout.tv_sec = 100;
      timeout.tv_usec = 0;
      
      setsockopt(csocketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

      clientDataStruct *cD = new clientDataStruct;
      cD->csockefd = csocketfd;
      cD->vectorNumber = clientID;    

      int *cid = new int;
      *cid = clientID;
      pthread_t handel;
      pthread_t handel2;
      cD->recvData = handel;
      cD->sendhandel = handel2;
      clientDataVector.push_back(cD); 

      pthread_create(&handel, NULL, recvClient, (void*)cid);
      pthread_create(&handel2, NULL, sendClient, (void*)cid);
     
      clientID++;
 
    }//END OF if loop

    


 

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

 

void* recvClient(void *arg){
    int clientid = *(int*)arg;

   pthread_mutex_lock(&vect);
   int socket = clientDataVector[clientid]->csockefd;
   std::string nickName = clientDataVector[clientid]->nickName;
   pthread_mutex_unlock(&vect);
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,0);
   pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,0);

      char intialMess[] = "TEXT TCP 1.0\n";
      int sent_recv_bytes = send(socket, intialMess, strlen(intialMess),0);
      if(sent_recv_bytes < 0){
      std::cout << "error while sending" << std::endl;
      close(socket);
       pthread_cancel(clientDataVector[clientid]->recvhandel);
       pthread_cancel(clientDataVector[clientid]->sendhandel);}

      char nbuffer[50];
      memset(nbuffer, 0, sizeof(nbuffer));
      sent_recv_bytes = recv(socket, nbuffer, sizeof(nbuffer),0);
      if(sent_recv_bytes < 0){
        std::cout << "error while recving data";
        close(socket);
        pthread_cancel(clientDataVector[clientid]->recvhandel);
        pthread_cancel(clientDataVector[clientid]->sendhandel);}


      std::string nick(nbuffer);
      nick = nick.substr(0,sent_recv_bytes -1);
      clientDataVector[clientid]->nickName = nick;

   char buffer[300];

   while(1){
   memset(buffer,0,sizeof(buffer));
   int sent_recv_bytes = recv(socket, buffer, sizeof(buffer),0);
   if(sent_recv_bytes == 0){close(socket);
   delete clientDataVector[clientid];
   clientDataVector[clientid] = NULL;
   break;}
   else if(sent_recv_bytes < 0){
    std::cout << "error while receving data "<< std::endl;
   }
   buffer[sent_recv_bytes] = '\0';
   std::string buf(buffer);

        if(buf== "TERMINATE\n"){
          close(socket);
          pthread_cancel(clientDataVector[clientid]->recvhandel);
          pthread_cancel(clientDataVector[clientid]->sendhandel);
          
        }
        else{
   
   buf = nick + " : " + buf;
   if(clientDataVector.size()>1){

   pthread_mutex_lock(&vect);
   for(size_t i =0; i<clientDataVector.size(); i++){
    if(i != clientid){
      if(clientDataVector[i] != NULL){
        clientDataVector[i]->sendData = buf;
      }
    }
   }
   pthread_mutex_unlock(&vect);}}
   }

  if(clientDataVector[clientid] != NULL){
   delete clientDataVector[clientid];
   FD_CLR(socket, &readfds);
   clientDataVector[clientid] = NULL;}

    pthread_cancel(clientDataVector[clientid]->recvhandel);
    pthread_cancel(clientDataVector[clientid]->sendhandel);
  return nullptr;  
}


void* sendClient(void *arg){
  int clientid = *(int*)arg;
  pthread_mutex_lock(&vect);
  int socket = clientDataVector[clientid]->csockefd;
  std::string nickName = clientDataVector[clientid]->nickName;
  pthread_mutex_unlock(&vect);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,0);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,0);
  while(1){
    pthread_mutex_lock(&vect);
    std::string sendString = clientDataVector[clientid]->sendData;
    pthread_mutex_unlock(&vect);
    if(!sendString.empty()){
    int sent_recv_bytes = send(socket, sendString.c_str(), sendString.length(), 0);
    if(sent_recv_bytes < 0){
      if(errno == EPIPE){
        continue;
      }
      else{
        perror("error in send");
      }
    }
    pthread_mutex_lock(&vect);
    clientDataVector[clientid]->sendData ="";
    pthread_mutex_unlock(&vect);
    }
    
  }
  if(clientDataVector[clientid] != NULL){
   delete clientDataVector[clientid];
   clientDataVector[clientid] = NULL;}
    pthread_cancel(clientDataVector[clientid]->recvhandel);
    pthread_cancel(clientDataVector[clientid]->sendhandel);
return nullptr;}


















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
