#pragma once

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include "Helpers.cpp"
#include "Communicator.hpp"
#include "FileSystem.hpp"
#include "NodeParent.hpp"

enum Instruccion {
	isAlive, 
	yesAlive, 
	requestPerson,
	responsePerson,
	setVote,
	confirmVote,
	setPVoted,
	confirmPVoted,
	setMultVotes,
	confirmMultVotes};


class Intermediario : public NodeParent{
  private:
  int id = -1;
  int port = 6000;
  std::vector<int> validOutputs;
  FileSystem* fs;
  std::string destAddress;
  Communicator* comm;
	std::map<std::string, Instruccion> mapInstruccion;
	void initMap();
public:
    Intermediario();
    int run();
    std::string process(std::string msj);
    std::string requestPerson(std::string msj);
    int storageIsAlive();
    std::string getInfoPerson(std::string msj);
    std::string saveVote(std::string msj);
    std::string saveVoterPerson(std::string msj);
    int optionListen();
};
