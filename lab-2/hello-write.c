#include <stdio.h>
#include <unistd.h>

int main(){
    write(1,"Hello world!\n",130);
    return 0;
}
