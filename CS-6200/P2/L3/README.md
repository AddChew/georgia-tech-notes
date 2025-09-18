# P2L3

## Threads Case Study: Pthreads Library in C

### Thread Data Type
```c
pthread_t aThread;
```

### Fork

```c
int pthread_create(pthread_t *thread, const pthread_attr *attr, void * (*start_routine)(void *), void *arg);
// pthread_attr defines features of new thread
// has default behaviour when set to NULL in pthread_create

int pthread_attr_init(pthread_attr_t *attr); // create initialize attr data structure
int pthread_attr_destroy(pthread_attr_t *attr); // destroy + free attr data structure
pthread_attr_set(attr); // set value of attr field
pthread_attr_get(attr); // get value of attr field
```

### Join
```c
int pthread_join(pthread_t thread, void **status);
```

### Detach

pthread allows for detached child threads which is independent of the parent thread, child can continue executing even when parent thread has exited. child = parent thread.
```c
int pthread_detach(); // create detached thread

pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED); // set thread attr to detach
pthread_create(..., attr) // By default, threads are created in joined mode, setting detach state attr, will create thread in detached mode

void pthread_exit() //  exit thread
```

Sample code snippet

```c
#include <stdio.h>
#include <pthread.h> // must include in main file !!!

void *foo (void *arg) {
    printf("Foobar!\n");
    pthread_exit(NULL);
}

int main(void) {
    int i;
    pthread_t tid;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_create(&tid, &attr, foo, NULL);

    return 0;
}
```

### Compiling PThreads

1. Include header
```c
#include <pthread.h>
```

2. Compile source with -lpthread or -pthread
```bash
gcc -o main main.c -lpthread
gcc -o main main.c -pthread
```

3. Check return value of common functions