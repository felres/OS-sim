/* *****************************************************************************
 * 
 * Ronald León Morales B33733
 * Tonny Ortiz Salazar B35054
 * Felipe Vega Vindas B78244
 * Andrés Zúñiga B68014
 * Javier Donato B92650
 * 
 */

/**
 * TODO:
 * - Agregar clase para manejar permisos 
 * - Separar programa en clases
 */

#include <iostream>
#include <stdio.h>
#include "FileSystem.hpp"

//ELEMENTOS DEL PROGRAMA
//1. Bloques son de 1 caracter
//2. Unidad es un arreglo de caracteres
//3. Tamaño 100 caracteres, por el momento
//4. Usando FAT
//5. Usando un struct para el directorio (nombre, bloque inicial, fecha, por el momento)


int tokenize(std::string s, std::string *buffer)
{
    std::stringstream ss(s);
    std::string word;
    int len = 0;
    while (ss >> word) {
        buffer[len] = word;
        ++len;
    }
    return len;
}

std::string process(std::string line, FileSystem *fs)
{
    std::string argv[SIZE];
    int argc = tokenize(line, argv);

    if(argc > 0)
    {
        if(argv[0] == "print")
        {
            fs->imprimirUnidad(10);
            return "";
        }
        else if(argv[0] == "touch")
        {
            if(argc > 1)
            {
                fs->crear(argv[1]);
                return "Created file " + argv[1] + ".\n";
            }
            else
                return "No file name provided."
        }
    }
    
    return "";
}

int main() {
    // random seed
    srand( time(NULL) );
    
    FileSystem *fs = new FileSystem();

    while(true)
    {
        std::string input;
        std::cout << RESET << BOLD << GREEN;
        std::cout << "~$ ";
        std::cout << RESET;
        std::getline(std::cin, input);
        std::cout << process(input, fs);
    }
    
}
  