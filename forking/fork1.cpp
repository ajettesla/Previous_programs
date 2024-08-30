#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string>


int main(int argc, char* argv[]){

int fd[2];

if(pipe(fd)== -1){
    perror("error pipe");
    return 1;
}
int id = fork();
if(id == -1){
    perror("error withd forking");
    return 4;
}

if(id == 0){
    close(fd[0]);
    char buffer[] = "ajettesla";
    if(write(fd[1], buffer,sizeof(buffer)) == -1){
        perror("error wirte");
        return 2;
    }
    close(fd[1]);
}
else{
    close(fd[1]);
    char buffer[30];
    if(read(fd[0],buffer, 30) == -1){
        perror("error read");
        return 3;
    }
    std::string z(buffer);
    close(fd[0]);
    std::cout << "we get the value " << z << std::endl;
}






 return 0;
}
