#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8082
#define MAX_CLIENTS 2

struct client_info {
    int socket;
    struct sockaddr_in address;
    int player_number;
};

struct game_state {
    int current_player;
    int player1_socket;
    int player2_socket;
    int player1_turn;
    int player2_turn;
};

void *handle_client(void *arg) {
    struct game_state *game = (struct game_state *)arg;
    int client_socket = game->current_player == 1 ? game->player1_socket : game->player2_socket;
    int other_player_socket = game->current_player == 1 ? game->player2_socket : game->player1_socket;

    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));

//        // Wait for the current player's turn
//        while (!client_turn) {
//            // Sleep to reduce CPU usage
//            //usleep(1000);
//        }
        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
            perror("Failed to receive data from the client");
            break;
        }

        printf("Received from player %d: %s\n", game->current_player, buffer);

        ssize_t sent = send(other_player_socket, buffer, strlen(buffer), 0);
        if (sent < 0) {
            perror("Failed to send data to the client");
            break;
        }

        //game->current_player = game->current_player == 1 ? 2 : 1;

//        if (game->current_player == 1) {
//            game->current_player = 2;
//            game->player1_turn = 0;
//            game->player2_turn = 1;
//        } else {
//            game->current_player = 1;
//            game->player1_turn = 1;
//            game->player2_turn = 0;
//        }
    }

    close(client_socket);

    return NULL;
}

int server_v3() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    pthread_t tid;
    struct client_info clients[MAX_CLIENTS];
    struct game_state game;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create server socket");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind the server socket");
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Failed to listen for client connections");
        return 1;
    }

    printf("Server started. Waiting for clients...\n");

    game.current_player = 1;
    game.player1_socket = -1;
    game.player2_socket = -1;
    game.player1_turn = 1;
    game.player2_turn = 0;


    ssize_t sent1;
    ssize_t sent2;
    while (1) {
        // Accept a client connection
        socklen_t client_address_len = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_len);
        if (client_socket < 0) {
            perror("Failed to accept client connection");
            continue;
        }

        printf("Client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Create a new thread to handle the client
        pthread_create(&tid, NULL, handle_client, &game);

        // Add the client to the game state
        if (game.player1_socket == -1) {
            game.player1_socket = client_socket;
            clients[0].socket = client_socket;
            clients[0].address = client_address;
            clients[0].player_number = 1;
        } else if (game.player2_socket == -1) {
            game.player2_socket = client_socket;
            clients[1].socket = client_socket;
            clients[1].address = client_address;
            clients[1].player_number = 2;
        } else {
            printf("Maximum number of clients reached. Closing connection: %s:%d\n",
                   inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            close(client_socket);
        }

        // Check if both clients have connected
        if (game.player1_socket != -1 && game.player2_socket != -1) {
            printf("Both players have connected. Let the game begin!\n");
//            game.player1_turn = 1;
//            game.player2_turn = 0;
            // Send the initial message to both players
            char initial_message[] = "Game starts. Other Player turn.";
            char start_message[] = "Your turn";
            if (game.current_player == 1) {
                sent1 = send(game.player1_socket, start_message, strlen(start_message), 0);
                sent2 = send(game.player2_socket, initial_message, strlen(initial_message), 0);
            } else {
                sent1 = send(game.player2_socket, start_message, strlen(start_message), 0);
                sent2 = send(game.player1_socket, initial_message, strlen(initial_message), 0);
            }
            if (sent1 < 0 || sent2 < 0){
                perror("Failed to send initial message to the clients");
                return 1;
            }

//            if (sent1 < 0 || sent2 < 0) {
//
          // }
        }

    }
}
            // Send the initial message toвызовите `pthread_join(tid, NULL);`, чтобы дождаться завершения потока обработки клиента. Вставьте эту строку перед закрытием клиентского сокета в функции `handle_client`.