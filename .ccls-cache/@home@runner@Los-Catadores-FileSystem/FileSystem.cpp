#include "FileSystem.hpp"

/* Retorna primera posicion de memoria sin uso.
 * Si no hay espacio en memoria, retorna -1;
 */
int FileSystem::encontrarBloqueVacio() {
	if(entregaAleatoria)
		return encontrarBloqueVacioAleatorio();
	else
		return encontrarBloqueVacioSecuencial();
};

int FileSystem::encontrarBloqueVacioSecuencial() {
	for(int i = 0; i < SIZE; i++) {
		if(fat[i] == -1)
			return i;
	}
	return err("Memoria llena!", -1);
};

int FileSystem::encontrarBloqueVacioAleatorio() {
	int i;
	int intentos = 150;
	do {
		i = random(0, SIZE);
		intentos--;
		if(intentos < 0) return encontrarBloqueVacioSecuencial();
	} while(fat[i] != -1);
	return i;
};

int FileSystem::encontrarUltimoFAT(int bloqueInicial) {
	if( fat[bloqueInicial] == bloqueInicial )
		return bloqueInicial;
	return encontrarUltimoFAT( fat[bloqueInicial] );
};

int FileSystem::encontrarIndiceFAT(int bloqueInicial, int indice, int count) {
	if( count == indice ) {
		return bloqueInicial;
	}
	return encontrarIndiceFAT( fat[bloqueInicial], indice, count+1);
};

/* 
 * Retorna el string empezando en lugar dado de la memoria  
 */
std::string FileSystem::stringDesde(int bloqueInicial) {
	if(bloqueInicial == -1) return "";
	strtemp.clear();
	stringDesdeRecursivo(bloqueInicial);
	return strtemp;
};

int FileSystem::stringDesdeRecursivo(int bloque) {
	strtemp += unidadDeMemoria[bloque];
	if( fat[bloque] == bloque )
		return bloque;
	return stringDesdeRecursivo( fat[bloque] );
};

int FileSystem::eliminarRecursivo(int bloque) {
	int temp = fat[bloque];
	fat[bloque] = -1;
	if(seguro) unidadDeMemoria[bloque] = ' ';
	if( temp == bloque )
		return bloque;
	return eliminarRecursivo( temp );
};

int FileSystem::getEntryIndex(std::string name) {
	int entryIndex = -1;
	// buscar el nombre en la base de datos (entradas)
	for(int i = 0; i < entradasTotales; i++)
	{
		if(name.compare(directorio[i].nombre) == 0)
		{
			entryIndex = i;
			break;
		}
	}
	return entryIndex;
};

/*
 * Constructor
 */
FileSystem::FileSystem() {
	for(int i = 0; i < SIZE; i++)
	{
		// -1 significa que el bloque esta en desuso
		fat[i] = -1;
		// vacio
		unidadDeMemoria[i] = ' ';
	}
};

void FileSystem::usarAsignacionAleatoria(bool usar) {
	entregaAleatoria = usar;
};

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::crear(std::string nombre) {
	//std::cerr << "crear: " << nombre << "\n";
	if(entradasTotales>=SIZE)
		return err("Sin espacio para: " + (std::string)nombre, 1);
	int entryIndex = getEntryIndex(nombre);
	if(entryIndex!=-1) return err("Ya existe " + nombre, 2);
	
	directorio[entradasTotales].nombre = nombre;
	// el -1 aqui sifnifica que no tiene un bloque asignado
	directorio[entradasTotales].bloqueInicial = -1;
	directorio[entradasTotales].fechaCreacion = std::time(nullptr);
	entradasTotales++;
	return 0;
};

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::escribir(std::string nombre, int index, char caracter)
{
	int entryIndex = getEntryIndex(nombre);
	if(entryIndex==-1) return err("No existe "+nombre+". Abortando escritura de "+chToStr(caracter)+".", 1);
	std::string str = stringDesde(directorio[entryIndex].bloqueInicial);
	if(index>=str.length()) return err("out of bounds", 2);

	int indexEnMemoria = encontrarIndiceFAT(
							directorio[entryIndex].bloqueInicial,
							index, 0);

	unidadDeMemoria[indexEnMemoria] = caracter;
	
	return 0;
};

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::agregar(std::string nombre, char caracter)
{
	int entryIndex = getEntryIndex(nombre);
	if(entryIndex==-1)
		return err("No existe "+
					nombre+". Abortando intento de agregar "
					+chToStr(caracter)+".", 1);
	// index del bloque nuevo
	int bloque = encontrarBloqueVacio();
	if(bloque==-1)
		return err("Sin espacio para char: "+chToStr(caracter),2);
	
	// Asignando un nombre al entry por primera vez?
	if(directorio[entryIndex].bloqueInicial == -1)
	{
		directorio[entryIndex].bloqueInicial = bloque;
	}
	else
	{
		// Modificar el penultimo FAT,
		// para que apunte al bloque nuevo.
		int ini = directorio[entryIndex].bloqueInicial;
		int penultimo = encontrarUltimoFAT(ini);
		fat[penultimo] = bloque;
	}
	// Apuntamos FAT del bloque a si mismo (para que sea el ultimo)
	fat[bloque] = bloque;
	// editar ese ultimo bloque de memoria
	unidadDeMemoria[bloque] = caracter;
	
	return 0;
};

