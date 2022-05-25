/* *****************************************************************************
 * 
 * Ronald León Morales B33733
 * Tonny Ortiz Salazar B35054
 * Felipe Vega Vindas B78244
 * Andrés Zúñiga B68014
 * Javier Donato B92650
 *
 * Documentation guide: https://developer.lsst.io/cpp/api-docs.html
 * 
 * To Do:
 * - Comentar codigo (IN ENGLISH)
 * - terminar transición de arrays a std::vectors
 * - rename directorio to files
 * - distinguish between index and id
 */

#include <iostream>
#include "FileSystem.hpp"
#include "helpers.cpp"
#include "Interface.hpp"



/**
 * Creates and executes user interface.
 */
int main() {
    // random seed
    srand( time(NULL) );
    
    FileSystem *fs = new FileSystem();
	Interface console;
	console.run(fs);
}