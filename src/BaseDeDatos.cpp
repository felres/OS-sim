#include "BaseDeDatos.hpp"

BaseDeDatos::BaseDeDatos(FileSystem* fs,
			                   Padrone* padroneApp,
                         VotesReg* voteApp){
  this->comm = new Communicator(fs);
  this->fs = fs;
  this->padroneApp = padroneApp;
  this->voteApp = voteApp;
  this->fs->loadRealFile("database/votes.txt");
  this->fs->loadRealFile("database/padrone.txt");
  this->voteApp->openVotesReg("database/votes.txt");
  this->padroneApp->openPadrone("database/padrone.txt");
}

int BaseDeDatos::optionListen()
{
    comm->listen(this);
    return 0;
}

std::string BaseDeDatos::process(std::string msj){
  std::vector<std::string> argv = tokenize(msj, ",");
	int argc = argv.size();
	
	if(argc > 0)
	{
    
		if(argv[0] == "isAlive")
		{
	    	return "Yes, I am alive";
		}
    else if(argv[0] == "setVote")
		{
      int check = setVote(argv[1]);
      if(check == 0){
        return "confirmVote, voto recibido";
      }else{
        return "error processing vote, " + check;
      }
        
		}
    else if(argv[0] == "requestPerson")
		{
      return "responsePerson, " + this->padroneApp->dbPersonCheck(argv[1]);        
		}
    else if(argv[0] == "setPVoted")
		{
      int check = this->padroneApp->processSetVotingStatus(argv[1]);
      if(check == 0){
        return "confirmPVoted, " + argv[1];        
      }else{
        return "error seting voting status of " + argv[1] + ", error, " + std::to_string(check);
      }
		}else if(argv[0] == "setMultVotes")
		{
      int votes = 0;
      for(int i = 2; i > argv.size(); i+2){
        for(int j = 0; j >= stoi(argv[i]); ++j){
          setVote(argv[i+1]);
          votes++;
        }
      }
      if (votes == stoi(argv[2])){
        return "confirmMultVotes, " + votes;
      }
      else{
        return "error guardando todos los votos, votos guardados: " + votes;
      }
		}
		else
		{
			return "Unknown message";
		}
	}
    return "No message recieved";
}

int BaseDeDatos::setVote(std::string candidate){
   return this->voteApp->processRegisterNewVote(candidate);
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
		
		/*
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
        }*/
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
