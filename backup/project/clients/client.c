#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>

char ip[INET_ADDRSTRLEN];
int port;

int socketfd;

int gsready(int *port, char *ip){
  int socketfd;
  struct in_addr ipb;
  socklen_t addrlen = sizeof(struct sockaddr);
  struct sockaddr_in *ipv4 = (struct sockadrr_in*)malloc(sizeof(struct sockaddr_in));
  if(inet_pton(AF_INET, ip, &ipb) < 0){perror("error with inepton");exit(1);}
  ipv4->sin_family = AF_INET;
  ipv4->sin_addr.s_addr = ipb.s_addr;
  ipv4->sin_port = htons(*port);
  socketfd = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

  if(connect(socketfd,(struct sockaddr*)ipv4, addrlen) < 0){perror("error with connect");exit(1);}
     
  free(ipv4);
return socketfd;
}  

int file_exists(const char *filename) {
   if(access(filename,F_OK) == 0){
    return 1; //file exists
   }  
   else {
    return 0;//file does'nt exits
   }
}

void exitProgram(int signal){
  printf("CTR-C is pressed closing socket.\n");
  char *buffer = "TERMINATE";
  int sent_recv_bytes = send(socketfd, buffer, sizeof(buffer),0);
  if(sent_recv_bytes < 0){perror("error with send fucntion"); close(socketfd);exit(1);}
  close(socketfd);
  exit(0);
}


int main(int argc, char* argv[]){
    char input[100];
    char matbasefile[] = "matinv_client_1_solution_";
    char kmeansfile[] = "kmeans_client_1_solution_";
    for(int i=0; i < argc; i++){
        if(strcmp(argv[i],"-ip") == 0){strcpy(ip, argv[i+1]);}
        if(strcmp(argv[i],"-p") == 0){port = atoi(argv[i+1]);}}

    
    socketfd = gsready(&port,ip);
    signal(SIGINT,exitProgram);

          while(1){
            printf("please enter the command : ");
            memset(input, 0, sizeof(input));
          if (fgets(input, sizeof(input), stdin) != NULL) {
             size_t length = strlen(input);
             if (input[length - 1] == '\n'){input[length - 1] = '\0'; }}else{printf("you have entered invalid string\n");exit(1);}
        
          int sent_recv_bytes = send(socketfd, input,strlen(input),0);
          if(sent_recv_bytes < 0){perror("error with send function");exit(1);}
          char recvbuffer[1024];
            int startindex = 0;
            int length = 4;
            char substring[5];
             strncpy(substring, input + startindex, length);
             substring[length] = '\0';
             int counter = 1;
             char filename[100];
             char directorypath[150] = "clients/results/";
             char tempfile[350];
             memset(filename, 0,sizeof(filename));
             memset(tempfile, 0, sizeof(tempfile));
             int k = 0;

            //creating file name 
            if(strcmp(substring,"mati")==0){
                while(1){
                   snprintf(filename, strlen(matbasefile) + 15,"%s%d.txt",matbasefile,counter);
                   strcpy(tempfile, directorypath);
                   strcat(tempfile,filename);
                   if(!file_exists(tempfile)){break;}
                    counter++;
                            }
                           
                              k = 1;

                      }
            else if(strcmp(substring,"kmea") == 0){
                while(1){
                   snprintf(filename, strlen(kmeansfile)+ 15,"%s%d.txt",kmeansfile,counter);
                   strcpy(tempfile, directorypath);
                   strcat(tempfile,filename);
                   if(!file_exists(tempfile)){break;}
                          counter++;
                         
                            } k = 1;
                            }
            else{
              printf("invlaid command is entered\n");
              k = 10;
            }
           // end of creation
          if(k == 1){
          long filesize;
          sent_recv_bytes = recv(socketfd,&filesize, sizeof(filesize),0);
          if(sent_recv_bytes < 0){close(socketfd);perror("error with recv function");exit(1);}
          int received_bytes = 0;
          FILE* file = fopen(tempfile,"w");
          if(file == NULL){perror("error with opening file"); exit(1);}
          if(filesize < 1024){
            memset(recvbuffer,0,sizeof(recvbuffer));
            sent_recv_bytes = recv(socketfd, recvbuffer,sizeof(recvbuffer),0);
            fprintf(file,"%s",recvbuffer);
            if (sent_recv_bytes < 0) {fclose(file);perror("error with recv function");close(socket);exit(1);}
             fclose(file);
          }
          else{
          while (received_bytes < filesize){
            memset(recvbuffer,0,sizeof(recvbuffer));
            sent_recv_bytes = recv(socketfd, recvbuffer, sizeof(recvbuffer), 0);
            if (sent_recv_bytes < 0) {fclose(file);perror("error with recv function");close(socket);exit(1);}
            else {fprintf(file,"%s",recvbuffer);}
            received_bytes = received_bytes + sent_recv_bytes;
            }
              fclose(file);
              }

              printf("Received the solution %s\n", filename);
      
           
          }}
           

    

return 0;}

