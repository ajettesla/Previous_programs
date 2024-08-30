#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include "../include/kmeans.h"
#include "../include/matrix_inverse.h"


// matrix inverse variables
int N;
double A[MAX_SIZE][MAX_SIZE];
double I[MAX_SIZE][MAX_SIZE];
int maxnum;
char* Init;
int PRINT;

// For kmeans variables
int k;         // number of centroids
point data[MAX_POINTS];		// Data coordinates
point cluster[MAX_CLUSTERS]; // The coordinates of each cluster center (also called centroid)
//

#define MAXCLIENT 50

//port number
int port;
// 1 for fork. 2 for muxbasic. 3 for muxscale
int strategy;
//1 for deamonProcess is true
int deamonProcess;

//split function declaration
char **split(char *string, char *seperators, int *count);

//variables global

typedef struct clientdata{
    struct sockaddr_in clientip; //we have to give it
    int solutionNoformat; // intial state is 0
    int solutionNoforkm;  // intial stat is 0
    char filename[300];   // intially defined
    int succesfulcal;     // we get 
    int clientid;         // we have set
    int clientsocket;     //we have to set
    char command[100];    // we have to set command
}clientdata;

int clientid;

//for select sytem call fucntions

int clientFD[MAXCLIENT];

fd_set readfds;

void clearfd(){
    FD_ZERO(&readfds);
    for(int i=0; i< MAXCLIENT;i++){
        clientFD[i] = -1;
    }
}

int getmax(){
    int max = 0;
    for(int i=0; i < MAXCLIENT; i++){
        if(clientFD[i] > max){
            max = clientFD[i];
        }
    }
    return max;
}

void re_inti_readfd(fd_set *readfds){
    FD_ZERO(readfds);
    for(int i=0; i < MAXCLIENT; i++){
        if(clientFD[i] != -1){
        FD_SET(clientFD[i], readfds);}
    }
}

void setclientfd(int socket){
    for(int i=0; i < MAXCLIENT; i++){
        if(clientFD[i] == -1){
            clientFD[i] = socket;
            break;
        }
        
    }
}

void removeclientfd(int socket){
    for(int i =0; i < MAXCLIENT; i++){
        if(clientFD[i] == socket){
            clientFD[i] = -1;
        }
    }
}




//function declerations 
void intialFunction(int *port, int* strategy, int* deamonProcess,char* argv[],int argc);
int file_exists(const char *filename);

//pthread functions
void getcalculations(clientdata* buffer);
    //callback function
void* callBackfun(void* buf);
void senddata(clientdata *client);

// pointer to client
clientdata* clientarray[MAXCLIENT];

void resetclientdataarray(){
  for(int j=0; j < MAXCLIENT; j++){
    clientarray[j] = NULL;
  }
}

void removeclientformarray(clientdata* dataptr){
  for(int j=0; j< MAXCLIENT;j++){
    if(clientarray[j] == dataptr){
      clientarray[j] = NULL;
    }
  }
}

void clearclientdataarray(){
  for(int j=0; j< MAXCLIENT; j++){
    if(clientarray[j] != NULL){
    free(clientarray[j]);
    clientarray[j] = NULL;}
  }
}

void setclientinarray(clientdata* cl){
  for(int j=0; j< MAXCLIENT; j++){
    if(clientarray[j] == NULL){
        clientarray[j] = cl;
        break;
    }
  }
}

//handel with zombie process wait for child process
void handle_sigchld(int sig) {
    (void)sig; 
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0);
}

