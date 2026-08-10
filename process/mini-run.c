#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s <command>\n", argv[0]);
		return 1;
	}
	

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		// child
		execvp(argv[1], &argv[1]);
		perror(argv[1]);
		_exit(127);
	}

	// parent
	int status;
	pid_t waited_pid = waitpid(pid, &status, 0);
	if (waited_pid == -1) {
		perror("waitpid");
		return 1;
	}
	if (WIFEXITED(status)) {
		int exit_code = WEXITSTATUS(status);
		printf("Parent: child %jd exited with status %d\n", (intmax_t) pid, exit_code);
		return exit_code;
	}
	if (WIFSIGNALED(status)) {
		int signal = WTERMSIG(status);
		printf("Parent: child %jd terminated by signal %d\n", (intmax_t) pid, signal);
		return (128 + signal);
	}

	fprintf(stderr, "unexpected  child status\n");
	return 1;
}
