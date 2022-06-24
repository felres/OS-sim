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
        randomize();
        int resultado = 0;
        int s = 0;
        int n = 0;
        char datos[256];
        struct sockaddr_in ipServidor;
     
        memset(datos, '\0' ,sizeof(datos));
     
        if((s = socket(AF_INET, SOCK_STREAM, 0))< 0)
        {
            std::cout << "Error de creación de socket" << "\n";
            resultado=1 ;
        }
        else
        {
            ipServidor.sin_family = AF_INET;
            ipServidor.sin_port = htons(1337);
            ipServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
     
            if(connect(s, (struct sockaddr *)&ipServidor, sizeof(ipServidor))<0)
            {
                std::cout << "\n" << "Error de conexión por IP o puerto" << "\n";
                resultado= 2;
            }
            else
            {
                // Aca esta conectado, de fijo
                std::string hello = "agregar " + std::to_string(random(2, 8));
                send(s, hello.c_str(), hello.length(), 0);
                
                std::cout << "Mensaje: " << hello << "\n";
                int valread = read(s, datos, 256);
                std::cout << RESET << FAINT;
                std::cout << "Datos en client:";
                std::cout << RESET << BOLD;
                std::cout << datos << "\n";
            }
        }
      
        return resultado;
    }
};