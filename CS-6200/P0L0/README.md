# Lecture 0

## Crash Course

### Pointers

Reference: https://www.thegeekstuff.com/2011/12/c-pointers-fundamentals/

Pointers = special variables that store the memory address of another variable

```c
// General syntax for pointer declaration
<pointer type> *<pointer name>;

// pointer type = type of variable whose address this pointer can store
// pointer name = name of pointer variable

// Sample declaration
char *chptr;
```

```c
// General syntax for pointer initialization
<pointer declaration> = <address of variable>

char c = 'c';
char *chptr = &c; // type of variable should match pointer type

// & operator is used to access the address of the variable 
```

```c
// How to access the value of a variable from its pointer (memory address)
// * operator when used on a pointer, allows us to get the value of the variable whose address is stored on the pointer
char t
char c = 'c'
char *chptr = &c
t = *chptr // *chptr == c == 'c'
```

#### String

String = array of characters. There is no string pointer in C. Character pointers are used for string.

When we point a pointer to a string, by default the address of the first character of the string is stored in the pointer.

Null character \0 is placed at the end of every string in C, to signify the end of the string

```c
#include <stdio.h> 

int main(void) 
{ 
    char *ptr = "I am a string"; 

    printf("\n [%c], [%s]\n", *ptr, ptr); // *ptr == I

    return 0; 
}

// Output: [I], [I am a string]
// If we specify the format specifier as '%s' and use 'ptr' (which contains the starting address of the string), then the complete string is printed using printf.
```

### Linked list

Reference: https://www.thegeekstuff.com/2012/08/c-linked-list-example/

## How to compile C code

By convention C source code files are named with .c extension, i.e. hello.c. But during compilation, we do not specify the .c extension.

```bash
# Command to compile hello.c
make hello # output file will be hello

# Command to run hello
./hello
```