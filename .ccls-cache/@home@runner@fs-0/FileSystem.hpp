#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include "helpers.cpp"
#define SIZE 100

struct Entry{
    std::string nombre;
    int bloqueInicial;    
    time_t fechaCreacion;
};

class FileSystem{
private:
    Entry directorio[SIZE];
    int entradasTotales = 0;
    int fat[SIZE];
    char unidadDeMemoria[SIZE];
    bool entregaAleatoria = false;
    // Usado en algunos metodos recursivos
    std::string strtemp;
    bool seguro = false;

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
public:
    /*
     * Constructor
     */
    FileSystem();

    void usarAsignacionAleatoria(bool usar);

    /* retorna 0 si todo bien
     * si no, hubo problema
     */
    int crear(std::string nombre);

    /* retorna 0 si todo bien
     * si no, hubo problema
     */
    int escribir(std::string nombre, int index, char caracter);
    
	/* retorna 0 si todo bien
     * si no, hubo problema
     */
    int agregar(std::string nombre, char caracter);    
    int eliminar(std::string nombre, bool seguro);
    
	/* retorna 0 si todo bien
     * si no, hubo problema
     */
    int agregar(std::string nombre, std::string str);
    void imprimirStrings();
    void imprimirDirectorio();
    void imprimirFAT(int cols);
    void imprimirMemoria(int cols);
    void imprimirUnidad(int cols);    
};

