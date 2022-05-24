#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include "helpers.cpp"

/// File entry struct
struct Entry{
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
    std::vector<Entry> directorio; // aqui se guardan los datos
    int entradasTotales = 0; // num de archivos
    std::vector<int> fat;
    std::vector<char> unidadDeMemoria;
    bool entregaAleatoria = false;
    // Usado en algunos metodos recursivos
    std::string strtemp;
    bool seguro = false;

    // permission related
    int currentUserIndex = -1;
    Group groups[SIZE];
    int groupCount = 0;
    User users[SIZE];
    int userCount = 0;

	int encontrarBloqueVacio();
    int encontrarBloqueVacioSecuencial();
    int encontrarBloqueVacioAleatorio();
    int encontrarUltimoFAT(int bloqueInicial);
    int encontrarIndiceFAT(int bloqueInicial, int indice, int count);
    
    /* 
     * Retorna el string empezando en lugar dado de la memoria  
     */
    std::string stringDesde(int bloqueInicial);
    int stringDesdeRecursivo(int bloque);
    int eliminarRecursivo(int bloque);
    int getEntryIndex(std::string name);
    int getGroupIndex(std::string name);
    int getUserIndex(std::string name);
    std::string getUserNameWithId(int userId);
    std::string getUserNameAt(int index);
    std::string getGroupNameAt(int index);
    Group getGroupWithId(int groupId);
    bool canCurrentUserModifyFile(std::string filename);
    bool canCurrentUserReadFile(std::string filename);

public:
    /*
     * Constructor
     */
    FileSystem();

    std::string getName();
    int createGroup(std::string name);
    int createUser(std::string name, std::string groupName, std::string password);
    int switchUser(std::string user);

    void usarAsignacionAleatoria(bool usar);

    /* retorna 0 si todo bien
     * si no, hubo problema
     */
    int crear(std::string nombre);

    /* retorna 0 si todo bien
     * si no, hubo problema
     */
    int escribir(std::string nombre, int index, char caracter);

    int readFile(std::string filename);
    
	/* retorna 0 si todo bien
     * si no, hubo problema
     */
    int agregar(std::string nombre, char caracter);    
    int eliminar(std::string nombre, bool seguro);

    int changemode(std::string filename, int flagindex, bool b);
    
	/* retorna 0 si todo bien
     * si no, hubo problema
     */
    int agregar(std::string nombre, std::string str);
    int getPrimaryMemberIdsInGroup(int groupId, int* buffer);
    int loadTest();    
    void imprimirStrings();
    void imprimirDirectorio();
    void imprimirFAT(int cols);
    void imprimirMemoria(int cols);
    void imprimirUnidad(int cols);    
    void printFiles();
    void printUsers();
    void printGroups();
};

