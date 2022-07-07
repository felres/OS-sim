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

class BaseDeDatos : public NodeParent{
  private:
  int id = -1;
  int port = 6000;
  std::vector<int> validOutputs;
  FileSystem* fs;
  Communicator* comm;
  int optionListen();
public:
    BaseDeDatos();
    int run();
    std::string process(std::string msj);
};