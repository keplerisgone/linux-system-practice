#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define WAIT_SECONDS 60

int main(void){
	pid_t pid;
	pid_t ppid;

	pid = getpid();
	ppid = getppid();

	printf("PID: %jd\n", (intmax_t) pid);
	printf("PPID: %jd\n", (intmax_t) ppid);

	printf("Observing for %d seconds...\n", WAIT_SECONDS);
	unsigned s_result = sleep(WAIT_SECONDS);

	if (s_result != 0) {
		fprintf(stderr, "sleep: error - left seconds: %u\n", s_result);
		return 1;
	}

	printf("finish observing\n");

	return 0;
}
