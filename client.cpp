#include <iostream>     // for output, input
#include <stdlib.h>
#include <sys/socket.h> // for utilzing socket, tcp
#include <netinet/in.h> // for sockaddr_in, AF_INET...
#include <arpa/inet.h>  // for inet_ntoa
#include <unistd.h>     // for close()

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

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0); // using ipv4, TCP, protocol 0 for TCP
    if(sock < 0) {
        std::cout << "Error creating socket\n";
        exit(1);
    }

    // Set server address
    int port = atoi(argv[1]);   // get server port in argv[1] since argv[0] is for the name of executable
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address)); // clear address for safety
    address.sin_family = AF_INET;
    address.sin_port = htons(port); // htons() to flip the byte if the computer is using big endian

    // Connect to server
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "Connection failed" << std::endl;
        exit(1);
    }

    // Get server info
    std::string server_ip = inet_ntoa(address.sin_addr);
    int server_port = ntohs(address.sin_port);

    std::cout << "  Successfully connected to server @ " << server_ip << ":" << server_port << ".\n\n";
    
    // Receive data from server
    // create buffer to store server message
    int BUFFLEN = 1024;
    char buffer[BUFFLEN];

    while (true){
        // send message to server
        char message[1024];
        memset(&message, 0, sizeof(message));
        std::cout << "Client: ";
        std::cin.getline(message, 1024);

        int bytes_send = send(sock, message, strlen(message), 0);
        if(bytes_send < 0) {
            std::cout << "Error sending message to server\n";
            exit(1);
        }

        // Receive server message
        int bytes_received = recv(sock, buffer, BUFFLEN-1, 0); // BUFFLEN-1 because last character should be end

        // if bytes is negative then it is error
        if(bytes_received < 0) {
            std::cout << "Could not receive\n";
            exit(1);
        }
        // if bytes is 0 then the client is disconnecting
        if(bytes_received == 0) {
            std::cout << "Server @" << server_ip << ":" << server_port << " has disconnected.\n";
            break;
        }
        
        // buffer[bytes_received] = '\0';
        std::cout << "Server: " << buffer << std::endl;
        memset(buffer, 0, BUFFLEN);
    }

    std::cout << "Shutting down client...\n";
    close(sock);

    exit(0);
}
//