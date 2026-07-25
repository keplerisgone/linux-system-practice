#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: ./mini-cat [file path]\n");
		return 1;
	}

	int fd;

	// open 및 에러 검사 (출력이 -1)
	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "error: file open\n");
		return 1;
	}
	
	// buf & read result;
	char buf[BUF_SIZE];
	ssize_t result_r = read(fd, buf, BUF_SIZE);
	ssize_t result_w;

	while(result_r > 0) {
		result_w = write(STDOUT_FILENO, buf, result_r);
		if (result_w == -1) {
			fprintf(stderr, "error: file write\n");
			close(fd);
			return 1;
		}
		result_r = read(fd, buf, BUF_SIZE);
        }

	if (result_r == -1) {
		fprintf(stderr, "error: file read\n");
		close(fd);
		return 1;
	}

	
	if (close(fd) == 0) {
		return 0;
	} else { 
		fprintf(stderr, "file r/w failed\n");
		return 1;
	}
}

