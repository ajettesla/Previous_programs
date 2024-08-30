#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>


void *callbackfun(void *arg){

while(1){
    std::string te; 
    std::getline(std::cin, te);
    *(std::string *)arg = te;
}

return nullptr;}





int main(){



pthread_t handel1;

pthread_attr_t attr;

pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

std::string *input = new std::string;

void *threadresult;

pthread_create( &handel1, NULL, callbackfun, (void*)input);

while(1){

    std::cout << "This is main loop"<< std::endl;
    
    std::cout << *input << std::endl;

    sleep(1);

}












}