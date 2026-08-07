#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	pid_t pid;

	pid = fork();

	if (pid == -1) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		// Child
		pid_t child_ppid = getppid();
		pid_t child_pid = getpid();
		printf("Child: PID=%jd, PPID=%jd, fork_result=%jd\n", (intmax_t) child_pid, (intmax_t) child_ppid, (intmax_t) pid);
		sleep(1);
		printf("Child: exiting with status 7\n");
		return 7;
	}

	// Parent
	pid_t parent = getpid();
	printf("Parent: PID=%jd, child PID=%jd\n", (intmax_t) parent, (intmax_t) pid);
	int status;
	pid_t waited_pid = waitpid(pid, &status, 0);
	if (waited_pid == -1) {
		perror("waitpid");
		return 1;
	}
	if (WIFEXITED(status)) {
		int exit_code = WEXITSTATUS(status);
		printf("Parent: child %jd exited with status %d\n", (intmax_t) pid, exit_code);
	}

	return 0;
}
