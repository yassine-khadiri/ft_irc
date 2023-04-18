#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 6667
#define MAX_CLIENTS 30

int main(int argc, char *argv[]) {
    int master_socket, new_socket, client_socket[MAX_CLIENTS], max_clients = MAX_CLIENTS;
    struct sockaddr_in server, client;
    fd_set readfds;
    char buffer[1024];
    int activity, i, valread, sd;

    // Create a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket to the server address
    if (bind(master_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(master_socket, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections and add the new sockets to the array
    int addrlen = sizeof(client);
    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add the master socket to the set
        FD_SET(master_socket, &readfds);

        // Add all the client sockets to the set
        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
        }

        // Wait for activity on any socket
        activity = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno!=EINTR)) {
            printf("select error");
        }

        // If activity is on the master socket, it means a new client is attempting to connect
        if (FD_ISSET(master_socket, &readfds)) {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&client, (socklen_t*)&addrlen)) < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }

            // Add the new socket to the array of client sockets
            for (i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        // If activity is on any of the client
    }