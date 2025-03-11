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

// Simplified DNS header structure
struct dns_header {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};


// Function to create a DNS response
void create_dns_response(char *buffer, int *length) {
	struct dns_header *header = (struct dns_header *)buffer;

	// Set DNS header fields
	header->flags = htons(0x8180); // Standard query response, no error
	header->ancount = htons(1); // One answer

	// Add the answer section 
	char *answer = buffer + *length;
	// char *answer = buffer + 12 + (12 + 1 + 2 + 2);

	// Name: Pointer to the query name
	answer[0] = 0xC0; // Pointer flag
	answer[1] = 0x0C; // Offset to the query name in the packet

	// Type: A record (host address)
	answer[2] = 0x00; // High byte of type
	answer[3] = 0x01; // Low byte of type (A record)

	// Class: IN (Internet)
	answer[4] = 0x00; // High byte of class
	answer[5] = 0x01; // Low byte of class (IN)
	
	// TTL: Time to live (60 seconds)
	answer[6] = 0x00; // Byte 1 of TTL
	answer[7] = 0x00; // Byte 2 of TTL
	answer[8] = 0x00; // Byte 3 of TTL
	answer[9] = 0x3C; // Byte 4 of TTL (60 seconds)
	
	// Data Length: 4 bytes (IPv4 address)
	answer[10] = 0x00; // High byte of data length
	answer[11] = 0x04; // Low byte of data length (4 bytes)
    
	// Data: IP address (192.168.1.1)
	answer[12] = 192; // Byte 1 of IP address
	answer[13] = 168; // Byte 2 of IP address
	answer[14] = 1;   // Byte 3 of IP address
	answer[15] = 1;   // Byte 4 of IP address

	// Update the total length of the DNS response
	*length = (12 + (12 + 1 + 2 + 2)) + 16; // Add the length of the answer section (16 bytes)
}

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
		
		printf("--------------------------------------\n");
		printf("Port de l'emetteur: %d\n",ntohs(client_addr.sin_port));
		printf("Adresse de l'emetteur: %s\n",inet_ntoa(client_addr.sin_addr));
		
		// Print the query name (for debugging)
		char *query_name = buffer + sizeof(struct dns_header);
		printf("Received DNS query for: %s\n", query_name);
		for (int i = 0; i < bytes_received; i++)
			printf("%hhx ", buffer[i]);
		printf("Bytes received from query %d\n", bytes_received);
		
		// Create a DNS response
		int response_length = 12 + (strlen(query_name) + 1 + 2 + 2); // Headers + Query
		create_dns_response(buffer, &response_length);
		
		for (int i = 0; i < response_length; i++)
			printf("%hhx ", buffer[i]);
			
		printf("\n");
		
		// Send the DNS response back to the client
		printf("Bytes send from response %d\n", response_length);
		if (sendto(sockfd, buffer, response_length, 0, (struct sockaddr *) &client_addr, client_len) < 0)
			perror("sendto failed");
	}
	
	close(sockfd);

	return 0;
}
