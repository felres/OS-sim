#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "Helpers.cpp"
#include "FileSystem.hpp"

class VotesReg {
private:
	FileSystem* fs;
    int totalCandidates = 50;
    /// Index of votesRegister file we are executing, stored in fs->directorio[]
    int votesRegisterFileIndex = -1;
public:
    VotesReg(FileSystem* filesys){
            fs=filesys;
    };

    int processRegisterNewVote(std::string vote = std::string());
    int processSaveVoteRegIntoReal();
    int run(std::string filename);
    int openVotesReg(std::string filename);
    std::string getVotesRegFileName();
    int processPrintIndividualVotes();
    int processCountAndPrintVotes();
};