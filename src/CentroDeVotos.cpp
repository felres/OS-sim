

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "CentroDeVotos.hpp"
#include "NodeParent.hpp"


int CentroDeVotos::run(){

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
        std::cout << "1. Ask if Intermediario is alive." << "\n";
        std::cout << "2. Get info of a person." << "\n";
        std::cout << "3. Registrar Voto." << "\n";
        std::cout << "4. Registrar votante." << "\n";
        std::cout << "5. Exit." << "\n";
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
            case 1: optionIsAlive(); break;
            case 2: getInfoPerson(); break;
            case 3: registerVote(); break;
            case 4: registerVoterPerson(); break;
            case 5: cont = false; break;
        }
	}
    return 0;
}


std::string CentroDeVotos::process(std::string msj){
  return "0";
}

CentroDeVotos::CentroDeVotos(){
  this->comm = new Communicator(fs);
}

int CentroDeVotos::optionIsAlive()
{
    comm->say(this, "isAlive");
    return 0;
}

int CentroDeVotos::getInfoPerson(std::string id, )
{
  comm->say(this, "isAlive");
    return 0;
}

int CentroDeVotos::registerVote()
{
  comm->say(this, "isAlive");
    return 0;
}

int CentroDeVotos::registerVoterPerson()
{
  comm->say(this, "isAlive");
    return 0;
}
