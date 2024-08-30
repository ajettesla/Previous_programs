#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>



int main(int argc, char* argv[]){

    int pid = fork();
    if(pid < 0){
        std::cout << "error with fork" << std::endl;
    }

    if(pid == 0){
        while(1){
            std::string ajet;
            std::cout << "print any thing on the screen " << std::endl;
            sleep(1);
            std::cin >> ajet;
            std::cout << ajet << std::endl;
        }
    }
    kill(getpid(), SIGTSTP);
 
    return 0;
}