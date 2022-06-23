#pragma once


#include <string>
#include "FileSystem.hpp"

class Padrone {
private: 
	FileSystem* fs;

    /// Index of padrone file we are executing, stored in fs->directorio[]
    int padroneFileIndex = -1;

    // process related
    int processNewPadrone();
    int processOpenPadrone();
    int processLoadRealPadrone();
    int processPrintFullPadrone();
    int processPersonCheck();
    int processSetVotingStatus();
    std::string getPadroneFileName();
    std::vector<std::string> getPersonColumn(std::string id);

public:
	/**
     * Constructor.
     * 
     * @param filesys filesystem object to refer to when working.
     */
    Padrone(FileSystem* filesys){
        fs=filesys;
    };

    int processAddPerson();
    
    int processSavePadroneFileIntoRealFile();

    int openPadrone(std::string filename);

    int run(std::string filename);
    
    /**
     * Create a new padrone filesystem from scratch.
     * Default extension is .p
     * 
     * @param filename name for the new padrone file
     * @return
     */
	int createPadFile(std::string filename);

    /**
     * Loads and creates a padrone from a real file.
     * 
     * @param
     * @return
     */
	int loadFromRealFile(std::string filename);	
	
    /**
     * Description.
     * 
     * @param
     * @return
     */    
    int appendtoFile(std::string filename, std::string str);

    /**
     * Returns line belonging to person.
     * 
     * @param id identification of person
     * @return "" if person doesnt exist
     */
	std::string getPerson(std::string id);

    /**
     * Checks whether given id belongs to a person that exists in
     * current padrone.
     * 
     * @param
     * @return
     */
	bool doesPersonExist(std::string id);

    /**
     * Checks if person has voted.
     * 
     * @param
     * @return
     */
	bool hasPersonVoted(std::string id);

    /**
     * Turns person vote char to true (1).
     * 
     * @param
     * @return
     */
	int setVoted(std::string id);
};