//start of mainloop
int main(int argc, char* argv[]){
clientid =0;
struct sockaddr_in clientaddress;
socklen_t addrlen = sizeof(clientaddress);

intialFunction(&port, &strategy, &deamonProcess,argv, argc);
int massocket = gsready(&port);

  int pid = fork();
  if(pid == 0){
    if(deamonProcess == 1){
    printf("The process id of deamon is %d\n", getpid());
    int fd = open("deamon.log",O_WRONLY|O_CREAT,0777);
    dup2(fd, STDOUT_FILENO);
    fflush(stdout);
    umask(0);
    close(STDERR_FILENO);
    close(STDIN_FILENO);
      if (setsid() < 0) {
          exit(1); }}
  signal(SIGCHLD,handle_sigchld);

//starting of strategy
if(strategy == 1){
    while(1){
      int csocketfd = accept(massocket,(struct sockaddr*)&clientaddress,&addrlen);
      if(csocketfd < 0){perror("error with accept function"); exit(1);}
      clientdata *client = (clientdata*)malloc(sizeof(clientdata));
      clientid++;
      client->clientid = clientid;
      setclientinarray(client);
      int cpid = fork();
      if(cpid ==0){
          close(massocket);
      while(1){
        char buffer[100];
        memset(buffer, 0, sizeof(buffer));
        int sent_recv_bytes = recv(csocketfd, buffer, sizeof(buffer),0);
        if(sent_recv_bytes < 0){perror("error with recv function 1");exit(1);}
        buffer[sent_recv_bytes] = '\0';
        //if terminate message is received than
        char terminatebuf[9] = "TERMINATE";
        if(strncmp(buffer,terminatebuf,8) == 0){removeclientformarray(client);close(csocketfd);exit(0);}
        else{
          memset(client->command,0,sizeof(client->command));
          strcpy(client->command, buffer);
          client->clientsocket = csocketfd;
          client->succesfulcal = 0;
          if(strcmp(client->command,"") !=0){
          printf("client %d command is  %s\n",client->clientid, client->command);
          getcalculations(client);
          }
          }
           
          
        }
      }
      }
  
    }
  }  

else if(strategy == 2){  
    clearfd();
    resetclientdataarray();
    setclientfd(massocket);
    while(1){
      re_inti_readfd(&readfds);
      int rc = select(getmax() + 1, &readfds, NULL, NULL, NULL);
      if(rc < 0){perror("error with select system call");exit(1);}
      if(FD_ISSET(massocket, &readfds)){
        int comsocket = accept(massocket, (struct sockaddr*)&clientaddress,&addrlen);
        if(comsocket < 0){perror("error with accept system call");exit(1);}
        setclientfd(comsocket);
        clientdata *client = (clientdata*)malloc(sizeof(clientdata));
        clientid++;
        client->clientid = clientid;
        client->clientsocket = comsocket;
        setclientinarray(client);
      }
      else{
        clientdata *client;
        int socketd;
        for(int i=0; i< getmax() + 1; i++){
          if(clientFD[i] != -1){
          if(FD_ISSET(clientFD[i], &readfds)){
            for(int j=0; j< getmax(); j++){if(clientarray[j]->clientsocket == clientFD[i]){client = clientarray[j];socketd = clientFD[i];break;}}}}}
          char buffer[100];
          memset(buffer, 0, sizeof(buffer));
          int sent_recv_bytes = recv(socketd,buffer, sizeof(buffer),0);
          if(sent_recv_bytes < 0){removeclientformarray(client);removeclientfd(socketd);close(socketd);perror("error with recv function");exit(1);}
          buffer[sent_recv_bytes] = '\0';
          char terminatebuf[9] = "TERMINATE";
          //if terminate message is received than
          if(strncmp(buffer,terminatebuf,8) == 0){removeclientformarray(client);removeclientfd(socketd);close(socketd);}
          //end of terminate message
         else{
          memset(client->command,0,sizeof(client->command));
          strcpy(client->command, buffer);
          client->clientsocket = socketd;
          client->succesfulcal = 0;
          if(strcmp(client->command,"") !=0){
          printf("client %d command is  %s\n",client->clientid, client->command);
          getcalculations(client);
          }
          }           
           //previosly sending data is here       

        
      }}

    }
    
    
    
//using select system call algorithm
//end of select system call algorithm
  
  else{
  if(deamonProcess == 1){
    exit(0);
  }
  else{
    wait(NULL);
  }
  }

  return 0;
}
//end of main loop

int gsready(int *port){
  int socketfd;
  struct sockaddr_in *ipv4 = (struct sockadrr_in*)malloc(sizeof(struct sockaddr_in));
  ipv4->sin_family = AF_INET;
  ipv4->sin_addr.s_addr = INADDR_ANY;
  ipv4->sin_port = htons(*port);
  socketfd = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

  if(bind(socketfd,(struct sockaddr*)ipv4,sizeof(struct sockaddr)) < 0){perror("error with bind function");exit(1);}

  if(listen(socketfd, 5)< 0){perror("error with listen fucntion");exit(1);}
   
  free(ipv4);
return socketfd;
}      

void getcalculations(clientdata* buf){

    pthread_t calhandel;
    pthread_create(&calhandel,NULL,callBackfun,(void*)buf);
    pthread_join(calhandel,NULL);
 
}

