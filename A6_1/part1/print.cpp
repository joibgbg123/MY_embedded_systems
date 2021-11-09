#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    int counter = 0;
    while (1)
    {
        sleep(1);
        counter += 1;
        printf("%d\n",counter);
        fflush(stdout);
        //cout.flush();
    }
    return 0;
}