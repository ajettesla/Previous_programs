#include <iostream>
#include <unistd.h>

int main(){

   FILE *file = fopen("ajet.txt", "w");
   while(1){
   fprintf(file,"hallo World");
   
   sleep(1);
   }

    return 0;
}