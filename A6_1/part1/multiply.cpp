#include <stdio.h> 
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
        {  
            cout << "Error: number of input arguments not enough \n";
            return -1;
        }
        int multi = atoi(argv[1]); 
        int in;
    while (1)
    {
        scanf("%d", &in);
        // cout <<  multi*in << endl;

        printf("%d\n",in*multi);
        //fflush(stdout);
    }
    return 0;
}