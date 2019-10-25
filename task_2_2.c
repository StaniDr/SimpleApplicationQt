#include <stdio.h>
#include <stdbool.h>


int main(int argc, char **argv)
{


   double l = 1.7E+308; 
   printf("was l: %E \n", l);
    l = l + 1; 
    printf("long is now: %E \n", l);

    return 0;


}
