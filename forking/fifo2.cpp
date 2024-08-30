#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>


int main(int argc, char* argv[]){


    int arr[5];
    srand(time(NULL));
    
    for(size_t i=0; i< 5; i++){
        std::cin >> arr[i];
    }

    if(mkfifo("myfifo",0777) == -1){
        if(errno != EEXIST){
        std::cout << "error in createing fifo file ";}
    }

    int fileO = open("myfifo",O_RDWR);

    if(fileO == -1){
        std::cout << "error with opening the file " << std::endl;
    }

    for(size_t i = 0; i < 5 ; i++){

        if(write(fileO, &arr[i], sizeof(int))== -1){
            std::cout << "error while writing to file " << std::endl;

        }
    }
    close(fileO);
    fileO = open("myfifo", O_RDONLY);
    int sum {0};
    if(read(fileO, &sum, sizeof(int))== -1){
        std::cout << "error while reading the file " << std::endl;
    }
    std::cout << "total sum is " << sum << std::endl;
    close(fileO);













    return 0;
}