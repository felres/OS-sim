#pragma once

#include <string>
#include "FileSystem.hpp"
#include "Padrone.hpp"
#include "VotesReg.hpp"

class Interface{
private:
	std::string input;
	FileSystem* fs;
    Padrone *padroneApp;
    VotesReg *voteApp;
    
public:
	Interface(FileSystem* fs,
				Padrone* padroneApp,
                VotesReg* voteApp);
	~Interface();

    /**
     * Read user input from cout. 
     * 
     * @param FileSystem object to send user requests to.
     * @return 0 if ok.
     */	
    int run();
	
	/**
     * Analizes line input from user,
     * sends relevant workload to FileSystem.
     * 
     * @param line string to analise.
     * @param *fs file system to use.
     * @return 0 if can recieve more input. 1 if user requested exit.
     */
	int process(std::string line);
};
