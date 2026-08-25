#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

struct info {
	int number;
	int input;
	int result;
};

void *worker(void *arg) {
	struct info *info_p = arg;
	printf("worker: %d, input: %d\n", info_p->number, info_p->input);

	info_p->result = info_p->input * 2;
	sleep(3);

	printf("worker %d done\n", info_p->number);
	return NULL;
}

int main(void) {
	pthread_t p_array[2];
	struct info info_array[2];
	info_array[0] = (struct info) {1, 10, 0};
	info_array[1] = (struct info) {2, 20, 0};

	int create_cnt = 0;
	for (int i=0; i<2 ; i++) {
		int err = pthread_create(&p_array[i], NULL, worker, &info_array[i]);
		if (err != 0) {
			fprintf(stderr, "pthread_create: %s\n", strerror(err));
			break;
		}
		create_cnt++;
	}

	for (int i=0; i<create_cnt; i++) {
		int err = pthread_join(p_array[i], NULL);
		if (err != 0) {
			fprintf(stderr, "pthread_join: %s\n", strerror(err));
			// TODO: Join the remaining created threads before returning.
			return 1;
		}
	}

	// TODO: Return failure without printing results if not all threads were created.
	printf("worker 1 result: %d\nworker 2 result: %d\n", info_array[0].result, info_array[1].result);

	return 0;
}
