

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "BaseDeDatos.hpp"
#include "Helpers.cpp"
#include "NodeParent.hpp"
BaseDeDatos::BaseDeDatos(){
  this->comm = new Communicator(fs);
}

int BaseDeDatos::optionListen()
{
    comm->listen(this);
    return 0;
}

std::string BaseDeDatos::process(std::string msj)
{
    return "0";
}

int BaseDeDatos::run(){

    std::string input;
    if(id == -1)
    {
        // preguntar por id y por lista de outouts validos
        std::cout << RESET << BOLD;
	    std::cout << "My id: ";
	    std::cout << RESET;
	    std::getline(std::cin, input);
        if(isNumber(input))
            id = stoi(input);
        else
            return err("No valid id.", 1);

        std::cout << RESET << BOLD;
        std::cout << "Please type all neighbor ids. Type a non-number to stop (ex. \"stop\")." << "\n";
        std::cout << RESET;
        bool cont = true; 
        while(cont)
        {
            std::cout << RESET << BOLD;
    	    std::cout << "Neighbor id: ";
    	    std::cout << RESET;
    	    std::getline(std::cin, input);
            if(isNumber(input))
            {
                validOutputs.push_back( stoi(input) );
            }
            else
            {
                cont = false;
            }
        }
    } //cierra if
    
    bool cont = true;
    while(cont)
	{
	    std::cout << "Choose an option: " << "\n";
        std::cout << RESET;
        std::cout << "1. Start listening to Intermediarios." << "\n";
        std::cout << "2. option2." << "\n";
        std::cout << "3. Exit." << "\n";
        std::cout << RESET;
        std::cout << "Decision: ";
        std::string input;
        std::getline(std::cin, input);
        if(!isNumber(input)) input = "0";
        int decision = std::stoi(input);
        switch(decision)
        {
            default:
            case 0: std::cout << RED << "Please enter a valid input" << "\n" << RESET; break;
            case 1: optionListen(); break;
            case 2: /*option2()*/; break;
            case 3: cont = false; break;
        }
	}
    return 0;
}