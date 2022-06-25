#pragma once

// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Helpers.cpp"
#include "FileSystem.hpp"
#include "Client.hpp"
//#include "Interface.hpp"

class Server
{
private:
    FileSystem* fs;
    Client* clientApp;
    //Interface* console;
    std::string process(std::string str);
    
public:
    Server(FileSystem* filesys, Client* clientApp);
    int listenInPort(int portNumber);
};
