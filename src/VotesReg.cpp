#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "FileSystem.hpp"
#include "VotesReg.hpp"
#include "Helpers.cpp"

int VotesReg::run(std::string filename)
{
    if(openVotesReg(filename)!=0)
        return err("Problem trying to open file.", 1);
    
    bool cont = true;
    while(cont)
    {
        std::cout << RESET << BOLD << BLUE;
        std::cout << "File currently open: " << getVotesRegFileName() << "\n";
        std::cout << "Choose an option: " << "\n";
        std::cout << RESET << BLUE;
        std::cout << "1. Read individual votes registered." << "\n";
        std::cout << "2. Count and list votes." << "\n";
        std::cout << "3. Register a new vote." << "\n";
        std::cout << "4. Save current vote register into real votes file." << "\n";
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
            case 1: processPrintIndividualVotes(); break;
            case 2: processCountAndPrintVotes(); break;
            case 3: processRegisterNewVote(); break;
            case 4: processSaveVoteRegIntoReal(); break;
            case 5: cont = false; break;
        }
    }
    return 0;
};

int VotesReg::openVotesReg(std::string filename)
{
    int fi = fs->getFileIndex(filename);
    if(fi==-1) return err("No such file called "+filename, 1);

    votesRegisterFileIndex = fi;
    return 0;
};

std::string VotesReg::getVotesRegFileName()
{
    if(votesRegisterFileIndex==-1) return "";

    return fs->getFileNameAt(votesRegisterFileIndex);
};

int VotesReg::processPrintIndividualVotes()
{
    if(votesRegisterFileIndex==-1) return err("No votes file opened.", 1);

    std::string fileStr = fs->requestFileContents( getVotesRegFileName() );
    std::vector<std::string> vec = tokenize(fileStr, "\n");
    std::cout << RESET << BOLD;

    for(int i = 0; i < vec.size(); i++)
    {
      if (vec[i] == "0") {
        std::cout << "-Voto en blanco" << "\n";
      } else if (vec[i] == "-1") {
        std::cout << "-Voto nulo" << "\n";
      } else{
        std::cout << "-Voto por partido #" << vec[i] << "\n";
      }
    }  
    return 0;
};

int VotesReg::processCountAndPrintVotes()
{
    // Vector con la cantidad total de candidatos
    // El + 2 es para agregar los votos nulos y los en blanco
    std::vector<int> votes (totalCandidates+2); 
    std::string fileStr = fs->requestFileContents( getVotesRegFileName() );
    std::vector<std::string> vec = tokenize(fileStr, "\n");
    int pivot = 0;
    for(int i = 0; i < vec.size(); ++i)
    {
      if (vec[i] == "0") {
        votes[0]++; 
      } else if (vec[i] == "-1") {
        votes[1]++;
      } else {
        pivot = stoi(vec[i]);
        votes[pivot+1]++;
      }
    }
    for(int i = 0; i < totalCandidates+2; ++i)
    {
      switch (i){
        case 0:
          std::cout << "Votos en Blanco: " << votes[i] << std::endl;
          break;
        case 1:
          std::cout << "Votos Nulos: " << votes[i] << std::endl;
          break;
        default:
          std::cout << "Votos por partido #" << i-1 << ": " << votes[i] << std::endl;
          break;
      }
    }
    return 0;
};

int VotesReg::processRegisterNewVote()
{
    std::string input, sfinal;
    std::cout << RESET << BOLD << BLUE;
    std::cout << "- You can input any string as a valid vote." << "\n";
    std::cout << "- An empty input will cast a vote as EN BLANCO." << "\n";
    std::cout << "- An incorrect input will cast a vote as NULO." << "\n";
    std::cout << RESET;
    std::cout << "Enter the vote choice: ";
    std::getline(std::cin, input);
    if(input == "")
    {
        sfinal = "0"; //en blanco
    }
    else if(isNumber(input))
    {
        int num = stoi(input);
        if((num < 1)||(num > totalCandidates))
            sfinal = "-1"; //nulo
        else
            sfinal = input;
    }
    else
    {
        sfinal = "-1"; //nulo
    }
    std::cout << "Recording vote as " << sfinal << ".\n";
    fs->appendToFile(getVotesRegFileName(), sfinal+"\n");
    return 0;
};

int VotesReg::processSaveVoteRegIntoReal()
{
    writeFile(getVotesRegFileName(), fs->requestFileContents(getVotesRegFileName()));
    return 0;
};
