#include <stdio.h> 

int main(void) 
{ 
    char *ptr = "I am a string"; 

    printf("\n [%c], [%s]\n", *ptr, ptr);

    return 0; 
}