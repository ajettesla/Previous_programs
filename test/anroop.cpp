#include <unistd.h>
#include <pthread.h>
#include <iostream>

void* callback(void *arg);


int main(){
pthread_t handel;
pthread_create(&handel, NULL, callback, NULL);

while(1){

 sleep(1);
 std::cout << "hii" << std::endl;}

pthread_join(handel, NULL);

return 0;
}


void *callback(void *arg){

    while(1){
        sleep(2);
        std::cout << " ok " << std::endl;
    }
}