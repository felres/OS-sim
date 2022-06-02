#pragma once

// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "helpers.cpp"
#define PORT 8080

class Client{
private:
public:
    int run()
    {
        int sock = 0, valread, client_fd;
        struct sockaddr_in serv_addr;
        char buffer[1024] = { 0 };
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            return err("Socket creation error.", 1);
     
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
     
        // Convert IPv4 and IPv6 addresses from text to binary
        // form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
            <= 0)
            return err("Invalid address", 2);
     
        if ((client_fd
             = connect(sock, (struct sockaddr*)&serv_addr,
                       sizeof(serv_addr))) < 0) 
            return err("Connection failed!", 3);

        std::string str = "Hello from client.";
        send(sock, str.c_str(), str.length(), 0);
        std::cout << "[Client] Message sent." << "\n";
        valread = read(sock, buffer, 1024);
        std::cout << "[Client] Got message: " << buffer << "\n";
     
        // closing the connected socket
        close(client_fd);
        return 0;
    }
};