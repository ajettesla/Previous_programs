#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <random>
#include <string>


typedef struct structTest{
    std::string buffer;
    int integer_b;
}structTest;

std::queue<structTest*> que;
pthread_mutex_t qu;
pthread_cond_t cv;


std::string getstring(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> intDistribution(1, 100);
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string randomString;
    std::uniform_int_distribution<int> stringLengthDistribution(8, 12); 

    int randomStringLength = stringLengthDistribution(gen);
    for (int i = 0; i < randomStringLength; ++i) {
    int randomIndex = intDistribution(gen) % charset.length();
    randomString += charset[randomIndex];
    }

return randomString;
}


int getinteger(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> intDistribution(1, 100);
    int randomNumber = intDistribution(gen);
return randomNumber;
}

void* callback(void *b){
    while(1){
    pthread_mutex_lock(&qu);
    if(que.empty()){
        std::cout << "que is empty" << std::endl;
        pthread_cond_wait(&cv,&qu);}
    else{
        std::cout << que.front()->buffer << " number is " << que.front()->integer_b << std::endl;
        que.pop();}
    pthread_mutex_unlock(&qu);
    }
    return nullptr;
}

void *pushcallback(void *c){
std::string nick = *(std::string*)c;
nick = nick + " : ";
while(1){
structTest *test = new structTest;
test->buffer = getstring();
test->buffer =  nick + test->buffer;
test->integer_b = getinteger();
pthread_mutex_lock(&qu);
que.push(test);
pthread_cond_signal(&cv);
pthread_mutex_unlock(&qu);
sleep(1);
}
}


int main(){
pthread_mutex_init(&qu, NULL);
pthread_cond_init(&cv, NULL);

pthread_t outputhandel;
pthread_create(&outputhandel, NULL, callback, NULL);

while(1){
std::cout << "enter the name for each\n";
std::string *nick = new std::string;
std::getline(std::cin, *nick);

pthread_t handel;
pthread_create(&handel,NULL, pushcallback,(void*)nick);
sleep(1);
}





}