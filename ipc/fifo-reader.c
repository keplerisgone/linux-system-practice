#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <path>\n", argv[0]);
		return 1;
	}

	printf("wait for writer\n");
	fflush(stdout);

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open");
		return 1;
	}
	printf("connected to writer\n");
	fflush(stdout);

	char buffer[128];
	while (1) {
		ssize_t bytes_read = read(fd, buffer, sizeof(buffer)-1);
		if (bytes_read > 0) {
			buffer[bytes_read] = '\0';
			printf("%s", buffer);
			fflush(stdout);
		} else if (bytes_read == 0) {
			// EOF
			printf("\n");
			break;
		} else if (bytes_read == -1 && errno == EINTR) {
			continue;
		} else {
			perror("read");
			close(fd);
			return 1;
		}
	}

	close(fd);
	return 0;
}
