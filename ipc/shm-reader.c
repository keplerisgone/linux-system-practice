#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_SIZE 4096
static const char shm_name[] = "/kepp_shm_demo";

int main(void) {
	int fd = shm_open(shm_name, O_RDONLY, 0);
	if (fd == -1) {
		perror("shm_open");
		return 1;
	}
	
	char *mapping = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (mapping == MAP_FAILED) {
		perror("mmap");
		close(fd);
		shm_unlink(shm_name);
		return 1;
	}

	if (close(fd) == -1) {
		perror("close");
		munmap(mapping, SHM_SIZE);
		shm_unlink(shm_name);
		return 1;
	}

	printf("Reader: %s\n", mapping);
	if (munmap(mapping, SHM_SIZE) == -1) {
		perror("munmap");
		shm_unlink(shm_name);
		return 1;
	}

	if (shm_unlink(shm_name) == -1) {
		perror("shm_unlink");
		return 1;
	}

	return 0;
}
