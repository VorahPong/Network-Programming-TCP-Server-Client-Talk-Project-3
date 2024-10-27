#include <iostream>     // for output, input
#include <stdlib.h>
#include <sys/socket.h> // for utilzing socket, tcp
#include <netinet/in.h> // for sockaddr_in, AF_INET...
#include <arpa/inet.h>  // for inet_ntoa
#include <cstring>      // might need for older g++ version to use memset()

// Main
// argv[0] for executable, argv[1] for port
int main (int argc, char ** argv) {
    // check if port was put by the user
    if(argc < 2) {
        std::cout << "Missing port number\n";
        std::cout << "Direction: please execute like this\n";
        std::cout << argv[0] << " <port number>\n";
        // exit with error code
        exit(1);
    }

    int port = atoi(argv[1]);   // take port in argv[1] since argv[0] is for the name of executable

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0); // using ipv4, TCP, protocol 0 for TCP
    if(sock < 0) {
        std::cout << "Error creating socket\n";
        exit(1);
    }

    // Bind
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address)); // clear address for safety
    address.sin_family = AF_INET;
    address.sin_port = htons(port); // htons() to flip the byte if the computer is using big endian

    int bind_value = bind(sock, (struct sockaddr *)&address, sizeof(address));
    // if bind return negative value then it is error
    if(bind_value < 0) {
        std::cout << "Could not bind\n";
        exit(1);
    }

    // Listen 
    int listen_value = listen(sock, 1);
    // if listen is negative then it is error
    if(listen_value < 0) {
        std::cout << "Could not listen\n";
        exit(1);
    }

    // Accept a connection
    struct sockaddr_in remote_address;          // hold information of incoming connection
    memset(&remote_address, 0, sizeof(remote_address)); // clear address for safety
    socklen_t remote_addrlen = sizeof(address); // hold the length of our address

    // accept function will stop the program till someone connect
    std::cout << "Waiting for connection...\n";
    int client_socket = accept(sock, (struct sockaddr *)&remote_address, &remote_addrlen); 

    // After connection
    // check if client is valid
    if(client_socket < 0) {
        std::cout << "Cannot accept a connection";
        exit(1);
    }
    std::string client_ip = inet_ntoa(remote_address.sin_addr); // to see client IP address
    int remote_port = ntohs(remote_address.sin_port);           // to see client PORT

    std::cout << "  A client @ " << client_ip << ":" << remote_port << " has connected.\n\n";

    //Receive client message
    // create buffer to store client message
    int BUFFLEN = 1024;
    char buffer[BUFFLEN];

    // always clear the buffer every time before receive new message
    while(true) {
        memset(buffer, 0, BUFFLEN);
        // Receive client message
        int bytes_received = recv(client_socket, buffer, BUFFLEN-1, 0); // BUFFLEN-1 because last character should be end

        // if bytes is negative then it is error
        if(bytes_received < 0) {
            std::cout << "Could not receive\n";
            exit(1);
        }
        // if bytes is 0 then the client is disconnecting
        if(bytes_received == 0) {
            std::cout << "Client @" << client_ip << ":" << remote_port << " has disconnected.\n";
            break;
        }
        // if bytes is positive then the client send something
        // show client message
        std::cout << "Client: ";
        printf("%s", buffer); // use printf() on buffer because it gives weird symbol in ada terminal
        std::cout << std::endl; 

        // send back message to client
        char message[1024];
        memset(&message, 0, sizeof(message));
        std::cout << "Server: ";
        std::cin.getline(message, 1024);

        int bytes_send = send(client_socket, message, strlen(message), 0);
        if(bytes_send < 0) {
            std::cout << "Error sending message to client\n";
            exit(1);
        }
    }

    // After client left, the server will shutdown
    std::cout << "Shutting down server...\n";
    shutdown(client_socket, SHUT_RDWR); // SHUT_RDWR = no more receiving or writing

    exit(0);
}
//