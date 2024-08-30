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
#include <queue>


#define MAXCLIENTS 50
fd_set readfds;

int CLIENTARRAY[MAXCLIENTS];

//mutex lock
pthread_mutex_t vect;
pthread_mutex_t que;
pthread_cond_t cv;
//


//readfds
void reintfdset(){
  FD_ZERO(&readfds);
  for(int i=0; i< MAXCLIENTS; i++){
    if(CLIENTARRAY[i] != -1){
      FD_SET(CLIENTARRAY[i],&readfds);
    }
  }

}

void resetfdset(){
  for(int i=0; i < MAXCLIENTS; i++){
    CLIENTARRAY[i] = -1;
  }

}

void addclienttofdset(int client){
  for(int i=0; i< MAXCLIENTS; i++){
    if(CLIENTARRAY[i] == -1){
      CLIENTARRAY[i] = client;
      break;
    }
  }

}

int getmaxfdset(){
  int max = 0;
  for(int i=0; i<MAXCLIENTS;i++){
    if(CLIENTARRAY[i] > max){
      max = CLIENTARRAY[i];
    }
  }
  return max;
}

void removefdset(int client){

  for(int i=0; i<MAXCLIENTS;i++){
    if(CLIENTARRAY[i] == client){
      
      CLIENTARRAY[i] = -1;
    
      break;
    }
  }
}

void printfdset(){
  for(int i=0; i<MAXCLIENTS; i++){
    std::cout << CLIENTARRAY[i] << " ";
  }
  std::cout << std::endl;
}
// end of fdset functions

//structures defination
typedef struct clientDataStruct{
    int csockefd;
    std::string nickName;
    int clientid;
    struct sockaddr_in clientip;
    struct sockaddr_in6 clientip6;
    int ipstatus;
    pthread_t recvhandel;;
}clientDataStruct;

typedef struct clientSendStruct{
  int csoketfd;
  char sendMessage[300];
}clientSendStruct;
//structure

//declartion of functions
std::vector<std::string> split(std::string sString,std::string delimiter);
void printIP(struct sockaddr_in *ipv4);
void printIP(struct sockaddr_in6 *ipv6);
void createClientthread(int csocket);
void gsready(std::string &ip, int port, struct sockaddr_in *ipv4,struct sockaddr_in6 *ipv6, int* ipstatus, int *msocket);
//thread callback functions
void *recvCallback(void *clientsocket);
void *sendCallback(void *nothing);
//

//global variables
std::vector<clientDataStruct*> clientDataVector;
int clientID;
std::queue <clientSendStruct*> queu;
socklen_t addrlen;
socklen_t addrlen6;
std::vector <int> socketVector;
//


int main(int argc, char *argv[]){
 clientID = 0;
 pthread_mutex_init(&vect, NULL);
 pthread_mutex_init(&que, NULL);
 pthread_cond_init(&cv, NULL);
 //intial setup 
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
int *msocket = new int;
//end intial setup

gsready(ipString ,port, ipv4, ipv6, ipstatus, msocket);

if(listen(*msocket, 5) < 0){
  perror("error listen function");
  exit(1);
}
resetfdset();
FD_SET(*msocket, &readfds);
//server sending thread
pthread_t sendMessages;
pthread_create(&sendMessages, NULL, sendCallback, NULL);
//server sending thread

while(1){
  printfdset();
  if(select(*msocket + 1,&readfds,NULL, NULL, NULL) < 0){
    perror("error with select");
    exit(1);}

  if(FD_ISSET(*msocket, &readfds)){
    struct sockaddr_in clientaddr;
    struct sockaddr_in6 clientaddr6;
    int csocketfd;
    if(*ipstatus == 1){csocketfd = accept(*msocket,(struct sockaddr*)&clientaddr, &addrlen);
    printIP(&clientaddr);std::cout << " is connected " << std::endl;
    clientDataStruct *client = new clientDataStruct;
    client->clientid = clientID;
    client->clientip = clientaddr;
    client->csockefd = csocketfd;
    client->ipstatus = 1;
    clientID++;
    pthread_mutex_lock(&vect);
    clientDataVector.push_back(client);
    pthread_mutex_unlock(&vect);
    printf("client id %d \n", clientID);
    }
    else if(*ipstatus == 2){csocketfd = accept(*msocket, (struct sockaddr*)&clientaddr6, &addrlen6);
    printIP(&clientaddr6);std::cout << " is connected " << std::endl;
    clientDataStruct *client = new clientDataStruct;
    client->clientid = clientID;
    client->clientip6 = clientaddr6;
    client->csockefd = csocketfd;
    client->ipstatus = 2;
    pthread_mutex_unlock(&vect);
    clientDataVector.push_back(client);
    pthread_mutex_unlock(&vect);
    }
    addclienttofdset(csocketfd);
    createClientthread(csocketfd);
   }

} //end of while loop

pthread_mutex_lock(&vect);
for(int i=0; i< clientDataVector.size(); i++){
  if(clientDataVector[i] != nullptr){
    delete clientDataVector[i];
    clientDataVector[i] = nullptr;
  }
}
pthread_mutex_unlock(&vect);


return 0;
}

void createClientthread(int csocket){
   pthread_t clienthandel;
   int *sock = new int;
   *sock = csocket;
   pthread_create(&clienthandel,NULL,recvCallback,(void*)sock);
   pthread_mutex_lock(&vect);
   for(int j=0; j<clientDataVector.size(); j++){
    if(clientDataVector[j]->csockefd == csocket){
      clientDataVector[j]->recvhandel = clienthandel;
      break;}
   }
   pthread_mutex_unlock(&vect);
}


