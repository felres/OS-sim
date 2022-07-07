#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Communicator.hpp"


#define DEF_NUM 6000

Communicator::Communicator(FileSystem *fs)
{
	this->fs = fs;
	this->clientApp = new Client(fs);
    this->serverApp = new Server(fs, clientApp);
}

int Communicator::say(NodeParent *np, std::string msg)
{
    return clientApp->say(DEF_NUM, np, msg);
};

int Communicator::listen(NodeParent *np)
{
    return serverApp->listenInPort(DEF_NUM, np);
};
