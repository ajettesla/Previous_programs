#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>


int main(int argc, char* argv[]){
int sum {0};
int fd = open("myfifo", O_RDONLY);
for(size_t i=0; i<5;i++){
int x;
if (read(fd, &x , sizeof(x)) == -1){
    std::cout << "error with write function" << std::endl;
    return 2;
}
sum = sum + x;
}
close(fd);
fd = open("myfifo", O_WRONLY);
if(write(fd, &sum, O_WRONLY)== -1){
    std::cout << "error while writing to file " << std::endl;
}
close(fd);


return 0;}
