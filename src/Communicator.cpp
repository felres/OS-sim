#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Communicator.hpp"

Communicator::Communicator(FileSystem *fs)
{
	this->fs = fs;
	this->clientApp = new Client(fs);
    this->serverApp = new Server(fs, clientApp);
}

int Communicator::say(int portNumber)
{
    return clientApp->say(portNumber);
};

int Communicator::listen(int portNumber)
{
    return serverApp->listenInPort(portNumber);
};
