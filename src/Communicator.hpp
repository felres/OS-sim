#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Helpers.cpp"
#include "Server.hpp"
#include "Client.hpp"



class Communicator
{
private:
    FileSystem* fs;
    Client* clientApp;
    Server* serverApp;
    std::string process(std::string str);
    
public:
	Communicator(FileSystem *fs);
    int say(int portNumber);
    int listen(int portNumber);
};
