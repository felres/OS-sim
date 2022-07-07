#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Intermediario.hpp"
#include "NodeParent.hpp"
#include "Communicator.hpp"


Intermediario::Intermediario(){
  this->fs = new FileSystem();
  this->comm = new Communicator(fs);
  this->id = id;
	this->mapInstruccion = new std::map<std::string, Instruccion>;
	initMap();
}

int Intermediario::run(){
 
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
        std::cout << "1. Start listening to vote centers." << "\n";
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

int Intermediario::optionListen()
{
    comm->listen(this);
    return 0;
}

std::string Intermediario::process(std::string msj){
  std::vector<std::string> argv = tokenize(msj, ",");
	int argc = argv.size();
	
	if(argc > 0)
	{
    switch(mapInstruccion[argv[0]])
        {
            default:
            case Instruccion::isAlive: std::cout << RED << "Please enter a valid input" << "\n" << RESET; break;
            case Instruccion::yesAlive: optionIsAlive(); break;
            case Instruccion::
							init
							getInfoPerson(); break;
            case 3: regsiterVote(); break;
            case 4: registerVoterPerson(); break;
            case 5: cont = false; break;
        }
		if(argv[0] == "isAlive")
		{
	    	return "Yes, I am alive";
		}
		else
		{
			return "Unknown message";
		}
	}
    return "No message recieved";
}

void Intermediario::initMap(){
	mapInstruccion["isAlive"] = Instruccion::isAlive;
	mapInstruccion["requestPerson"] = Instruccion::requestPerson;
	mapInstruccion["responsePerson"] = Instruccion::responsePerson;
	mapInstruccion["setVote"] = Instruccion::setVote;
	mapInstruccion["confirmVote"] = Instruccion::confirmVote;
	mapInstruccion["setPVoted"] = Instruccion::setPVoted;
	mapInstruccion["confirmPVoted"] = Instruccion::confirmPVoted;
	mapInstruccion["setMultVotes"] = Instruccion::setMultVotes;
	mapInstruccion["confirmMultVotes"] = Instruccion::confirmMultVotes;
}