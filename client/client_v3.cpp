#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../logicpart/logic.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8082

int client() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create client socket");
        return 1;
    }

    // Prepare server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        perror("Failed to convert server IP address");
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to connect to the server");
        return 1;
    }

    printf("Connected to the server.\n");
    // interfaceTest();

    while(1) {
        printf("Enter message: ");
        fgets(buffer, strlen(buffer) - 1, stdin);
        send(client_socket, buffer, strlen(buffer) - 1, 0);

        // Получение ответа от сервера
        int bytes_received = recv(client_socket, buffer, strlen(buffer) - 1, 0);
        if (bytes_received > 0) {
            printf("Received message: %s\n", buffer);
        }
    }


    // Close the client socket
    close(client_socket);

    return 0;
}