void *recvCallback(void *clientsocket){
    int csocketfd = *(int*)clientsocket;
    char testbuffer[] = "TEXT_TCP_1.0\n";
    int sent_recv_bytes = send(csocketfd,testbuffer,sizeof(testbuffer),0);
    if(sent_recv_bytes < 0){
        perror("erro with send function");
        pthread_exit(0);
        exit(1);}
    char nickName[30];
    memset(nickName,0, sizeof(nickName));
    sent_recv_bytes = recv(csocketfd, nickName, sizeof(nickName),0);
    if(sent_recv_bytes < 0){
      perror("error with recv fucntion on nick name");
      pthread_exit(0);
      exit(1);}
    nickName[sent_recv_bytes - 1] = '\0';
    char colum[4] = " : ";
    strcat(nickName, colum);

  while(1){
    char recvtotalbuffer[300];
    memset(recvtotalbuffer, 0, sizeof(recvtotalbuffer));
    strcpy(recvtotalbuffer, nickName);
    char recvbuffer[300];
    memset(recvbuffer,0,sizeof(recvbuffer));
    sent_recv_bytes = recv(csocketfd, recvbuffer, sizeof(recvbuffer), 0);
    if(sent_recv_bytes < 0){
     perror("error with recv");pthread_exit(0);exit(1);}
     if(strcmp(recvbuffer,"TERMINATE\n") == 0){
      removefdset(csocketfd);
      std::cout << " removed successfully " << std::endl;
      printfdset();
      clientID--;
      close(csocketfd);
      pthread_mutex_lock(&vect);
      int k;
      for(int i=0; i < clientDataVector.size(); i++){
        if(clientDataVector[i]->csockefd == csocketfd){
          k = i;
          if(clientDataVector[i]->ipstatus == 0){
            printIP(&(clientDataVector[i]->clientip));
          }
          else{
             printIP(&(clientDataVector[i]->clientip6));
          }
          
        }
      }
      pthread_mutex_unlock(&vect);
      delete clientDataVector[k];
      if(k > 0 && k < clientDataVector.size()) {
        clientDataVector.erase(clientDataVector.begin() + k);
      }
      pthread_exit(0);
      exit(0);
     }
    strcat(recvtotalbuffer, recvbuffer);
    clientSendStruct *clientdata = new clientSendStruct;
    clientdata->csoketfd = csocketfd;
    strcpy(clientdata->sendMessage, recvtotalbuffer);
    if(clientID > 1){
    pthread_mutex_lock(&que);
    queu.push(clientdata);
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&que);}
  }



  return nullptr;
}


void *sendCallback(void *nothing){
  
    while(1){
      if(clientID > 1){
    pthread_mutex_lock(&que);
    if(queu.empty()){
        pthread_cond_wait(&cv,&que);}//end of if l
    if(!queu.empty()){
       for(int i=0; i < getmaxfdset()+1; i++){
        if(CLIENTARRAY[i] != -1){
          if(CLIENTARRAY[i] != queu.front()->csoketfd){
            int sent_recv_bytes = send(CLIENTARRAY[i], queu.front()->sendMessage, sizeof(queu.front()->sendMessage), 0);
            if(sent_recv_bytes < 0){perror("error with send function");exit(1);}
          }
        }
       }
      queu.pop();   
    pthread_mutex_unlock(&que);
    }
   //end of else loop
    }//end of clientID if loop
   }//end of while loop
  
return nullptr;
}

void printIP(struct sockaddr_in *ipv4){
  char ipv4buffer[INET_ADDRSTRLEN];
  const char *result = inet_ntop(AF_INET,&(ipv4->sin_addr),ipv4buffer,sizeof(ipv4buffer));
  if(result == nullptr){
    perror("error with inet_ntop");}
    std::cout << ipv4buffer << " port " << ipv4->sin_family;
    }


void printIP(struct sockaddr_in6 *ipv6){
  char ipv6buffer[INET6_ADDRSTRLEN];
  const char *result = inet_ntop(AF_INET,&(ipv6->sin6_addr),ipv6buffer,sizeof(ipv6buffer));
  if(result == nullptr){
    perror("error with inet_ntop");}
    std::cout << ipv6buffer << " port " << ipv6->sin6_port;
}

void gsready(std::string &ip, int port, sockaddr_in *ipv4, sockaddr_in6 *ipv6, int* ipstatus, int *msocket){
struct addrinfo hint, *output, *temp;
memset(&hint, 0, sizeof(hint));
hint.ai_family = AF_UNSPEC;
hint.ai_socktype = SOCK_STREAM;
int status = getaddrinfo(ip.c_str(), NULL, &hint, &output);
if(status != 0){perror("getaddress info");}

for(temp=output; temp != NULL;temp->ai_addr){
if(temp->ai_family == AF_INET){
ipv4->sin_family = AF_INET;
ipv4->sin_port = htons(port);
ipv4->sin_addr.s_addr = ((struct sockaddr_in*)temp->ai_addr)->sin_addr.s_addr;
*msocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
if(*msocket > 0){
  *ipstatus = 1;
  if(bind(*msocket,(struct sockaddr*)ipv4,sizeof(struct sockaddr)) < 0){
    perror("error with bind error");
    exit(1);}
    else{
    break;}}
  }
                                              
else if(temp->ai_family == AF_INET6){
ipv6->sin6_family = AF_INET6;
ipv6->sin6_port = htons(port);
ipv6->sin6_addr = ((struct sockaddr_in6*)temp->ai_addr)->sin6_addr;
*msocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
if(*msocket > 0){
  *ipstatus = 2;
  if(bind(*msocket,(struct sockaddr*)ipv6,sizeof(struct sockaddr_in6)) < 0){
     perror("error with bind error");}
     else{
     break;}}
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
         nString.push_back(temp);}  }



return nString;
}
