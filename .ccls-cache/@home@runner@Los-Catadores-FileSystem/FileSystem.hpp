#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include "helpers.cpp"

/// File entry struct
struct File{
    std::string nombre;
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
    std::string nombre;
    std::string password;
    int primaryGroupId;
};

struct Group{
    int id;    
    std::string nombre;
    int memberIdList[SIZE];
    int memberCount;
};

class FileSystem{

private:
    /// Vector that contains all files
    std::vector<File> directorio;
    int entradasTotales = 0; // num de archivos

    /// Vector that acts as File Allocation System (FAT).
    /// -1 means data block is available.
    std::vector<int> fat;

    /// Memory unit, that acts as real storage.
    std::vector<char> unidadDeMemoria;

    /// Wether assign fat blocks in order or not.
    bool entregaAleatoria = false;
    
    /// Used as temporary global variable for some recursive methods.
    std::string strtemp;

    /// I dont remember
    bool seguro = false;

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

    /**
     * Description.
     * 
     * @param
     * @return
     */
	int encontrarBloqueVacio();

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int encontrarBloqueVacioSecuencial();

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int encontrarBloqueVacioAleatorio();

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int encontrarUltimoFAT(int bloqueInicial);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int encontrarIndiceFAT(int bloqueInicial, int indice, int count);
    
    /**
     * Description.
     * 
     * @param
     * @return
     */
    std::string stringDesde(int bloqueInicial);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int stringDesdeRecursivo(int bloque);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int eliminarRecursivo(int bloque);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int getEntryIndex(std::string name);

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

    /**
     * Description.
     * 
     * @param
     * @return
     */
    bool canCurrentUserModifyFile(std::string filename);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    bool canCurrentUserReadFile(std::string filename);

public:
    /**
     * Constructor
     */
    FileSystem();

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
    int createUser(std::string name, std::string groupName, std::string password);

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

    /* retorna 0 si todo bien
     * si no, hubo problema
     */
    int crear(std::string nombre);

    /* retorna 0 si todo bien
     * si no, hubo problema
     */
    int escribir(std::string nombre, int index, char caracter);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int readFile(std::string filename);
    
	/* retorna 0 si todo bien
     * si no, hubo problema
     */
    int agregar(std::string nombre, char caracter);    

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int eliminar(std::string nombre, bool seguro);

    /**
     * Description.
     * 
     * @param
     * @return
     */
    int changemode(std::string filename, int flagindex, bool b);
    
	/* retorna 0 si todo bien
     * si no, hubo problema
     */
    int agregar(std::string nombre, std::string str);

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

