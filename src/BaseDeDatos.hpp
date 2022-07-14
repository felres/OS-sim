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
#include "VotesReg.hpp"
#include "Padrone.hpp"

class BaseDeDatos : public NodeParent{
  private:
  int id = -1;
  int port = 6000;
  std::vector<int> validOutputs;
  FileSystem* fs;
  Padrone *padroneApp;
  VotesReg *voteApp;
  Communicator* comm;
  int optionListen();
public:
    BaseDeDatos(FileSystem* fs, 
                Padrone* padroneApp, 
                VotesReg* voteApp);
    int run();
    int setVote(std::string candidate);
    std::string process(std::string msj);
};