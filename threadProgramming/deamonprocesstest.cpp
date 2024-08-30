#include <iostream>
#include <fcntl.h>
#include <unistd.h>



int main(){

int pid = fork();
if(pid <0){
    perror("error with fork");
}

if(pid == 0){

if(setsid() == -1){
    perror("unable to setsid");
    return 2;}

if(chdir("/") == 1){
    perror("error changing the working directory");}
    
    std::cout <<" proccess id of deamon process is " << getpid() << std::endl;
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    close(STDIN_FILENO);
    open("/dev/null", O_RDONLY); 
    open("/dev/null", O_WRONLY); 
    open("/dev/null", O_WRONLY); 

    while(true){
        std::cout << "hallo world" << std::endl;
        sleep(1);
    }
}

    return 1;
}

