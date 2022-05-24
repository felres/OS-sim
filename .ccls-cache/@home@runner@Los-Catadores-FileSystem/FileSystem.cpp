#include "FileSystem.hpp"


/// ******************************************
/// PRIVATE METHODS
/// ******************************************

/* Retorna primera posicion de memoria sin uso.
 * Si no hay espacio en memoria, retorna -1;
 */
int FileSystem::encontrarBloqueVacio()
{
	if(entregaAleatoria)
		return encontrarBloqueVacioAleatorio();
	else
		return encontrarBloqueVacioSecuencial();
};

int FileSystem::encontrarBloqueVacioSecuencial()
{
	for(int i = 0; i < SIZE; i++) {
		if(fat[i] == -1)
			return i;
	}
	return err("Memoria llena!", -1);
};

int FileSystem::encontrarBloqueVacioAleatorio()
{
	int i;
	int intentos = 150;
	do {
		i = random(0, SIZE);
		intentos--;
		if(intentos < 0) return encontrarBloqueVacioSecuencial();
	} while(fat[i] != -1);
	return i;
};

int FileSystem::encontrarUltimoFAT(int bloqueInicial)
{
	if( fat[bloqueInicial] == bloqueInicial )
		return bloqueInicial;
	return encontrarUltimoFAT( fat[bloqueInicial] );
};

int FileSystem::encontrarIndiceFAT(int bloqueInicial, int indice, int count)
{
	if( count == indice ) {
		return bloqueInicial;
	}
	return encontrarIndiceFAT( fat[bloqueInicial], indice, count+1);
};

/* 
 * Retorna el string empezando en lugar dado de la memoria  
 */
std::string FileSystem::stringDesde(int bloqueInicial)
{
	if(bloqueInicial == -1) return "";
	strtemp.clear();
	stringDesdeRecursivo(bloqueInicial);
	return strtemp;
};

int FileSystem::stringDesdeRecursivo(int bloque)
{
	strtemp += unidadDeMemoria[bloque];
	if( fat[bloque] == bloque )
		return bloque;
	return stringDesdeRecursivo( fat[bloque] );
};

int FileSystem::eliminarRecursivo(int bloque)
{
	int temp = fat[bloque];
	fat[bloque] = -1;
	if(seguro) unidadDeMemoria[bloque] = ' ';
	if( temp == bloque )
		return bloque;
	return eliminarRecursivo( temp );
};

/*
 * Find user with name, else return -1.
 */
