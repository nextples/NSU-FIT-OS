#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void udp_client() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int i = 0;
    const char* message = "Hello World!";
    while (1) {
        sendto(sockfd, message, strlen(message), 0,
               (const struct sockaddr*) &server_addr, addr_len);
        printf("Sent message %d: %s\n", i+1, message);

        ssize_t len = recvfrom(sockfd, (char*) buffer, BUFFER_SIZE, 0, (struct sockaddr*) &server_addr, &addr_len);
        buffer[len] = '\0';
        printf("Received echo %d: %s\n", i+1, buffer);

        sleep(1);
        i++;
    }

    close(sockfd);
}

int main() {
    udp_client();
    return 0;
}
