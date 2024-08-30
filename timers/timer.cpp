#include <iostream>
#include <time.h>
#include <vector>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <random>
#include <stdlib.h>
#include <algorithm>
#include <memory.h>

void printtime(){
    time_t t;
    time(&t);
    std::cout << ctime(&t) << std::endl;}

std::vector<int> clientid;

void removeClientid(int data) {
    auto it = std::find(clientid.begin(), clientid.end(), data);
    if (it != clientid.end()) {
        clientid.erase(it);}}\


void printvector(){
    for(int j=0; j < clientid.size(); j++){
        std::cout << clientid[j] << " ";
    }
    std::cout << std::endl;
}

int getRandomNumberNotInVector() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 10000);

    int randomNum;
    bool isInVector = false;

    do {
        randomNum = distribution(gen); // Generate a random number
        isInVector = std::find(clientid.begin(), clientid.end(), randomNum) != clientid.end();
    } while (isInVector);
    clientid.push_back(randomNum);

    return randomNum;
}

typedef struct pait_{
   int a;
}pait_;

void timer_callback(union sigval arg){
    pait_ *pair = (pait_*)arg.sival_ptr;
    removeClientid(pair->a);
    printvector();


}

void timerdemo(int data){
    struct sigevent evp;

    timer_t timer;

    memset(&timer, 0, sizeof(timer_t));

    memset(&evp, 0, sizeof(struct sigevent));

    pait_ *p = new pait_;
    p->a = data;

    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = timer_callback;
    evp.sigev_value.sival_ptr = (void*)p;

    int ret = timer_create(CLOCK_REALTIME, &evp, &timer);

    if(ret < 0){
        perror("error with timer create ");
        exit(1);
    }

    struct itimerspec ts;
    ts.it_value.tv_sec = 5;
    ts.it_value.tv_nsec = 0;

    ts.it_interval.tv_nsec = 0;
    ts.it_interval.tv_sec = 0;

    ret = timer_settime(timer, 0, &ts, NULL);
    if(ret < 0){
        perror("error with time settimeout");
        exit(1);
    }
    else{
        std::cout << "timer is created successfully" << std::endl;
    }
}



int main(){
int a = getRandomNumberNotInVector();
timerdemo(a);
pause();





    return 0;
}