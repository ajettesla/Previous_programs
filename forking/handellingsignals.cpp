#include <iostream>
#include <unistd.h>
#include <signal.h>

void hsignal(int sig){
    std::cout << "signal is handeled " << std::endl;
    fflush(stdout);
}

int main(int argv, char* argc[]){

    struct sigaction sa;
    sa.sa_handler = &hsignal;
    sa.sa_flags = SA_RESTART;

    sigaction(SIGTSTP, &sa, NULL);


    int x;
    std::cout << "input x " << std::endl;
    std::cin >> x;
    std::cout << x;










    return 0;
}