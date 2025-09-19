#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_READ_WRITES 10

int delay;
int num_reads = 0;
int num_writes = 0;
int resource_counter = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;  	/* mutex lock for buffer */
pthread_cond_t c_reader = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_writer = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

void *reader(void *param) {
	while(1){
		pthread_mutex_lock (&m);
			if (num_reads >= NUM_READ_WRITES) {
				printf("reader quiting\n");
				pthread_mutex_unlock (&m);
				return 0;
			}
			while (resource_counter < 0) {
				pthread_cond_wait (&c_reader, &m);
			}
			resource_counter++;
		pthread_mutex_unlock (&m);

		printf("read value: %d\n", num_writes);
		printf("num readers: %d\n", resource_counter);

		pthread_mutex_lock (&m);
			resource_counter--;
			num_reads++;
			if (resource_counter == 0) {
				pthread_cond_signal (&c_writer);
			}
		pthread_mutex_unlock (&m);
		pthread_cond_broadcast(&c_reader);

		delay = (rand() % 1000000) + 100000;
		usleep(delay);
	}
	return 0;
}

void *writer(void *param) {
	while(1){
		pthread_mutex_lock (&m);
			if (num_writes >= NUM_READ_WRITES) {
				printf("writer quiting\n");
				pthread_mutex_unlock (&m);
				return 0;
			}
			while (resource_counter != 0) {
				pthread_cond_wait (&c_writer, &m);
			}
			resource_counter = -1;
		pthread_mutex_unlock (&m);

		num_writes++;
		printf("write value: %d\n", num_writes); // TODO: add random sleep time
		printf("num readers: %d\n", resource_counter);

		pthread_mutex_lock (&m);
			resource_counter = 0;
		pthread_mutex_unlock (&m);
		
		pthread_cond_signal (&c_writer);
		pthread_cond_broadcast(&c_reader);

		delay = (rand() % 1000000) + 100000;
		usleep(delay);
	}
	return 0;
}

int main(int argc, char *argv[]) {
	int i;
	pthread_t tid1[NUM_THREADS], tid2[NUM_THREADS];

	srand(time(NULL));

    // Create reader threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid1[i], NULL, reader, NULL);
    }

    // Create writer threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid2[i], NULL, writer, NULL);
    }

    // Join reader threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid1[i], NULL);
    }

    // Join write threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid2[i], NULL);
    }
	printf("Parent quiting\n");

	return 0;
}