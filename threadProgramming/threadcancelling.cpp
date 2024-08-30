#include <iostream>
#include <pthread.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

void* closefn(void*f){
    fclose((FILE *)f);
    return nullptr;
}

void* callbackwrite(void *filename){
    const char *fi = static_cast<const char*>(filename);
    FILE *file = fopen(fi, "w");
    if(file == nullptr){
        perror("error with file opening");
    }

   

    int count = 0;
    while(1){
        fprintf(file ,"the file is writing an line number is %d.\n",count);
        fflush(file);
        sleep(1);
        count++;
    }


    return nullptr;
    }

pthread_t create_thread(char *file){
    pthread_t writehandel;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,0);
    char *filename = new char[30];
    strcpy(filename, file);
    pthread_create(&writehandel, NULL, callbackwrite,filename);

    return writehandel;
}


int main(int argc, char** argv){
std::cout << "Number of thread do you want to start " << std::endl;
int numberofthread {0};
std::vector<pthread_t> vect;

std::cin >> numberofthread;
for(int j=0; j < numberofthread; j++){
    char filename[30];
    sprintf(filename,"ajet_%d.txt",j);
    vect.push_back(create_thread(filename));
}
while(1){
    std::cout << "enter thread to cancel" << std::endl;
    int num {0};
    std::cin >> num;
    pthread_t han = vect[num];
    pthread_cancel(han);
    std::cout << "cancelling the thread " << num << " is successful." << std::endl;
}


}