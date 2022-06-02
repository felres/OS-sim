#pragma once

// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "helpers.cpp"
#define PORT 8080

class Server{
private:
    int process(std::string str)
    {
        std::vector<std::string> vec  = tokenize(str, " ");
        std::cout << "[Server] Got command: " << vec[0] << "\n";
        return 0;
    }
public:
    int run()
    {
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = { 0 };
     
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
            return err("socket failed", 1);
     
        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET,
                       SO_REUSEADDR | SO_REUSEPORT, &opt,
                       sizeof(opt)))
            return err("setsockopt", 2);
        
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
     
        // Forcefully attaching socket to the port 8080
        if (bind(server_fd,
                (struct sockaddr*)&address, sizeof(address)) < 0)
            return err("bind failed", 3);
        
        if (listen(server_fd, 3) < 0)
            return err("listen", 4);
        
        if ((new_socket
             = accept(server_fd, (struct sockaddr*)&address,
                      (socklen_t*)&addrlen)) < 0)
            return err("accept", 5);
        
        while(true)
        {
            valread = read(new_socket, buffer, 1024);
            process(buffer);
            
            
            /*
            std::string str = "Hello from server.";
            send(new_socket, str.c_str(), str.length(), 0);
            std::cout << "[Server] Message sent." << "\n";
            */
        }
        
        // close connected socket
        close(new_socket);

        // close
        shutdown(server_fd, SHUT_RDWR);
        return 0;
    }
};