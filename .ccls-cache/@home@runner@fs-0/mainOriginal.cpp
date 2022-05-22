#include <iostream>

//RESTRICCIONES
//1. bloques son de 1 caracter
//2. Unidad es una matriz de caracteres
//3. Tamaño máximo 100 caracteres, por el momento
//4. Usando FAT
//5. Usando un struct para el directorio (nombre, bloque inicial, fecha, por el momento)

class FS
{
  // vars
  private:
  std::string nombre = "";

  // methods
  public: 
  void crear(std::string nombre){};
  void agregar(std::string hilera, char caracter){};
  void imprimirUnidad(){};
};


int main() {
  FS *fs = new FS() ;

  fs->crear("a.dat") ;
  fs->agregar("a.dat",'a') ;
  fs->imprimirUnidad() ; //imprime unidad, directorio y tabla fat

  fs->crear("b.dat") ;
  fs->agregar("b.dat", 'b') ;
  fs->imprimirUnidad() ;
  fs->agregar("b.dat",'c') ;
  fs->imprimirUnidad() ;
  
}



/*  IMPRIMIR UNIDAD al final

Directorio:
"a.dat"   0
"b.dat"   1

FAT
x 2 x
0 1 2 

3 4 5 
  
6 7 8



Cla/*

UNIDAD
a b 2
_ _ _
_ _ _
  
*/
  