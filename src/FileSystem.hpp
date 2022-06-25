#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include "Helpers.cpp"

/// File entry struct
struct File{
    std::string name;
    int bloqueInicial;    
    time_t fechaCreacion;

    /// permission stuff
    int ownerId;
    int groupId;
    // flags
    bool ownerCanWrite;
    bool ownerCanRead;
    bool ownerCanExecute;
    bool groupCanWrite;
    bool groupCanRead;
    bool groupCanExecute;
    bool publicCanWrite;
    bool publicCanRead;
    bool publicCanExecute;

};

struct User{
    int id;
    std::string name;
    std::string password;
    int primaryGroupId;
};

struct Group{
    int id;    
    std::string name;
    int memberIdList[SIZE];
    int memberCount;
};

class FileSystem{

private:
    /// Vector that contains all files
    std::vector<File> directorio;
    int entradasTotales = 0; // should not exist

    /// Vector that acts as File Allocation System (FAT).
    /// -1 means data block is available.
    std::vector<int> fat;

    /// Memory unit, that acts as real storage.
    std::vector<char> unidadDeMemoria;

    /// Whether look for FAT blocks in order or not.
    bool randomizedBlockSearching = false;
    
    /// Used as temporary global variable for some recursive methods.
    std::string strtemp;

    /// When deleting, memory will be set to blank too
    bool secureDeletion = false;

    /// Current user logged in the interface.
    /// -1: No user.
    /// 0: root.
    int currentUserIndex = -1;

    /// Array containing group list.
    Group groups[SIZE];
    int groupCount = 0;

    /// Array containing user list.
    User users[SIZE];
    int userCount = 0;

    int createUserBypass(std::string name, std::string groupName, std::string password);

    std::string requestPasswordRaw(std::string str);

    std::string encrypt(std::string str);

    std::string decrypt(std::string str);

    /**
     * Can either search for a block randomly or sequentially.
     * 
     * @return Index of available memory block. -1 if full.
     */
	int getAvailableBlock();

    /**
     * Searches for the earliest available block in memory.
     * Counts from block 0.
     * 
     * @return Index of available memory block. -1 if full.
     */
    int getAvailableBlockSequential();

    /**
     * Searches for a random block in memory.
     * Has a limited number of tries, and if depleted uses sequential.
     * 
     * @return Index of available memory block. -1 if full.
     */
    int getAvailableBlockRandom();

    /**
     * Recursively search through FAT references until
     * it finds the index that references itself.
     * Meaning it searches for the last index in the allocation block sequence.
     * Its useful for finding the block where a string ends. 
     *
     * @param
     * @return
     */
    int getLastBlockInChainFrom(int block);

    /**
     * Recursively finds specific index in FAT string.
     * Should always be called with count=0 for the first time.
     * Its useful for finding a specific index in the string. 
     * 
     * @param blockIndex Index in FAT to start from.
     * @param index Index we need.
     * @param count Internal count to get to index.
     * @return
     */
    int getBlockInChainAtIndex(int blockIndex, int index, int count);
    
    /**
     * Works like getLastBlockInChainFrom, but saves a string to
     * global variable and returns it.
     * 
     * @param
     * @return
     */
    std::string getStringFrom(int bloqueInicial);

    /**
     * Recursive element of getStringFrom.
     * 
     * @param
     * @return
     */
    int getStringFromRecursive(int bloque);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int removeFileRecursive(int bloque);

    

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int getGroupIndex(std::string name);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int getUserIndex(std::string name);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    std::string getUserNameWithId(int userId);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    std::string getUserNameAt(int index);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    std::string getGroupNameAt(int index);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    Group getGroupWithId(int groupId);

    

public:
    /**
     * Constructor
     */
    FileSystem();

    /**
     * Used internally with padrone and voting apps. Still, it checcks permissions
     * 
     * @param
     * @return
     */
    std::string requestFileContents(std::string filename);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int getFileIndex(std::string name);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    std::string getFileNameAt(int name);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    bool canCurrentUserModifyFile(std::string filename);

    int changePassword();

    /**
     * Description.
     * 
     * @param
     * @return
     */
    bool canCurrentUserReadFile(std::string filename);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    bool canCurrentUserExecuteFile(std::string filename);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    std::string getName();

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int createGroup(std::string name);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int createUser(std::string name, std::string groupName);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int switchUser(std::string user);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    void usarAsignacionAleatoria(bool usar);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int createFile(std::string name);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int writeInFileAt(std::string name, int index, char caracter);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int readFile(std::string filename);
    
	/**
     * Description.
     * 
     * @param
     * @return
     */
    int appendToFile(std::string name, char caracter); 

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int appendToFile(std::string name, std::string str);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int removeFile(std::string name, bool seguro);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int changemode(std::string filename, int flagindex, bool b);
    
	

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int getPrimaryMemberIdsInGroup(int groupId, int* buffer);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int loadTest();
    int loadTestOld();

    int loadRealFile(std::string filename);

    /**
     * Description.
     * 
     * @param
     */
    void imprimirStrings();

    /**
     * Description.
     * 
     * @param
     */
    void imprimirDirectorio();

    /**
     * Description.
     * 
     * @param
     */
    void imprimirFAT(int cols);

    /**
     * Description.
     * 
     * @param
     */
    void imprimirMemoria(int cols);

    /**
     * Description.
     * 
     * @param
     */
    void imprimirUnidad(int cols);   

    /**
     * Description.
     */
    void printFiles();

    /**
     * Description.
     */
    void printUsers();

    /**
     * Description.
     */
    void printGroups();
};