int FileSystem::eliminar(std::string nombre, bool seguro) {
	int entryIndex = getEntryIndex(nombre);
	if(entryIndex==-1) return err("No existe " + nombre, 1);

	// Eliminar del FAT
	this->seguro = seguro;
	eliminarRecursivo(directorio[entryIndex].bloqueInicial);

	// Eliminar de directorio
	entradasTotales--;
	
	return 0;
};

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::agregar(std::string nombre, std::string str) {
	for(int i = 0; i < str.length(); i++) {
		int ret = agregar(nombre, (char)str[i]);    
	}
	return 0;
};


void FileSystem::imprimirStrings() {
	std::cout << RESET << BOLD << UNDERLINE;
	std::cout << "Strings:" << "\n";
	std::cout << RESET;
	for(int i = 0; i < entradasTotales; i++)
	{
		Entry entrada = directorio[i];
		std::cout << entrada.nombre << ": ";
		std::cout << stringDesde(entrada.bloqueInicial);
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
};

void FileSystem::imprimirDirectorio() {
	std::cout << RESET << BOLD;
	std::cout << "Directorio:" << "\n";
	std::cout << " Índice| Nombre| Bloque Inicial| Creación" << "\n";
	std::cout << RESET;
	for(int i = 0; i < entradasTotales; i++)
	{
		std::cout << std::setw(7) << i << "|";
		std::cout << std::setw(7) << directorio[i].nombre << "|";
		int ini = directorio[i].bloqueInicial;
		std::string inistr = std::to_string(ini);
		if(ini==-1) inistr += " (n/a)";
		std::cout << std::setw(15) << inistr << "|";
		std::cout << std::setw(9) << directorio[i].fechaCreacion;
		std::cout << "\n";   
	}
	std::cout << "\n";
};

void FileSystem::imprimirFAT(int cols) {
	int espacio = 3;
	std::cout << RESET << BOLD;
	std::cout << "File Allocation Table (FAT):" << "\n";
	for(int i = 0; i < SIZE; i++) {
		std::cout << RESET << FAINT;
		std::cout << std::setw(espacio) << i;
		std::cout << RESET << BOLD;
		std::cout << std::setw(espacio) << fat[i];
		std::cout << RESET << FAINT;
		std::cout << "|";
		
		if( (i+1)%cols == 0) std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
};

void FileSystem::imprimirMemoria(int cols) {
	int espacio = 3;
	std::cout << RESET << BOLD;
	std::cout << "Unidad De Memoria:" << "\n";
	for(int i = 0; i < SIZE; i++) {
		std::cout << RESET << FAINT;
		std::cout << std::setw(espacio) << i;
		std::cout << RESET << BOLD;
		std::cout << std::setw(espacio) << unidadDeMemoria[i];
		std::cout << RESET << FAINT;
		std::cout << "|";
		
		if( (i+1)%cols == 0) std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
};

void FileSystem::imprimirUnidad(int cols) {
	imprimirDirectorio();
	imprimirFAT(cols);
	imprimirMemoria(cols);
	std::cout << "\n";
}