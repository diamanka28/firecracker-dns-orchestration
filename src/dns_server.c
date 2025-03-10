#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5354
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
    header->id = htons(0x1234); // Use the same ID as the query
    header->flags = htons(0x8180); // Standard query response, no error
    header->qdcount = htons(1); // One question
    header->ancount = htons(1); // One answer

    // Add the question section (hardcoded for "example.com")
    char *question = buffer + sizeof(struct dns_header);
    question[0] = 7; // Length of "example"
    memcpy(question + 1, "example", 7);
    question[8] = 3; // Length of "com"
    memcpy(question + 9, "com", 3);
    question[12] = 0; // Null-terminate the domain name
    question[13] = 0x00; // Type A (host address)
    question[14] = 0x01; // Class IN (Internet)

    // Add the answer section (hardcoded for "example.com")
    char *answer = question + 15;
    answer[0] = 0xC0; // Pointer to the query name
    answer[1] = 0x0C; // Offset to the query name in the packet
    answer[2] = 0x00; // Type A (host address)
    answer[3] = 0x01; // Class IN (Internet)
    answer[4] = 0x00; // TTL (Time to live)
    answer[5] = 0x00;
    answer[6] = 0x00;
    answer[7] = 0x3C; // 60 seconds
    answer[8] = 0x00; // Data length (4 bytes for IPv4)
    answer[9] = 0x04;
    answer[10] = 192; // IP address (192.168.1.1)
    answer[11] = 168;
    answer[12] = 1;
    answer[13] = 1;

    // Calculate the total length of the DNS response
    *length = sizeof(struct dns_header) + 15 + 14;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("DNS server is running on port %d...\n", PORT);

    while (1) {
        // Receive a DNS query
        bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                  (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received < 0) {
            perror("recvfrom failed");
            continue;
        }

        printf("Received DNS query\n");

        // Create a DNS response
        int response_length;
        create_dns_response(buffer, &response_length);

        // Send the DNS response back to the client
        if (sendto(sockfd, buffer, response_length, 0,
                   (struct sockaddr *)&client_addr, client_len) < 0) {
            perror("sendto failed");
        }
    }

    close(sockfd);
    return 0;
}
