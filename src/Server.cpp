#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Server.hpp"


std::string Server::process(std::string line)
{
    std::vector<std::string> argv = tokenize(line, " ");
	int argc = argv.size();
	
	if(argc > 0)
	{
		if(argv[0] == "ping")
		{
	    	return "pong";
		}
		else
		{
			return "Unknown message";
		}
	}
    return "No message recieved";
};

Server::Server(FileSystem* filesys, Client* clientApp)
{
    this->fs = filesys;
    this->clientApp = clientApp;
    
};

int Server::listenInPort(int portNumber, NodeParent *np)
{
    /// "buffer"
    char datos[256]; 
    memset(datos, '\0', sizeof(datos));

    /// "server_fd"
    /// File descriptor.
    int socketServidor = 0;
    
    /// "new_socket"
    int conexion = 0;

    /// return value for checking
    int val = 0;
    
    /// "address"
    struct sockaddr_in ip;
    memset(&ip, '0', sizeof(ip));
    /// Fill info into ip
    // The address family for the transport address.
    // This member should always be set to AF_INET.
    ip.sin_family = AF_INET;
    // An IN_ADDR structure that contains an IPv4 transport address.
    ip.sin_addr.s_addr = htonl(INADDR_ANY);
    // A transport protocol port number.
    ip.sin_port = htons(portNumber);
    

    // Create socket file descriptor
    socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketServidor == 0) return err("Socket failed", 1);

    // bind socketServidor to the other args
    val = bind(socketServidor, (struct sockaddr*)&ip , sizeof(ip));
    if(val < 0) return err("bind", 2);
    
    // Habilita socket para recibir conexiones
    val = listen(socketServidor , 20);
    if(val < 0) return err("listen", 3);
    
    int msjs = 0; sleep(1);
    struct sockaddr_storage ipRemoto;
    socklen_t l= sizeof(ipRemoto);
    char strIpRemoto[INET6_ADDRSTRLEN];
    int port;
    std::cout << RESET << BOLD;
    std::cout << "Server listening." << "\n";
    std::cout << RESET;
    while(msjs<5000)
    {
        conexion= accept(socketServidor, (struct sockaddr*) &ipRemoto, &l);
        if (conexion < 0) return err("accept", 3);
        
        /// ???
        struct sockaddr_in *s = (struct sockaddr_in *)&ipRemoto;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, strIpRemoto, sizeof strIpRemoto);
        std::cout << "Remote IP: " << strIpRemoto << "\n" ;

        /*
        read(conexion, datos, 256);
        string msj="El servidor ha recibido "+ to_string(msjs) + " mensajes.";
        msj.copy(datos,msj.size()+1) ;
        write(conexion, datos, strlen(datos));*/
        
        
        memset(datos, '\0', sizeof(datos));
        int valread = read(conexion, datos, 256);
        
        /// response
        std::string myResponse = np->process(datos);
        send(conexion, myResponse.c_str(), myResponse.length(), 0);

		std::cout << "Message recieved: " << datos << "\n";
        std::cout << "Response sent: " << myResponse << "\n";
        
        close(conexion);
        sleep(1);
    
    } //while
    std::cout << RESET << BOLD;
    std::cout << "Server closed.";
    std::cout << RESET;
    return 0;
};
