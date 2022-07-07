#pragma once
#include <string>
#include <stdlib.h>
#include <string>
class NodeParent{
  public:
    virtual std::string process(std::string msj) = 0; //forzar hijos a implementar
};