#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_SIZE 4096
static const char shm_name[] = "/kepp_shm_demo";

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <message>\n", argv[0]);
		return 1;
	}

	size_t message_len = strlen(argv[1]);
	if (message_len >= SHM_SIZE) {
		fprintf(stderr, "message is too long\n");
		return 1;
	}

	int fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
	if (fd == -1) {
		perror("shm_open");
		return 1;
	}

	if (ftruncate(fd, SHM_SIZE) == -1) {
		perror("ftruncate");
		close(fd);
		shm_unlink(shm_name);
		return 1;
	}

	char *mmap_result = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mmap_result == MAP_FAILED) {
		perror("mmap");
		close(fd);
		shm_unlink(shm_name);
		return 1;
	}

	if (close(fd) == -1) {
		perror("close");
		munmap(mmap_result, SHM_SIZE);
		shm_unlink(shm_name);
		return 1;
	}

	memcpy(mmap_result, argv[1], message_len + 1);
	if (munmap(mmap_result, SHM_SIZE) == -1) {
		perror("munmap");
		shm_unlink(shm_name);
		return 1;
	}

	return 0;
}
