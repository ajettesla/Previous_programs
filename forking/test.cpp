#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

    int pid = fork();

    if(pid == 0){
        std::cout << "this is child procsses " << std::endl;
        std::cout << getppid() << "--< parent id " << "-->childid " << getpid() << std::endl; 
        std::cout << "this is pid " << pid << std::endl;
        while(1){
            std::cout << "hii" << std::endl;
        }
    }
    else{
        std::cout << "this is parent " << std::endl;
        std::cout << getppid() << "--< parent id " << "-->childid " << getpid() << std::endl; 
        std::cout << "this is pid " << pid << std::endl;
    }
    sleep(1);
    kill(pid,SIGKILL);
    wait(NULL);





    return 0;
}