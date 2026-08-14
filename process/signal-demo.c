#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t stop_requested = 0;

void custom_handler(int signo) {
	stop_requested = signo;
}

int main(void) {
	struct sigaction action = {0};
	action.sa_handler = custom_handler;
	if (sigemptyset(&action.sa_mask) == -1) {
		perror("sigemptyset");
		return 1;
	}
	action.sa_flags = 0;

	if (sigaction(SIGINT, &action, NULL) == -1) {
		perror("sigaction - SIGINT");
		return 1;
	}
	if (sigaction(SIGTERM, &action, NULL) == -1) {
		perror("sigaction - SIGTERM");
		return 1;
	}

	pid_t pid = getpid();
	printf("PID: %jd\n", (intmax_t) pid);

	while (stop_requested == 0) {
		sleep(1);
	}

	printf("received signal %d, shutting down...\n", (int)stop_requested);

	return 0;
}
