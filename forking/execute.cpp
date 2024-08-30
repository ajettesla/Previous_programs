#include <iostream>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    
execlp("ping", "ping", "google.com", NULL);




    return 0;
}