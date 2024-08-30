#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


int main(int argc, char* argv[]){

     int arr[] = {1,2,3,4,5,6,7,8,9,10};
     int fd[2];
     int f[2];
     int sizeOfArray = sizeof(arr)/sizeof(int);
     int start, end;
     if(pipe(fd) == -1){
        std::cout << "error with pipe" << std::endl;
        return 2;
     }
     if(pipe(f) == -1){
        std::cout << "error with pipe" << std::endl;
        return 2;
     }
     int id = fork();
     int id1 = fork();
     if(id == -1){
        std::cout << "error with fork" << std::endl;
        return 1;
     }

     if(id == 0){
        if(id1 =! 0){
            start = sizeOfArray/2;
            end = sizeOfArray;
            int sum1 = 0;
            for(size_t i=start; i < end; i++){
                sum1 = sum1 + arr[i];
            }
            close(fd[0]);
           if(write(fd[1], &sum1,sizeof(sum1)) == -1){
            std::cout << "error with write " << std::endl;
            return 3;
           }  close(fd[1]);
           std::cout << "child output is " << sum1 << std::endl;
        }
        
     }
     else{
        if(id1 == 0){
            start = 0;
            end = sizeOfArray/2;
            int sum1 = 0;
            for(size_t i=start; i < end; i++){
                sum1 = sum1 + arr[i]; }
            close(f[0]);
           if(write(f[1], &sum1,sizeof(sum1)) == -1){
            std::cout << "error with write " << std::endl;
            return 3;
           }
           std::cout << "chid one output is " << sum1 << std::endl;
            close(f[1]);
        }
        else{
            int x, y;
            close(fd[1]);
            close(f[1]);
            if(read(fd[0], &x, sizeof(int))== -1){
                std::cout << "error with read" << std::endl;
            }
            if(read(f[0], &y, sizeof(int))== -1){
                std::cout << "error with read" << std::endl;
            }
            std::cout << "sum of total is " << x + y << std::endl;
            close(fd[0]);
            close(f[0]);            
        }
     }









    return 0;
}