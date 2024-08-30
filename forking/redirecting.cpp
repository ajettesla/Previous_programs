#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main(int arg, char* argv[]){

//    char buffer[30];
//    if(read(0,buffer,sizeof(buffer)) == -1){
//     std::cout << "error while reading the file " << std::endl;
//    }

//    if(write(1,buffer,sizeof(buffer)) == -1){
//     std::cout << "error while writing to file " << std::endl;
//    }


   int fo = open("myfile.txt",O_WRONLY|O_CREAT, 0777);
   if(fo == -1){
    std::cout << "unable to open file " << std::endl;
   }
   std::cout << "printing results" << std::endl;
   dup2(fo, STDOUT_FILENO);
   execlp("ping", "ping", "google.com", NULL);

   
    return 0;
}