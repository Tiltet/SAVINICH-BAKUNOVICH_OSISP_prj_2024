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

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Wait for the player's turn
        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
            perror("Failed to receive data from the server");
            break;
        }

        printf("Received from server: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
        // Take input from the player
        printf("Enter your move: ");
        //fgets(buffer, sizeof(buffer), stdin);
        //Отправка координат выстрела
        sendShotCoordinates(client_socket);

        // Send the move to the server
        ssize_t sent = send(client_socket, buffer, strlen(buffer), 0);
        if (sent < 0) {
            perror("Failed to send data to the server");
            break;
        }
    }

    // Close the client socket
    close(client_socket);

    return 0;
}