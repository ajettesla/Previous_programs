#include <stdio.h>


int main(){

int a=732;
int b=2684;
while(a != b) {
  if(a > b) {
    a=a-b;
  } else {
    b=b-a;
  }
}

printf("a value is %d, b %d ", a, b);

return 0;

}
