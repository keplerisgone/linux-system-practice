#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <fifo_path> <message>\n", argv[0]);
		return 1;
	}

	printf("waiting for reader...\n");
	fflush(stdout);

	int fd = open(argv[1], O_WRONLY);
	if (fd == -1) {
		perror("open");
		return 1;
	}
	printf("connected to reader\n");
	fflush(stdout);

	ssize_t w_result = write(fd, argv[2], strlen(argv[2]));
	if (w_result == -1) {
		perror("write");
		close(fd);
		return 1;
	} else if (w_result < (ssize_t)strlen(argv[2])) {
		fprintf(stderr, "partial write\n");
		close(fd);
		return 1;
	}

	close(fd);
	return 0;
}