int FileSystem::getUserIndex(std::string name)
{
    int index = -1;
	for(int i = 0; i < userCount; i++)
	{
		if(name.compare(users[i].nombre) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
};

/*
 * Find group with name, else return -1.
 */
int FileSystem::getGroupIndex(std::string name)
{
    int index = -1;
	for(int i = 0; i < groupCount; i++)
	{
		if(name.compare(groups[i].nombre) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
};

/*
 * Find file with name, else return -1.
 */
int FileSystem::getEntryIndex(std::string name)
{
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

std::string FileSystem::getUserNameWithId(int userId)
{
	for(int i = 0; i < userCount; i++)
	{
		if(users[i].id == userId)
		{
			return users[i].nombre;
		}
	}
	return "ERROR??";
};

std::string FileSystem::getUserNameAt(int index)
{
    return users[index].nombre;
};

std::string FileSystem::getGroupNameAt(int index)
{
    return groups[index].nombre;
};

Group FileSystem::getGroupWithId(int groupId)
{
    for(int i = 0; i < groupCount; i++)
    {
        if(groups[i].id == groupId)
            return groups[i];
    }
    err("ERROR, VERY WEIRD!", 1);
    return *(new Group());
};

/// ******************************************
/// PUBLIC METHODS
/// ******************************************

/*
 * Constructor
 */
FileSystem::FileSystem()
{
	directorio.reserve(SIZE);
	fat.resize(SIZE, -1);
	unidadDeMemoria.resize(SIZE);

    // "factory reset"
    // Add root group, user and set
    createGroup("root");
    createUser("root", "root", "");
    switchUser("root");
    
};

std::string FileSystem::getName()
{
    return getUserNameAt(currentUserIndex) + "@" +
            getGroupNameAt(users[currentUserIndex].primaryGroupId);
};

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::createGroup(std::string name)
{
    if(groupCount>=SIZE)
		return err("Sin espacio para grupo: " + (std::string)name, 1);
	int groupIndex = getGroupIndex(name);
	if(groupIndex!=-1) return err("Ya existe grupo " + name, 2);

    groups[groupCount].id = groupCount;
    groups[groupCount].nombre = name;
    groups[groupCount].memberCount = 0;
    
    groupCount++;
    return 0;
};

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::crear(std::string nombre)
{
	//std::cerr << "crear: " << nombre << "\n";
	if(directorio.size()>=SIZE)
		return err("Sin espacio para: " + (std::string)nombre, 1);
	int entryIndex = getEntryIndex(nombre);
	if(entryIndex!=-1) return err("Ya existe file " + nombre, 2);
    if(currentUserIndex == -1) return err("No user selected.", 3);
    directorio.resize(directorio.size()+1);
	directorio[directorio.size()-1].nombre = nombre;
	// el -1 aqui sifnifica que no tiene un bloque asignado
	directorio[directorio.size()-1].bloqueInicial = -1;
	directorio[directorio.size()-1].fechaCreacion = std::time(nullptr);
    // permission stuff
    directorio[directorio.size()-1].ownerId = currentUserIndex;
    directorio[directorio.size()-1].groupId = users[currentUserIndex].primaryGroupId;
    directorio[directorio.size()-1].ownerCanWrite = true;
    directorio[directorio.size()-1].ownerCanRead = true;
    directorio[directorio.size()-1].ownerCanExecute = true;
    directorio[directorio.size()-1].groupCanWrite = false;
    directorio[directorio.size()-1].groupCanRead = true;
    directorio[directorio.size()-1].groupCanExecute = false;
    directorio[directorio.size()-1].publicCanWrite = false;
    directorio[directorio.size()-1].publicCanRead = false;
    directorio[directorio.size()-1].publicCanExecute = false;
    
	entradasTotales++;
	return 0;
};

int FileSystem::getPrimaryMemberIdsInGroup(int groupId, int *buffer)
{
    int len = 0;
    for(int i = 0; i < userCount; i++)
    {
        if(users[i].primaryGroupId == groupId)
        {
            buffer[len] = users[i].id;
            len++;
        }
    }
    return len;
};

void FileSystem::printFiles()
{
    std::cout << RESET << BOLD;
	std::cout << "    Flags|   Owner|   Group|Size|        Date|    Name" << "\n";
	std::cout << RESET;
	for(int i = 0; i < entradasTotales; i++)
	{
		Entry entrada = directorio[i];
        std::string flags;
        if(entrada.ownerCanWrite) flags+="w"; else flags+="-";
        if(entrada.ownerCanRead) flags+="r"; else flags+="-";
        if(entrada.ownerCanExecute) flags+="x"; else flags+="-";
        if(entrada.groupCanWrite) flags+="w"; else flags+="-";
        if(entrada.groupCanRead) flags+="r"; else flags+="-";
        if(entrada.groupCanExecute) flags+="x"; else flags+="-";
        if(entrada.publicCanWrite) flags+="w"; else flags+="-";
        if(entrada.publicCanRead) flags+="r"; else flags+="-";
        if(entrada.publicCanExecute) flags+="x"; else flags+="-";
        std::cout << flags << "|";
		std::cout << std::setw(8) << getUserNameAt(entrada.ownerId) << "|";
        std::cout << std::setw(8) << getGroupNameAt(entrada.groupId) << "|";
        std::string contents = stringDesde(entrada.bloqueInicial);
		std::cout << std::setw(4) << contents.length() << "|";
        std::cout << std::setw(12) << entrada.fechaCreacion << "|";
        std::cout << std::setw(8) << entrada.nombre;
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
}

void FileSystem::printUsers()
{
    std::cout << RESET << BOLD;
	std::cout << " Id|    Name|        Password| Primary Group" << "\n";
	std::cout << RESET;
	for(int i = 0; i < userCount; i++)
	{
        std::cout << std::setw(3) << users[i].id << "|";
        std::cout << std::setw(8) << users[i].nombre << "|";
        std::cout << std::setw(16) << users[i].password << "|";
        std::cout << std::setw(14)
            << getGroupNameAt(users[i].primaryGroupId);
        std::cout << "\n";
    }
    std::cout << "\n";
	std::cout << RESET;
}

/*
 * Display all members including those who have group as primary
 */
void FileSystem::printGroups()
{
    std::cout << RESET << BOLD;
	std::cout << " Id|    Name| Users" << "\n";
	std::cout << RESET;
	for(int i = 0; i < groupCount; i++)
	{
        std::cout << std::setw(3) << groups[i].id << "|";
        std::cout << std::setw(8) << groups[i].nombre << "|";
        // primary members
        int arr[SIZE];
        int arrlen = getPrimaryMemberIdsInGroup(groups[i].id, arr);
        for(int j = 0; j < arrlen; j++)
        {
            std::cout << RESET << BOLD;
            std::cout << getUserNameWithId(arr[j]) << ", ";
            std::cout << RESET;
        }
        // secondary members
        for(int j = 0; j < groups[i].memberCount; j++)
        {
            std::cout << std::setw(3)
                << getUserNameAt(groups[i].memberIdList[j]) << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
	std::cout << RESET;
}

bool FileSystem::canCurrentUserModifyFile(std::string filename)
{
    int fileId = getEntryIndex(filename);
    Entry fileInfo = directorio[fileId];
    if(currentUserIndex==0) return true; //root
    if(fileInfo.publicCanWrite) return true;
    if((fileInfo.ownerCanWrite)
        &&(currentUserIndex==fileInfo.ownerId)) return true;
    if(fileInfo.groupCanWrite)
    {
        // primary group
        if(users[currentUserIndex].primaryGroupId==fileInfo.groupId)
            return true;
        Group currentGroup = getGroupWithId(users[currentUserIndex].primaryGroupId);
        // secondary members
        for(int i = 0; i < currentGroup.memberCount; i++)
        {
            if(currentGroup.memberIdList[i] == users[currentUserIndex].id)
                return true;
        }
    }
    return false;
};

bool FileSystem::canCurrentUserReadFile(std::string filename)
{
    int fileId = getEntryIndex(filename);
    Entry fileInfo = directorio[fileId];
    if(currentUserIndex==0) return true; //root
    if(fileInfo.publicCanRead) return true;
    if((fileInfo.ownerCanRead)&&(currentUserIndex==fileInfo.ownerId))
        return true;
    if(fileInfo.groupCanRead)
    {
        // primary group
        if(users[currentUserIndex].primaryGroupId==fileInfo.groupId)
            return true;
        Group currentGroup = getGroupWithId(users[currentUserIndex].primaryGroupId);
        // secondary members
        for(int i = 0; i < currentGroup.memberCount; i++)
        {
            if(currentGroup.memberIdList[i] == users[currentUserIndex].id)
                return true;
        }
    }
    return false;
};

/// ******************************************
/// METHODS THAT NEED PASSWORDS
/// ******************************************

int FileSystem::switchUser(std::string name)
{
    int userIndex = getUserIndex(name);
	if(userIndex==-1) return err("No existe user " + name, 2);
    // pedir clave
    std::cout << "(password request WIP)\n";
    currentUserIndex = userIndex;
    return 0;
};

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::createUser(std::string name, std::string groupName, std::string password)
{
    if(userCount>=SIZE)
		return err("Sin espacio para user: " + (std::string)name, 1);
	int userIndex = getUserIndex(name);
	if(userIndex!=-1) return err("Ya existe user " + name, 2);
    int groupIndex = getGroupIndex(groupName);
    if(groupIndex==-1) return err("No existe grupo " + groupName, 3);
        
    users[userCount].id = userCount;
    users[userCount].nombre = name;
    users[userCount].password = password;
    users[userCount].primaryGroupId = groupIndex;
    
    userCount++;
    return 0;
};

/// ******************************************
/// METHODS THAT CHECK FILE PERMISSIONS
/// ******************************************

int FileSystem::changemode(std::string filename, int flagindex, bool b)
{
    int entryIndex = getEntryIndex(filename);
	if(entryIndex==-1) return err("File "+filename+" does not exist.", 1);
    if(!canCurrentUserModifyFile(filename))
        return err("User cannot modify " + filename, 2);
    if((flagindex < 0)||(flagindex >= 9))
        return err("Incorrect flag index, use [0-8] inclusive.", 3);
    
    switch(flagindex)
    {
        case 0: directorio[entryIndex].ownerCanWrite = b; break;
        case 1: directorio[entryIndex].ownerCanRead = b; break;
        case 2: directorio[entryIndex].ownerCanExecute = b; break;
        case 3: directorio[entryIndex].groupCanWrite = b; break;
        case 4: directorio[entryIndex].groupCanRead = b; break;
        case 5: directorio[entryIndex].groupCanExecute = b; break;
        case 6: directorio[entryIndex].publicCanWrite = b; break;
        case 7: directorio[entryIndex].publicCanRead = b; break;
        case 8: directorio[entryIndex].publicCanExecute = b; break;
    }
    
    return 0;
};

int FileSystem::readFile(std::string filename)
{
    if(currentUserIndex == -1) return err("No user selected.", 1);
    int entryIndex = getEntryIndex(filename);
    if(entryIndex==-1) return err("File " + filename + " does not exist.", 2);
    if(!canCurrentUserReadFile(filename))
        return err("User cannot read " + filename, 3);

    std::cout << "\""
                << stringDesde(directorio[entryIndex].bloqueInicial)
                << "\"\n";
    
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
    if(!canCurrentUserModifyFile(nombre))
        return err("User cannot modify " + nombre, 3);
        
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
    if(!canCurrentUserModifyFile(nombre))
        return err("User cannot modify " + nombre, 3);
    
	// Asignando un nombre al entry por primera vez?
	if(directorio[entryIndex].bloqueInicial == -1)
	{
		directorio[entryIndex].bloqueInicial = bloque;
	}
	else
	{
		// Modificar el penultimo FAT.
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

/* retorna 0 si todo bien
 * si no, hubo problema
 */
int FileSystem::agregar(std::string nombre, std::string str)
{
    for(int i = 0; i < str.length(); i++) {
        if(agregar(nombre, (char)str[i])!= 0)
        {
            return err("Failed to add char.", 1);
            break;
        }
	}
	return 0;
};

int FileSystem::eliminar(std::string nombre, bool seguro)
{
	int entryIndex = getEntryIndex(nombre);
	if(entryIndex==-1) return err("No existe " + nombre, 1);

    if(!canCurrentUserModifyFile(nombre))
        return err("User cannot modify " + nombre, 3);
    
	// Eliminar del FAT
	this->seguro = seguro;
	eliminarRecursivo(directorio[entryIndex].bloqueInicial);

	// Eliminar de directorio
	directorio.erase(directorio.begin()+ entryIndex);
    entradasTotales--;
	
	return 0;
};

/// DEBUG METHODS

void FileSystem::usarAsignacionAleatoria(bool usar)
{
	entregaAleatoria = usar;
};

void FileSystem::imprimirStrings()
{
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

void FileSystem::imprimirDirectorio()
{
	std::cout << RESET << BOLD;
	std::cout << "Directorio:" << "\n";
	std::cout << " Índice|    Nombre| Bloque Inicial| Contenido" << "\n";
	std::cout << RESET;
	for(int i = 0; i < directorio.size(); i++)
	{
		std::cout << std::setw(7) << i << "|";
		std::cout << std::setw(10) << directorio[i].nombre << "|";
		int ini = directorio[i].bloqueInicial;
		std::string inistr = std::to_string(ini);
		if(ini==-1) inistr += " (n/a)";
		std::cout << std::setw(15) << inistr << "|";
        std::cout << " " << stringDesde(directorio[i].bloqueInicial);
		std::cout << "\n";
	}
	std::cout << "\n";
};

void FileSystem::imprimirFAT(int cols)
{
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

void FileSystem::imprimirMemoria(int cols)
{
	int espacio = 3;
	std::cout << RESET << BOLD;
	std::cout << "Unidad De Memoria:" << "\n";
	for(int i = 0; i < SIZE; i++) {
		std::cout << RESET << FAINT;
		std::cout << std::setw(espacio) << i;
		std::cout << RESET << BOLD;
		if (!unidadDeMemoria[i])
			std::cout << std::setw(espacio) << " ";	
		else 
			std::cout << std::setw(espacio) << unidadDeMemoria[i];
		std::cout << RESET << FAINT;
		std::cout << "|";
		
		if( (i+1)%cols == 0) std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
};

void FileSystem::imprimirUnidad(int cols)
{
    imprimirDirectorio();
    imprimirFAT(cols);
	imprimirMemoria(cols);
	std::cout << "\n";
};

int FileSystem::loadTest()
{
    // en teoria se ejecuta luego de agregar el root
    createGroup("students");
    createUser("alice", "students", "");
    switchUser("alice");
    crear("a.txt");
    agregar("a.txt", "ants!");
    createUser("bob", "students", "");
    switchUser("bob");
    crear("b.txt");
    agregar("b.txt", "birds!");
    createGroup("teachers");
    createUser("carol", "teachers", "");
    switchUser("carol");
    crear("c.txt");
    agregar("c.txt", "cats!");
    crear("board.txt");
    agregar("board.txt", "dsjkfhakls");
    changemode("board.txt", 0, true);
    changemode("board.txt", 1, true);
    changemode("board.txt", 2, true);
    changemode("board.txt", 3, true);
    changemode("board.txt", 4, true);
    changemode("board.txt", 5, true);
    changemode("board.txt", 6, true);
    changemode("board.txt", 7, true);
    changemode("board.txt", 8, true);
    createGroup("guests");
    createUser("dave", "guests", "");
    switchUser("dave");
    crear("d.txt");
    agregar("d.txt", "dragons!");
    return 0;
};