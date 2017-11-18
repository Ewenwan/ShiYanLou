#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
   // # Celsius degree
    int input = 0;

   // # paramter number must bu one
    if( argc != 2)
        return 1;

   // # argv[0] is executive program ,argv[1] is first command line paramter
    input = atoi(argv[1]);

    float output_f=input*1.8+32.0;
    printf("%.1f",output_f);
    return 0;
}
