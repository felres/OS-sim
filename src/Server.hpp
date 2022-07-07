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
#include "NodeParent.hpp"


class Server
{
private:
    FileSystem* fs;
    Client* clientApp;
    std::string process(std::string str);
    
public:
    Server(FileSystem* filesys, Client* clientApp);
    int listenInPort(int portNumber,NodeParent *np);
};
