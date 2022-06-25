#include <algorithm>
#include <ctype.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include "Padrone.hpp"
#include "Helpers.cpp"

int Padrone::run(std::string filename)
{
    if(openPadrone(filename)!=0)
        return err("Problem trying to open file.", 1);
    
    bool cont = true;
    while(cont)
    {
        std::cout << RESET << BOLD << YELLOW;
        std::cout << "File currently open: " << getPadroneFileName() << "\n";
        std::cout << "Choose an option: " << "\n";
        std::cout << RESET << YELLOW;
        std::cout << "1. Read entire padrone." << "\n";
        std::cout << "2. Check info about a single person." << "\n";
        std::cout << "3. Set a person's voting status to TRUE." << "\n";
        std::cout << "4. Add a new person to the padrone." << "\n";
        std::cout << "5. Save current padrone into real padrone file." << "\n";
        std::cout << "6. Exit." << "\n";
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
            case 1: processPrintFullPadrone(); break;
            case 2: processPersonCheck(); break;
            case 3: processSetVotingStatus(); break;
            case 4: processAddPerson(); break;
            case 5: processSavePadroneFileIntoRealFile(); break;
            case 6: cont = false; break;
        }
    }
    return 0;
};

int Padrone::processAddPerson()
{
    std::string id, name, table, didVote, sfinal;
    std::cout << "New person Id: ";
    std::getline(std::cin, id);
    if(doesPersonExist(id)) return err("Person with Id "+id+" already exists.", 1);
    std::cout << "New person full name: ";
    std::getline(std::cin, name);
    // convert name to all uppercase
    std::transform(name.begin(), name.end(), name.begin(), ::toupper); 
    std::cout << "New person voting table: ";
    std::getline(std::cin, table);
    std::cout << "Has new person voted? (True/false): ";
    std::getline(std::cin, didVote);
    if(strToBool(didVote))
        didVote = "1";
    else
        didVote = "0";
    sfinal = id+","+name+","+table+","+didVote;
    fs->appendToFile(getPadroneFileName(), sfinal);
    return 0;
};
    
int Padrone::processSavePadroneFileIntoRealFile()
{
    writeFile(getPadroneFileName(), fs->requestFileContents(getPadroneFileName()));
    return 0;
};

int Padrone::processNewPadrone()
{
    // not urgent
    return 0;
};

int Padrone::processOpenPadrone()
{
    // open nonreal padrone file
    std::cout << "file name: ";
    std::string input;
    std::getline(std::cin, input);

    openPadrone(input);
    
    return 0;
};

int Padrone::processLoadRealPadrone()
{
    // not urgent
    return 0;
};

int Padrone::openPadrone(std::string name)
{
    int fi = fs->getFileIndex(name);
    if(fi==-1) return err("No such file called "+name, 1);

    padroneFileIndex = fi;
    return 0;
}

int Padrone::processPrintFullPadrone()
{
    if(padroneFileIndex==-1) return err("No padrone file opened.", 1);

    
    std::string fileStr = fs->requestFileContents(getPadroneFileName());
    std::vector<std::string> vec = tokenize(fileStr, "\n");
    std::cout << RESET << BOLD;
	std::cout << "         Id|                            Name| Place| Voted?" << "\n";
	std::cout << RESET;
    for(int i = 0; i < vec.size(); i++)
    {
        std::vector<std::string> row = tokenize(vec[i], ",");
        std::cout << std::setw(11) << row[0] << "|";
        std::cout << std::setw(32) << row[1] << "|";
        std::cout << std::setw(6) << row[2] << "|";
        std::cout << " ";
        printYesNo(strToBool(row[3]));
        std::cout << "\n";
        
    }
    std::cout << "\n";
    return 0;
};

// print full info on someone, including voting status
int Padrone::processPersonCheck()
{
    if(padroneFileIndex==-1) return err("No padrone file opened.", 1);

    std::cout << "Person Id: ";
    std::string input;
    std::getline(std::cin, input);
    
    if(!doesPersonExist(input))
        return err("Person ID "+input+" does not exist.", 2);
    std::vector<std::string> personInfo = getPersonColumn(input);
    std::cout << BOLD;
    std::cout << "Id: ";
    std::cout << RESET;
    std::cout << personInfo[0] << "\n";
    std::cout << BOLD;
    std::cout << "Name: ";
    std::cout << RESET;
    std::cout << personInfo[1] << "\n";
    std::cout << BOLD;
    std::cout << "Voting classroom: ";
    std::cout << RESET;
    std::cout << personInfo[2] << "\n";
    std::cout << BOLD;
    std::cout << "Voting information: ";
    std::cout << RESET;
    if(hasPersonVoted(input))
        std::cout << "This person already voted." << "\n";
    else
        std::cout << "This person has not yet voted." << "\n";
    std::cout << RESET;
    return 0;
};

int Padrone::processSetVotingStatus()
{
    if(padroneFileIndex==-1) return err("No padrone file opened.", 1);
    std::cout << "Id of person to set voting status to TRUE: ";
    std::string input;
    std::getline(std::cin, input);

    
    if(!doesPersonExist(input))
        return err("Person ID "+input+" does not exist.", 2);
    setVoted(input);
    
    return 0;
};

int Padrone::createPadFile(std::string filename)
{
	// not urgent
    return 0;
};

int Padrone::loadFromRealFile(std::string filename)
{
    // not urgent
    return 0;
};

std::vector<std::string> Padrone::getPersonColumn(std::string id)
{
    std::string fileStr = fs->requestFileContents( getPadroneFileName() );
    std::vector<std::string> vec = tokenize(fileStr, "\n");
    for(int i = 0; i < vec.size(); i++)
    {
        std::vector<std::string> columns = tokenize(vec[i], ",");
        if(columns[0] == id) return columns;
    }
    return {};
};

bool Padrone::doesPersonExist(std::string id)
{
    std::string fileStr = fs->requestFileContents( getPadroneFileName() );
    std::vector<std::string> vec = tokenize(fileStr, "\n");
    for(int i = 0; i < vec.size(); i++)
    {
        std::vector<std::string> columns = tokenize(vec[i], ",");
        if(columns[0] == id) return true; //check id part
    }
    return false;
};

bool Padrone::hasPersonVoted(std::string id)
{
    std::string fileStr = fs->requestFileContents( getPadroneFileName() );
    std::vector<std::string> vec = tokenize(fileStr, "\n");
    for(int i = 0; i < vec.size(); i++)
    {
        std::vector<std::string> columns = tokenize(vec[i], ",");
        if((columns[0] == id)&&(columns[3]=="1")) return true;
    }
    return false;
}

int Padrone::setVoted(std::string id)
{
    // VERY UNSTABLE METHOD BUT IT WORKS FOR NOW :^)
    std::string fileStr = fs->requestFileContents( getPadroneFileName() );
    int fileIndexToChange = 0;
    std::vector<std::string> vec = tokenize(fileStr, "\n");
    for(int i = 0; i < vec.size(); i++)
    {
        std::vector<std::string> row = tokenize(vec[i], ",");
        if(row[0] == id)
        {
            // Change last char to a 1, no matter what
            fileIndexToChange += vec[i].length() -1;
            fs->writeInFileAt(getPadroneFileName(), fileIndexToChange, '1');
            return 0;
        }

        fileIndexToChange += vec[i].length()+1; //add line length + "\n"
    }
    return err("Could not set vote because person was not found", 1);
};

std::string Padrone::getPadroneFileName()
{
    if(padroneFileIndex==-1) return "";

    return fs->getFileNameAt(padroneFileIndex);
}
