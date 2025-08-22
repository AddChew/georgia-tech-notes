# C Programming

## How to compile and run C code

By convention C source code files are named with .c extension, i.e. hello.c. But during compilation, we do not specify the .c extension if using make.

```bash
# Command to compile hello.c
# Same as: gcc hello.c -o hello
make hello # output file will be hello

# Command to run hello
./hello
```

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

#### Structure Objects

```c
// -> arrow operator is used to access the values of struct attributes from the struct pointer (memory address)
// . dot operator is used to access the values of struct attributes from itself

#include<stdio.h> 

struct st{ 
int a; 
char ch; 
}; 

int main(void) 
{ 
    struct st obj; 
    struct st *stobj = &obj; 

    stobj->a = 5; 
    stobj->ch = 'a'; 

    printf("\n [%d] [%c]\n", stobj->a, stobj->ch); 
    printf("\n [%d] [%c]\n", obj.a, obj.ch); // stobj->a == obj.a, stobj->ch == obj.ch
    return 0; 
}

// Output:  [5] [a]
// Output:  [5] [a]
```

### Linked list

Reference: https://www.thegeekstuff.com/2012/08/c-linked-list-example/

### Date and time

Reference: https://www.w3schools.com/c/c_date_time.php

Use <time.h> library.

### Random numbers

Reference: https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm

Use rand() function in <stdlib.h> library.

### String functions

Reference: https://en.wikibooks.org/wiki/A_Little_C_Primer/C_String_Function_Library

# C++ Programming

## How to compile and run C++ code

By convention C++ source code files are named with .cpp extension, i.e. hello.cpp. But during compilation, we do not specify the .cpp extension if using make.

```bash
# Command to compile hello.cpp
# Same as: g++ class.cpp -o class
make hello # output file will be hello

# Command to run hello
./hello
```

## Crash Course

References:
- [C++ strings](https://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/IO/Streams/string)
- [C++ OOP](https://www.w3schools.com/cpp/cpp_oop.asp)
- [C++ class](https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes)
- [C++ pointers](https://www.tutorialspoint.com/pointers-smart-pointers-and-shared-pointers-in-cplusplus)
- [C++ lock guard](https://en.cppreference.com/w/cpp/thread/lock_guard.html)

### Class Inheritance

Public: Classes which inherit with the keyword public from a base class inherit all the public members as public members. 

Protected: Inherited as protected data.

Private: Inherited but not directly accessible by the child class

```cpp
class Form {
private:
  double area;

public:
  int color;

  double getArea(){
    return this->area; // this keyword acts as a pointer to the class itself (i.e. analogous to self in Python)
  }

  void setArea(double area){
    this->area = area;
  }

};

// Child class Circle inherits private attribute area from base class Form but cannot access it directly, have to use getter and setter functions to access it
class Circle : public Form {
public:
  double getRadius() {
    double a;
    a = getArea();
    return sqrt(a / 2 * 3.14);
  }

  void setRadius(double diameter) {
    setArea( pow(diameter * 0.5, 2) * 3.14 );
  }

  bool isDark() {
    return (color > 10);
  }

};
```

#### Params in Constructor

```cpp
// Inheritance with Parent class that requires parameters in contructor
ChildClass::ChildClass(int a, int b) : ParentClass(a, b)
{
  //Child constructor here
}
```

#### Static

Use of static keyword for a data member will cause that member to be shared by all instances of the owner and derived classes. Analogous to global variable in Python.

```cpp
class Foo {
public:
  Foo() {
    ++iNumFoos;
    cout << "We have now created " << iNumFoos << " instances of the Foo class\n";
  }
private:
  static int iNumFoos;
};

int Foo::iNumFoos = 0;  // allocate memory for numFoos, and initialize it

int main() {
  Foo f1;
  Foo f2;
  Foo f3;
}
```

# Makefile

References:
- https://web.archive.org/web/20200706225945/http://mrbook.org/blog/tutorials/make/
- https://web.archive.org/web/20200706225945/http://www.gnu.org/software/make/manual/make.html

By default, make command will look for a file named makefile in current directory and execute it.

```bash
# Use this command if you have several makefiles or makefile with a different name
# make -f <name of make file>
make -f MyMakefile
```

## Build Process

1. Compiler takes the source files and outputs object files
2. Linker takes the object files and creates an executable

## Basic makefile

```bash
# Manual compiling
g++ main.cpp hello.cpp factorial.cpp -o hello
```

Basic makefile is composed of:
```Makefile
target: dependencies
    system command
```

In the example below, our target is called all, which is the default target for makefiles. make will execute this target if none is specified. There are no dependencies for target all in this example.
```Makefile
# Save to Makefile-1
all:
    g++ main.cpp hello.cpp factorial.cpp -o hello
```

To run makefile
```bash
make -f Makefile-1
```

## Using dependencies

Using different targets allows us to recompile only the files that we modified, instead of recompiling everything.

Now we see that the target all has only dependencies, but no system commands. In order for make to execute correctly, it has to meet all the dependencies of the called target (in this case all).

Each of the dependencies are searched through all the targets available and executed if found.

Useful to include clean target for a fast way to get rid of all the object files and executables.

```Makefile
all: hello

hello: main.o factorial.o hello.o
    g++ main.o factorial.o hello.o -o hello

main.o: main.cpp
    g++ -c main.cpp

factorial.o: factorial.cpp
    g++ -c factorial.cpp

hello.o: hello.cpp
    g++ -c hello.cpp

clean:
    rm *o hello
```

```bash
# Sample commands
make -f Makefile-2 main.o
make -f Makefile-2 hello.o
make -f Makefile-2 clean
```

## Using variables and comments

```Makefile
# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all: hello

hello: main.o factorial.o hello.o
    $(CC) main.o factorial.o hello.o -o hello

main.o: main.cpp
    $(CC) $(CFLAGS) main.cpp

factorial.o: factorial.cpp
    $(CC) $(CFLAGS) factorial.cpp

hello.o: hello.cpp
    $(CC) $(CFLAGS) hello.cpp

clean:
    rm *o hello
```