#pragma once

// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Helpers.cpp"
#include "NodeParent.hpp"

class Client{
private:
    FileSystem* fs;
public:
    Client(FileSystem* filesys)
    {
        this->fs = filesys;
    };
    int say(int portNumber, NodeParent *np, std::string msg, std::string & ans)
    {
        // first, request message to send through given port number
        // opcional: si no se da msg, preguntar por consola
        if(msg == "" ){
            std::cout << RESET << BOLD;
            std::cout << "Message: ";
            std::cout << RESET;
            std::getline(std::cin, msg);
        }
        
        int resultado = 0;
        int s = 0;
        int n = 0;
        char datos[256];
        struct sockaddr_in ipServidor;
     
        memset(datos, '\0' ,sizeof(datos));
     
        if((s = socket(AF_INET, SOCK_STREAM, 0))< 0 ){
          std::cout << "Error de creación de socket" << "\n";
          resultado=1 ;
        }
        else {
          ipServidor.sin_family = AF_INET;
          ipServidor.sin_port = htons(portNumber);
          ipServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
   
          if(connect(s, (struct sockaddr *)&ipServidor, sizeof(ipServidor))<0)
          {
              std::cout << "\n" << "Error de conexión por IP o puerto" << "\n";
              resultado= 2;
            }
            else {
              // Aca esta conectado, de fijo
              //msg = "agregar " + std::to_string(random(2, 8));
              send(s, msg.c_str(), msg.length(), 0);
              np->process(datos);
              
              std::cout << "Tu mensaje es: " << msg << "\n";
              int valread = read(s, datos, 256);
              ans = datos;
              //std::cout << "Respuesta del servidor: ";
              //std::cout << datos << "\n";
            }
        }
      
        return resultado;
    }
};
