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

int main() {
	char * hello = "hello from client";
	 
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
	 
	if (sendto(sockfd, (const char *) hello, strlen(hello), 0, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
		perror("failed to send");
	
	close(sockfd);

	return 0;
}