int file_exists(const char *filename) {
   if(access(filename,F_OK) == 0){
    return 1; //file exists
   }  
   else {
    return 0;//file does'nt exits
   }
}


void senddata(clientdata *client){
          int csocket = client->clientsocket;
          if(client->succesfulcal == 1){
          FILE *file = fopen(client->filename,"r");
          fseek(file, 0, SEEK_END);
          long filesize = ftell(file);
          fseek(file, 0, SEEK_SET);
          char sendbuffer[1024];
          int sent_recv_bytes = send(csocket, &filesize, sizeof(filesize),0);
          if(sent_recv_bytes < 0){removeclientformarray(client);close(csocket);perror("error with recv function 2");exit(1);}
          while(1){
                  memset(sendbuffer, 0, sizeof(sendbuffer));
                  int sent_recv = fread(sendbuffer, 1, sizeof(sendbuffer), file);
                  if(sent_recv < 0){break;}
                  if(sent_recv_bytes == 0){break;}
                  sent_recv_bytes = send(csocket, sendbuffer, sent_recv, 0);
                  if(sent_recv_bytes < 0){close(csocket);removeclientformarray(client);removeclientfd(csocket);close(csocket);perror("error with send function 3");break;}}
          
          fclose(file);     
          printf("sending data to client %s\n", client->filename);
          
        }
        else{
          printf("unable to make calculations\n");
          char buffer[] = "fail to calcutate results\n";
          int sent_recv_bytes = send(csocket, buffer, sizeof(buffer),0);
          if(sent_recv_bytes < 0){removeclientformarray(client);removeclientfd(csocket);close(csocket);perror("error with sent function");exit(1);}
        }
}


void* callBackfun(void* buf){

    clientdata* data = (clientdata*)buf;
    char matinvbasefilename[] = "matinv_client";
    char kmeansbasefilename[] = "kmeans_client";
    int *count = (int*)malloc(sizeof(int));
    char **list =  split(data->command, " ", count);
    char substring[5];
    int startindex = 0;
    int length = 4;
    if(startindex + length <= strlen(list[0])){
        strncpy(substring, list[0] + startindex,length);
        substring[length] = '\0';
    }
    else{
      printf("invalid input\n");
    }
    if(strcmp(substring,"mati")==0){
      int solutionno = data->solutionNoformat;
      int clientno = data->clientid;
      int counter = 1;
      char filename[200];
      char directorypath[] = "mathserver/computed_results/";
      char* tempfile = (char *)malloc(strlen(directorypath) + strlen(filename) + sizeof(char)*50);
    
      //file check exist and select name for file
      while(1){
        snprintf(filename, strlen(matinvbasefilename)+25,"%s_%d_solution_%d.txt",matinvbasefilename,clientno,counter);
        strcpy(tempfile, directorypath);
        strcat(tempfile,filename);
        if(!file_exists(tempfile)){
          data->solutionNoformat = counter;
          break;
        }
        counter++;
      }
    
      //end of createing file name
      //open file and saving output in file 
  
      int charcount;
      char **com = split(data->command," ", &charcount);
      startcalulation(tempfile, charcount, com);
      data->solutionNoformat = data->solutionNoformat + 1;
      char command[100];
      strcpy(command,data->command);
      if(file_exists(tempfile) == 1){
      data->succesfulcal = 1;
      strcpy(data->filename, tempfile);}
      else{
         data->succesfulcal = 10;}
    
      //end of calculation and closeing file
    }
    else if(strcmp(substring,"kmea")==0){
     
      int solutionno = data->solutionNoforkm;
      int clientno = data->clientid;
      int counter = 1;
      char filename[200];
      char directorypath[] = "mathserver/computed_results/";
      char* tempfile = (char *)malloc(strlen(directorypath) + strlen(filename) + sizeof(char)*50);
         
       while(1){
        snprintf(filename, strlen(kmeansbasefilename)+25,"%s_%d_solution_%d.txt",kmeansbasefilename,clientno,counter);
        strcpy(tempfile, directorypath);
        strcat(tempfile,filename);
        if(!file_exists(tempfile)){
          data->solutionNoforkm = counter;
          break;
        }        
        counter++;
      }
      
    memset(data->filename, 0, sizeof(data->filename));
    
    int count;
    char **com = split(data->command, " ", &count);
    char *buffer[30];
  
    for(int i=0; i <count; i++){
      if(strcmp(com[i], "-k" ) == 0){
        k = atoi(com[i+1]);
      }
      if(strcmp(com[i],"-f")==0){
        strcpy(buffer,com[i+1]);
      }
    }
  
    char check[200] = "mathserver/src/";
    strcat(check,buffer);
   
    if(file_exists(check) == 1){
    startkmeans(tempfile, buffer);
    data->solutionNoforkm = data->solutionNoforkm + 1;
    }
    else{
      data->succesfulcal = 5;
      printf("file %s not fount sorry !",buffer);
    }

    if(file_exists(tempfile) == 1){
      data->succesfulcal = 1;
      strcpy(data->filename, tempfile);
    }
    else{
      if(data->succesfulcal != 5){
      data->succesfulcal = 10;}
    }


  }
    else{
      if(strcmp(data->command,"TERMINATE\n")==0){
        close(data->clientsocket);
        removeclientfd(data->clientsocket);
        removeclientformarray(data);
      }
      else{
      printf("invalid command form the client.\n");}

      data->succesfulcal = 10;
    }
    senddata(data);
   for (int i = 0; i < *count; i++){free(list[i]);}
   

return NULL;
}

