#include <iostream>
#include <pthread.h>
#include <unistd.h>

int digits[5] {1,2,3,4,5};

pthread_mutex_t Arr;


void *swapCall(void* arg){
    while(1){
    pthread_mutex_lock(&Arr);
    int temp = digits[4];
    digits[4] = digits[1];
    digits[1] = temp;
    pthread_mutex_unlock(&Arr);}

return nullptr;}

void *sumCall(void* arg){
     int j = 0;
    while(1){
        pthread_mutex_lock(&Arr);
        for(size_t i=0; i<5; i++){
           j = j+ digits[i]; 
        }
        std::cout << j << std::endl;
        j = 0;
        pthread_mutex_unlock(&Arr);
        
    }
return nullptr;
}


int main(){
pthread_mutex_init(&Arr, 0);
pthread_t swapHandel;
pthread_t sumHandel;

pthread_create(&swapHandel, NULL, swapCall, NULL);
pthread_create(&sumHandel, NULL, sumCall, NULL);
while(1){

}
pthread_mutex_destroy(&Arr);

return 0;}