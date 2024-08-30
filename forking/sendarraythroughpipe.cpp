#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

//first we have to send size of array
//second we have to send array it self

int main(int argc, char* argv[]){

int pidc = fork();
int fd[2];
if(pipe(fd) < 0){
    std::cout << "error with pipe \n";
    return 2;
}

if(pidc == -1){
    std::cout << "error with fork \n";
    return 1;
}

if(pidc == 0){
    close(fd[0]);
    int n, i;
    int arr[10];
    for(size_t i {0}; i < 10; i++){
        srand(time(NULL));
         n = rand() % 10 + 1;
         arr[i] = n;
    }
    if(write(fd[1], arr, sizeof(arr)) < 0){
        std::cout << "error with write \n";
        return 3;
    }

}
else{
    close(fd[1]);

    for(size_t j = 0; j < 10; j++){

    }
}








    return 0;
}