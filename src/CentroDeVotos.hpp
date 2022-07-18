#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Helpers.cpp"
#include "Communicator.hpp"
#include "FileSystem.hpp"
#include "NodeParent.hpp"

class CentroDeVotos : public NodeParent{
  private:
  int id = -1;
  int port = 6000;
  std::vector<int> validOutputs;
  std::string destAddress;
  FileSystem* fs;
  Communicator* comm;
  public:
  CentroDeVotos();
  int run();
  std::string process(std::string msj);
  int optionIsAlive();
  int getInfoPerson(std::string id);
  int registerVote(std::string candidate);
  int registerVoterPerson(std::string id);
};