void intialFunction(int *port, int* strategy, int* deamonProcess,char* argv[],int argc){
     for(int i=0; i< argc; i++)
    {
        if(strcmp(argv[i],"-p")==0){
            *port = atoi(argv[i+1]);
        }

        if(strcmp(argv[i],"-h")==0){
            printf("-p port Listen to port number PORT \n");
            printf("-d Run as a daemon instead of as a normal program.\n");
            printf("-s strategy Specify the request handling strategy: fork, muxbasic, or muxscale.\n");
            printf("****strategy is mandatory.****\n");
            printf("****port number mandatory.****\n");           
            break;
            exit(1);
        }

        if(strcmp(argv[i],"-s") == 0){
            if(strcmp(argv[i+1],"fork")==0){
                *strategy = 1;
            }
            else if(strcmp(argv[i+1],"muxbasic")==0){
                *strategy = 2;
            }
            else if(strcmp(argv[i+1],"muxscale") == 0){
                *strategy = 3;
            }
            else{
                printf("You have entered wrong strategy.\n");
            }
        }
        if(strcmp(argv[i],"-d")==0){
            *deamonProcess = 1;
        }

    }

    if(*strategy > 3 || *strategy < 0){
            printf("-p port Listen to port number PORT \n");
            printf("-d Run as a daemon instead of as a normal program.\n");
            printf("-s strategy Specify the request handling strategy: fork, muxbasic, or muxscale.\n");
            printf("****strategy is mandatory.****\n");
            printf("****port number mandatory.****\n");
            exit(1);
    }
    else if(*port == 0){
            printf("-p port Listen to port number PORT \n");
            printf("-d Run as a daemon instead of as a normal program.\n");
            printf("-s strategy Specify the request handling strategy: fork, muxbasic, or muxscale.\n");
            printf("****strategy is mandatory.****\n");
            printf("****port number mandatory.****\n");
            exit(1);
    }
}


char **split(char *string, char *seperators, int *count)
{

  int len = strlen(string);
  
  *count = 0;
 
  int i = 0;
  while (i < len)
  {
   
    while (i < len)
    {
      
      if (strchr(seperators, string[i]) == NULL)
        break;
      i++;
    }
    
  
    int old_i = i;
    while (i < len)
    {
   
      if (strchr(seperators, string[i]) != NULL)
        break;
      i++;
    }

   
    if (i > old_i) *count = *count + 1;
  }

  char **strings = malloc(sizeof(char *) * *count);
  

  i = 0;

 
  char buffer[100];
  int string_index = 0;
  while (i < len)
  {
    
    while (i < len)
    {
      if (strchr(seperators, string[i]) == NULL)
        break;
      i++;
    }
    
   
    int j = 0;
    while (i < len)
    {
      if (strchr(seperators, string[i]) != NULL)
        break;
      
      buffer[j] = string[i];
      i++;
      j++;
    }
    
    
    if (j > 0)
    {
     
      buffer[j] = '\0';

     
      int to_allocate = sizeof(char) *
                        (strlen(buffer) + 1);
     
      strings[string_index] = malloc(to_allocate);

      strcpy(strings[string_index], buffer);
      
     
      string_index++;
    }
  }

 
  return strings;
}
