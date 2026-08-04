#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 4096

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <SOURCE> <DESTINATION>\n", argv[0]);
		return 1;
	}

	int fd_source = open(argv[1], O_RDONLY);
	if (fd_source == -1) {
		perror("mini-cp: open source");
		return 1;
	}

	int fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_dest == -1) {
		perror("mini-cp: open destination");
		if (close(fd_source) == -1) {
			perror("mini-cp: close source");
		}
		return 1;
	}

	int result = 0;
	char buf[BUF_SIZE];
	ssize_t r_result = read(fd_source, buf, BUF_SIZE);

	while (r_result > 0) {
		ssize_t w_result = write(fd_dest, buf, r_result);
		if (w_result == -1) {
			perror("mini-cp: write destination");
			result = 1;
			break;
		}
		 /* TODO: Retry write() until all bytes have been written. */
		if (w_result < r_result) {
			fprintf(stderr, "mini-cp: partial write: wrote %zd of %zd bytes\n", w_result, r_result);
			result = 1;
			break;
		}
		r_result = read(fd_source, buf, BUF_SIZE);
	}

	if (r_result == -1) {
		perror("mini-cp: read source");
		result = 1;
	}

	if (close(fd_dest) == -1) {
		perror("mini-cp: close destination");
		result = 1;
	}

	if (close(fd_source) != 0) {
		perror("mini-cp: close source");
		result = 1;
	}

	if (result == 0) {
		printf("good!: %s, %s\n", argv[1], argv[2]);
	}

	return result;
}
