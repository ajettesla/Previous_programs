#include <iostream>
#include <unistd.h>
#include <queue>

int main(){
    std::queue<int> myq;


    myq.push(10);
    myq.push(20);
    myq.push(30);


    while(!myq.empty()){
    std::cout << myq.front() << std::endl;
    myq.pop();
    std::cout << myq.size() << std::endl;
    sleep(1);
    }

return 0;
}