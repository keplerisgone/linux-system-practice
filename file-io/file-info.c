#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <PATH>\n", argv[0]);
		return 1;
	}

	struct stat sb;

	if (lstat(argv[1], &sb) == -1) {
		perror("lstat error");
		return 1;
	}

	printf("Path: %s\n", argv[1]);

	switch (sb.st_mode & S_IFMT) {
		case S_IFREG: printf("Type: regular file\n"); break;
		case S_IFDIR: printf("Type: directory\n"); break;
		case S_IFLNK: printf("Type: symbolic link\n"); break;
		default: printf("Type: other\n");
	}

	printf("Size: %jd\n", (intmax_t) sb.st_size);
	printf("Inode: %ju\n", (uintmax_t) sb.st_ino);
	printf("Links: %ju\n", (uintmax_t) sb.st_nlink);
	printf("Mode: %04o\n", (unsigned int) sb.st_mode & 0777);

	return 0;
}
