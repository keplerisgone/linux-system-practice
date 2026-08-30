#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9090);
	
	int buffer;
	int r = inet_pton(AF_INET, "127.0.0.1", &buffer);

	printf("socket: %d\n", server_fd);
	
	if (close(server_fd) == -1) {
		perror("close");
		return 1;
	}
	return 0;
}
