#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <threads.h>

#define THREAD_COUNT 4
const long long ITERATIONS = 1000000;
static long long counter = 0;

pthread_mutex_t mutex;

void *worker(void *arg) {
	(void) arg;
	for (int i=0; i<ITERATIONS; i++) {
		pthread_mutex_lock(&mutex);
		counter++;
		pthread_mutex_unlock(&mutex);
	}
	printf("worker done\n");
	return NULL;
}

int main(void) {
	pthread_t threads[THREAD_COUNT];

	int create_cnt = 0;
	pthread_mutex_init(&mutex, NULL);
	for (int i=0; i<THREAD_COUNT; i++) {
		int err = pthread_create(&threads[i], NULL, worker, NULL);
		if (err != 0) {
			fprintf(stderr, "pthread_create: %s\n", strerror(err));
			break;
		}
		create_cnt++;
	}

	for (int i=0; i<create_cnt; i++) {
		int err = pthread_join(threads[i], NULL);
		if (err != 0) {
			fprintf(stderr, "pthread_join: %s\n", strerror(err));
		}
	}

	pthread_mutex_destroy(&mutex);

	printf("expected: %lld, actual: %lld\n", THREAD_COUNT * ITERATIONS, counter);
	return 0;
}
