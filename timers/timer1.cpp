#include <iostream>
#include <time.h>
#include <vector>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <random>
#include <stdlib.h>
#include <algorithm>

std::vector<int> clientid;

void removeClientid(int data) {
    auto it = std::find(clientid.begin(), clientid.end(), data);
    if (it != clientid.end()) {
        clientid.erase(it);}}

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

void printvector(){
    for(int j=0; j < clientid.size(); j++){
        std::cout << clientid[j] << " ";
    }
    std::cout << std::endl;
}

void printCurrentTime() {
    time_t t;
    time(&t);
    std::cout << ctime(&t) << std::endl;
}

int main(int argc, char* argv[]) {
    getRandomNumberNotInVector();
    clientid.push_back(10);
    printvector();
    removeClientid(10);
    std::cout << " after removing cliend id 10 \n";
    printvector();
    pause();
    return 0;
}
