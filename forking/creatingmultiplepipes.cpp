#include <iostream>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>



int main(int argc, char* argv[]){

    int fd1[2];
    int fd2[2];
    int fd3[2];

    if(pipe(fd1) < 0){
        std::cout << "error with pipe \n";
    }

    if(pipe(fd2) < 0){
        std::cout << "error with pipe \n";
    }

     if(pipe(fd3) < 0){
        std::cout << "error with pipe \n";
    }
   
   
   int cpid = fork();

   int ccpid = fork();

   if(cpid == 0){
       if(ccpid == 0){
        close(fd1[1]);
        close(fd2[0]);
        int x;
        if(read(fd1[0], &x, sizeof(int)) < 0){
            std::cout << "error with read" << std::endl;
        }
        x += 5;
        if(write(fd2[1], &x, sizeof(int)) < 0){
            std::cout << "error with write " << std::endl;
        }
        close(fd1[0]);
        close(fd2[1]);
       }
       else{
        close(fd2[1]);
        close(fd3[0]);
        int x;
        if(read(fd2[0], &x, sizeof(int)) < 0){
            std::cout << "error with read" << std::endl;
        }
        x += 5;
        if(write(fd3[1], &x, sizeof(int)) < 0){
            std::cout << "error with write " << std::endl;
        }
        close(fd2[0]);
        close(fd3[1]);
       }

   }

   else{
        if(ccpid != 0){
        close(fd1[0]);
        int x = 0;
        if(write(fd1[1], &x, sizeof(int)) < 0){
            std::cout << "error with write " << std::endl;
        }
        close(fd1[0]);

        close(fd1[1]);

        close(fd3[1]);
        int y;
        if(read(fd3[0], &y, sizeof(int))< 0){
            std::cout << "error with read " << std::endl;
        }
        close(fd3[0]);
        std::cout << "value of y is " << y << std::endl;
        wait(NULL);        
       }
       else{
        std::exit(0);
       }}

    return 0;
}