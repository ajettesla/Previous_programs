#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <errno.h>


void* callback(void *arg){
    std::string t = "thread";
    int *a = (int*)arg;
    t = t + std::to_string(*a);

    std::ofstream outF;

    outF.open(t);

     if (!outF.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
    std::exit(1); // Return an error code
    }
 
    while(1){
        outF << t << std::endl;
        sleep(1);
    }
    outF.close();

    return nullptr;
}


int main(){

int noOfthreads = 5;

for(size_t i = 0; i<noOfthreads; i++){
    pthread_t handel[noOfthreads];
    size_t rc = pthread_create(&handel[i], NULL, callback, (void*)&i);
     if (rc != 0) {
            std::cerr << "Error creating thread: " << rc << std::endl;
            // Handle the error as needed
        }
}

while(1){
    std::string a;
    std::getline(std::cin,a);
    int b = std::stoi(a);
    std::cout << b << std::endl;
}







return 0;}
