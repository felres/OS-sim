/* *****************************************************************************
 * 
 * Ronald León Morales B33733
 * Tonny Ortiz Salazar B35054
 * Felipe Vega Vindas B78244
 * Andrés Zúñiga B68014
 * Javier Donato B92650
 * 
 * To Do:
 * - Comentar codigo
 * - terminar transición de arrays a std::vectors
 * - 
 */

#include <iostream>
#include "FileSystem.hpp"
#include "helpers.cpp"
#include "Interface.hpp"

//ELEMENTOS DEL PROGRAMA
//1. Bloques son de 1 caracter
//2. Unidad es un arreglo de caracteres
//3. Tamaño SIZE caracteres, por el momento
//4. Usando FAT
//5. Usando un struct para el directorio (nombre, bloque inicial, fecha, por el momento)
//6. Permisos




int main() {
    // random seed
    srand( time(NULL) );
    
    FileSystem *fs = new FileSystem();
	Interface consola;
	consola.run(fs);
}
  