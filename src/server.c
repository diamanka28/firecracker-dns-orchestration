#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	// Create UDP socket
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("failed to create socket");
		exit(EXIT_FAILURE);
	}
	
	// Configure server address
	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	// Bind the socket to the server address
	if (bind(sockfd, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
		perror("failed to bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	
	printf("DNS server is running on port %d...\n", ntohs(server_addr.sin_port));
	
	// Waiting for requests
	char buffer[BUFFER_SIZE] = {0};
	int bytes_received;
	while (1) {
		bytes_received = recvfrom(sockfd, (char *) buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_addr, &client_len);
		buffer[bytes_received] = '\n';
		printf("%s", buffer);
	}
	
	close(sockfd);

	return 0;
}
