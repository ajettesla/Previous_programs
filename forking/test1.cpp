#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>





int main(int argc, char* argv[]){


while(1){
std::string ajet {"anroop"};
std::cout << ajet << std::endl;
}

kill(getpid(),SIGTSTP);
int fd = open("/dev/null", O_WRONLY);





    return 0;
}