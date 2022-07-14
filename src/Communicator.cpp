#include "Communicator.hpp"

#define DEF_NUM 6000

Communicator::Communicator(FileSystem *fs)
{
	this->fs = fs;
	this->clientApp = new Client(fs);
    this->serverApp = new Server(fs, clientApp);
}

int Communicator::say(NodeParent *np, std::string msg, std::string & ans)
{
    return clientApp->say(DEF_NUM, np, msg, ans);
};

int Communicator::listen(NodeParent *np)
{
    return serverApp->listenInPort(DEF_NUM, np);
};
