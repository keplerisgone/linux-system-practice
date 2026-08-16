#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(void) {
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe");
		return 1;
	}

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return 1;
	}

	// child
	if (pid == 0) {
		close(pipefd[1]);
		// read from pipe
		char buffer[128];
		ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer)-1);
		while (1) {
			if (bytes_read > 0) {
				buffer[bytes_read] = '\0';
				printf("Child received: %s\n", buffer);
				bytes_read = read(pipefd[0], buffer, sizeof(buffer)-1);
			} else if (bytes_read == 0) {
				printf("Child: EOF\n");
				break;
			} else if (bytes_read == -1 && errno == EINTR) {
				bytes_read = read(pipefd[0], buffer, sizeof(buffer)-1);
			} else {
				perror("read");
				close(pipefd[0]);
				return 1;
			}
		}
		close(pipefd[0]);
		return 0;
	}

	// parent
	int result = 0;
	close(pipefd[0]);
	const char message[] = "hello from parent";
	ssize_t bytes_write = write(pipefd[1], message, sizeof(message)-1);
	if (bytes_write == -1) {
		perror("write");
		result = 1;
	} else if (bytes_write < (ssize_t) (sizeof(message)-1)) {
		fprintf(stderr, "partial write\n");
		result = 1;
	}
	close(pipefd[1]);

	int status;
	pid_t waited_pid = waitpid(pid, &status, 0);
	if (waited_pid == -1) {
		perror("waitpid");
		return 1;
	}
	if (WIFEXITED(status)) {
		int child_result = WEXITSTATUS(status);
		printf("Parent: child %jd exited with status %d\n", (intmax_t) pid, child_result);
		result = (child_result != 0) ? child_result : result;
	} else if (WIFSIGNALED(status)) {
		int term_signal = WTERMSIG(status);
		printf("Parent: child %jd terminated by signal %d\n", (intmax_t) pid, term_signal);
		result = 128 + term_signal;
	}
	
	return result;
}	
