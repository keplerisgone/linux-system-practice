#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
	pid_t pid = getpid();
	printf("pid: %jd\n", (intmax_t) pid);

	fflush(stdout);
	execlp("sleep", "sleep", "30", (char *) NULL);
	perror("execlp");

	return 1;
}
