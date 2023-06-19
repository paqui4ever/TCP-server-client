#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

// Create socket
int socketCreation(void)
{
    printf("\n Creating socket... \n");
    // Setting up the socket for TCP local connections (will be tested within localhost) using the IP protocol
    int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    // Simple error handling if/else conditional
    if (sockfd == -1) {
        printf("An error ocurred when creating the socket, please run again the program. \n");
        return 1;
        exit();
    }
    else {
        printf("Socket creation procedure successful. \n");
        return sockfd;
    }
}

// Bind the socket
int socketBinding(int sockfd)
{
    int binding;
    // Defining clientPort variable, which is the one that we will later connect to 
    int clientPort = 8888;
    // We initialize the struct socketAddress of type sockaddr_in
    struct sockaddr_in socketAddress = {0};
    // We declare the sin_family variable within the sockaddr_in struct type as AF_LOCAL (used for connections within the same host)
    socketAddress.sin_family = AF_LOCAL;
    // We declare the sin_addr.s_addr variable (a struct variable within the sin_addr struct type) as INADDR_ANY for being capable of using all available network interfaces
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // We declare the sin_port variable within the sockaddr_in struct type as clientPort
    socketAddress.sin_port = htons(clientPort);
    // For using the bind function we have to tell it which socket to use (in this case we have previously declared a sockfd variable that we defined as essential for the functionality of this function), the socket address and its size (for this we use the sizeof function).
    // We have to cast the socketAddres pointer because the bind function needs a socketaddr type struct. We declared our socketAddress with the sockaddr_in struct type for the specificty needed for our IPv4 connection but for the bind function to accept it we have to cast it to the socketaddr type.
    binding = bind(sockfd, (struct socketaddr *)&socketAddress, sizeof(socketAddress));
    // Simple error handling if/else conditional 
    if (binding < 0){
        printf("An error has ocurred when binding the socket, please run again the program. \n");
        return 1;
        exit();
    }
    else {
        printf("Socket binding procedure succesful. \n");
        return binding;
    }
}

int main (int argc, char *argv[]) {
    // We initialize both the client and server sockaddr_in structs for later use
    struct sockaddr_in client, server;
    // We initialize both the client message and the server message strings to 0
    char clientMessage [250] = {0};
    char serverMessage [150] = {0};
    // We initialize the clientLen variable to the size of the sockaddr_in struct, which is used for defining its address
    int clientLen = sizeof(struct sockaddr_in);
    // We use the previously created function to start a socket.
    int socket = socketCreation();
    // We use the listen function to start listening for incoming connections. The second parameter is the number of maximum pending connections to our socket.
    listen(socket, 3);
    printf("Listening for incoming connections...");
    for (int i = 0; i == 0; i){
        // ACCEPTING THE CONNECTION

        // We set up the accept function, that blocks the program until a connection is established. We assign a new socket descriptor to the socketToBeAccepted variable. It represents the connection to the client.
        int socketToBeAccepted = accept(socket, (struct sockaddr *)&client, (socklen_t *)&clientLen);
        if (socketToBeAccepted < 0) {
            printf("Accepting procedure failed, please run again the program. \n");
            return 1;
            exit();
        }
        else {
            printf("Accepting socket connection... \n");
            printf("Connection accepted.\n");
        }
        
        // Before receiving data from the client, we have to dedicate two separate empty memory spaces for both the server message and the client message. For this we can use the memset function from the string.h header file and set the buffer to the null character
        memset(clientMessage, "\0", sizeof(clientMessage));
        memset(serverMessage, "\0", sizeof(serverMessage));

        // RECEIVING AND SENDING DATA FROM/TO THE CLIENT

        // We use the recv function to receive data from the client. We pass the socketToBeAccepted variable as the socket we want to receive data from, then the clientMessage buffer that we previously setted up, then the maximun amount of bytes that we want to receive and lastly we pass 0 as a sign for no-flags
        int replyFromClient = recv(socketToBeAccepted, clientMessage, 250, 0);
        if (replyFromClient < 0){
            printf("An error has occurred while trying to receive a reply from the client\n");
            break;
        }
        else {
            printf("Client message received: %s\n", clientMessage);
            // We use the strcpy function to load the "Hello client!" message to its corresponding serverMessage buffer
            strcpy (serverMessage, "Hello client!");
            // We use the send function to send a message to the client. We pass the socketToBeAccepted as the socket we want to send data to, then the message, then the length of the message itself and lastly 0 to represent no flags added
            int sendingMessage = send(socketToBeAccepted, serverMessage, strlen(serverMessage), 0);
            // Simple error handling with if/else conditional
            if (sendingMessage < 0) {
                printf("Sending of the message to the server failed\n");
                return 1;
            }
            else {
                printf("Message successfully sended");
            }
        }

        close(socketToBeAccepted);
        sleep (5);
    } 
    return 0;
}
