Server/Client Talk Application
Developed by VorahPong, Mean
Date 10/27/2024

* Overview
The purpose of this code is to demonstrate the TCP socket functions and data exchanging through a socket.

* Features
The two programs client and server allowed two computers to talk to each other. The server will automatically shutdown when client disconnected.

* Usage
To compile this project, you must have these files:
    client.cpp
    server.cpp
    makefile

To compile:
    1. Open the folder that includes the files in your command prompt.
    2. Execute the command "make" which will run the makefile and create two executables which are server and client.
    3. Open another command prompt in that same folder.
    4. On one command prompt, start a tcp server by executing "./server <port number>"
        For example: ./server 2299
        The port has to be from 1024-65535
    5. On the second command prompt, start a client by executing "./client <same port number as server>"
        For example: ./client 2299
    6. After making sure both executables have no error, the client is ready to send message to server, and server can also send back message.

    Example:

    Command Prompt 1                        Command Prompt 2
    ./server 2299                   
    Waiting for connection...
                                            ./client 2299
    A client @ x.x.x.x:x has connected
                                            Client: Hello
    Client: Hello
    Server: Hi, how are you?                  
                                            Server: Hi, how are you?
                                            Client: I'm good
    Client: I'm good
                                            Control + D (To Quit on Window) or ^ + C (On Mac)
    Client @ x.x.x.x:x has disconnected.
    Shutting down server...
