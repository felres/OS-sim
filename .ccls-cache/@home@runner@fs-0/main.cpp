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


int main() {
    // random seed
    srand( time(NULL) );
    
    FileSystem *fs = new FileSystem();
    fs->usarAsignacionAleatoria(false);

    /*
    for(int i = 0; i < 10; i++)
    {
        fs->crear( std::to_string(i)+".dat" );
    }
    */
    
    fs->crear("a.dat");
    fs->agregar("a.dat","hola");
    /*
    fs->crear("b.dat");
    fs->agregar("b.dat", "comieron trigo en un trigal!");
    fs->eliminar("b.dat", false);

    fs->crear("c.dat");
    fs->agregar("c.dat", "murieron");
    fs->escribir("c.dat", 2, 't');
    fs->escribir("c.dat", 5, 'b');

    fs->crear("nombre");
    fs->agregar("nombre", "mi nombre es pablo diego francisco de san juan vasquez de la trinidad ruiz");
    */
    
    fs->imprimirUnidad(10) ;
    fs->imprimirStrings();
}
  