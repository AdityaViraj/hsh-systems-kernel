#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // A standard variable on stack
    int secret_code = 42;

    //Pointer 
    int *target_pointer=&secret_code;

    //String
    char shell_name[]="hsh";

    printf("--- THE PHYSICAL MEMORY MAP ---\n");

    //Using %p to print the raw Hexadecimal memory address
    printf("1. The value of 'secret_code' is:%d\n",secret_code);
    printf(" The physical address of 'secret_code is: %p\n",(void*)&secret_code);

    printf("2. The value inside 'target_pointer' is: %p\n",(void*)target_pointer);
    printf(" Notice how it matches to the address above exactly!\n");
    printf(" If we 'dereference' the pointer (*target_pointer), we get the value:%d\n\n" , *target_pointer);

    printf("3. The string 'shell_name' starts at address :%p\n",(void*)shell_name);
    printf(" Letter 1 ('h') is at: %p\n",(void*)&shell_name[0]);
    printf(" Letter 2 ('s') is exactly 1 byte later at: %p\n",(void*)&shell_name[1]);

    return 0;

}