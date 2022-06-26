#include "FileSystem.hpp"
#include "Helpers.cpp"

/// ******************************************
/// PRIVATE METHODS
/// ******************************************

int FileSystem::createUserBypass(std::string name, std::string groupName, std::string password)
{
    if(userCount>=SIZE)
		return err("Sin espacio para user: " + (std::string)name, 1);
	int userIndex = getUserIndex(name);
	if(userIndex!=-1) return err("Ya existe user " + name, 2);
    int groupIndex = getGroupIndex(groupName);
    if(groupIndex==-1) return err("No existe grupo " + groupName, 3);
    
    users[userCount].id = userCount;
    users[userCount].name = name;
    users[userCount].password = password;
    users[userCount].primaryGroupId = groupIndex;
    
    userCount++;
    return 0;
};

std::string FileSystem::encrypt(std::string str)
{
    return ceasarShift(str, +1);
};

std::string FileSystem::decrypt(std::string str)
{
    return ceasarShift(str, -1);
};

int FileSystem::getAvailableBlock()
{
	if(randomizedBlockSearching)
		return getAvailableBlockRandom();
	else
		return getAvailableBlockSequential();
};

int FileSystem::getAvailableBlockSequential()
{
	for(int i = 0; i < SIZE; i++) {
		if(fat[i] == -1)
			return i;
	}
	return err("Memoria llena!", -1);
};

int FileSystem::getAvailableBlockRandom()
{
	int i;
	int tries = 150;
	do {
		i = random(0, SIZE);
		tries--;
		if(tries < 0) return getAvailableBlockSequential();
	} while(fat[i] != -1);
	return i;
};

int FileSystem::getLastBlockInChainFrom(int blockIndex)
{
	if( fat[blockIndex] == blockIndex )
		return blockIndex;
	return getLastBlockInChainFrom( fat[blockIndex] );
};

int FileSystem::getBlockInChainAtIndex(int blockIndex, int index, int count)
{
	if( count == index ) {
		return blockIndex;
	}
	return getBlockInChainAtIndex( fat[blockIndex], index, count+1);
};

std::string FileSystem::getStringFrom(int blockIndex)
{
	if(blockIndex == -1) return "";
	strtemp.clear();
	getStringFromRecursive(blockIndex);
	return strtemp;
};

int FileSystem::getStringFromRecursive(int blockIndex)
{
	strtemp += unidadDeMemoria[blockIndex];
	if( fat[blockIndex] == blockIndex )
		return blockIndex;
	return getStringFromRecursive( fat[blockIndex] );
};

int FileSystem::removeFileRecursive(int blockIndex)
{
	int temp = fat[blockIndex];
	fat[blockIndex] = -1;
	if(secureDeletion) unidadDeMemoria[blockIndex] = ' ';
	if( temp == blockIndex )
		return blockIndex;
	return removeFileRecursive( temp );
};

int FileSystem::getUserIndex(std::string name)
{
    int index = -1;
	for(int i = 0; i < userCount; i++)
	{
		if(name.compare(users[i].name) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
};

int FileSystem::getGroupIndex(std::string name)
{
    int index = -1;
	for(int i = 0; i < groupCount; i++)
	{
		if(name.compare(groups[i].name) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
};

int FileSystem::getFileIndex(std::string name)
{
	int entryIndex = -1;
	// buscar el nombre en la base de datos (entradas)
	for(int i = 0; i < directorio.size(); i++)
	{
		if(name.compare(directorio[i].name) == 0)
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
			return users[i].name;
		}
	}
	return "ERROR??";
};

std::string FileSystem::getFileNameAt(int index)
{
    return directorio[index].name;
};

std::string FileSystem::getUserNameAt(int index)
{
    return users[index].name;
};

std::string FileSystem::getGroupNameAt(int index)
{
    return groups[index].name;
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

bool FileSystem::canCurrentUserExecuteFile(std::string filename)
{
    int fileId = getFileIndex(filename);
    File fileInfo = directorio[fileId];
    if(currentUserIndex==0) return true; //root
    if(fileInfo.publicCanExecute) return true;
    if((fileInfo.ownerCanExecute)
        &&(currentUserIndex==fileInfo.ownerId)) return true;
    if(fileInfo.groupCanExecute)
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

bool FileSystem::canCurrentUserModifyFile(std::string filename)
{
    int fileId = getFileIndex(filename);
    File fileInfo = directorio[fileId];
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
    int fileId = getFileIndex(filename);
    File fileInfo = directorio[fileId];
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

/// ******************************************
/// PUBLIC METHODS
/// ******************************************

FileSystem::FileSystem()
{
	directorio.reserve(SIZE);
	fat.resize(SIZE, -1);
	unidadDeMemoria.resize(SIZE);

    // "factory reset"
    // Add root group, user and set
    createGroup("root");
    createUserBypass("root", "root", "");
    switchUser("root");
    
};

std::string FileSystem::getName()
{
    return getUserNameAt(currentUserIndex) + "@" +
            getGroupNameAt(users[currentUserIndex].primaryGroupId);
};

int FileSystem::createGroup(std::string name)
{
    if(groupCount>=SIZE)
		return err("Sin espacio para grupo: " + (std::string)name, 1);
	int groupIndex = getGroupIndex(name);
	if(groupIndex!=-1) return err("Ya existe grupo " + name, 2);

    groups[groupCount].id = groupCount;
    groups[groupCount].name = name;
    groups[groupCount].memberCount = 0;
    
    groupCount++;
    return 0;
};

int FileSystem::createFile(std::string name)
{
	if(directorio.size()>=SIZE)
		return err("Sin espacio para: " + (std::string)name, 1);
	int entryIndex = getFileIndex(name);
	if(entryIndex!=-1) return err("Ya existe file " + name, 2);
    if(currentUserIndex == -1) return err("No user selected.", 3);
    directorio.resize(directorio.size()+1);
	directorio[directorio.size()-1].name = name;
	directorio[directorio.size()-1].bloqueInicial = -1; //-1: no block assigned
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

int FileSystem::loadRealFile(std::string filename)
{
    std::vector<std::string> vec = readFileToVector(filename);
    if(vec.size() == 0) return err("Error trying to open file "+filename, 1);
    createFile(filename);
    for(int i = 0; i < vec.size(); i++)
    {
        appendToFile(filename, vec[i]);
        appendToFile(filename, "\n");
    }
    return 0;
};

void FileSystem::printFiles()
{
    std::cout << RESET << BOLD;
	std::cout << "    Flags|   Owner|   Group|Size|        Date|    Name" << "\n";
	std::cout << RESET;
	for(int i = 0; i < directorio.size(); i++)
	{
		File entrada = directorio[i];
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
        std::string contents = getStringFrom(entrada.bloqueInicial);
		std::cout << std::setw(4) << contents.length() << "|";
        std::cout << std::setw(12) << entrada.fechaCreacion << "|";
        std::cout << std::setw(8) << entrada.name;
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
}

void FileSystem::printUsers()
{
    std::cout << RESET << BOLD;
	std::cout << " Id|        Name|        Password(encrypted)|   Primary Group" << "\n";
	std::cout << RESET;
	for(int i = 0; i < userCount; i++)
	{
        std::cout << std::setw(3) << users[i].id << "|";
        std::cout << std::setw(12) << users[i].name << "|";
        std::cout << std::setw(27) << users[i].password << "|";
        std::cout << " ";
        std::cout << std::setw(15)
            << getGroupNameAt(users[i].primaryGroupId);
        std::cout << "\n";
    }
    std::cout << "\n";
	std::cout << RESET;
}

void FileSystem::printGroups()
{
    std::cout << RESET << BOLD;
	std::cout << " Id|           Name| Users" << "\n";
	std::cout << RESET;
	for(int i = 0; i < groupCount; i++)
	{
        std::cout << std::setw(3) << groups[i].id << "|";
        std::cout << std::setw(15) << groups[i].name << "|";
        // primary members
        int arr[SIZE];
        int arrlen = getPrimaryMemberIdsInGroup(groups[i].id, arr);
        std::cout << " ";
        for(int j = 0; j < arrlen; j++)
        {
            std::cout << RESET;
            std::cout << getUserNameWithId(arr[j]) << ", ";
            std::cout << RESET;
        }
        // secondary members
        for(int j = 0; j < groups[i].memberCount; j++)
        {
            std::cout << RESET;
            std::cout << std::setw(3)
                << getUserNameAt(groups[i].memberIdList[j]) << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
	std::cout << RESET;
}

/// ******************************************
/// METHODS THAT CHECK PASSWORDS
/// ******************************************

int FileSystem::changePassword()
{
    if(currentUserIndex == -1) return err("No user selected.", 1);
    std::string pass = users[currentUserIndex].password;
    if(pass=="")
    {
        // Set password for the first time
        std::cout << RESET << BOLD;
        std::cout << "Creating password for user "+getUserNameAt(currentUserIndex)+".\n";
        users[currentUserIndex].password = encrypt(requestPasswordRaw("Enter password: "));
    }
    else
    {
        // Change current password
        std::cout << RESET << BOLD;
        std::cout << "Changing password for user "+getUserNameAt(currentUserIndex)+".\n";
        if(encrypt(requestPasswordRaw("Current password: "))==pass)
        {
            users[currentUserIndex].password = encrypt(requestPasswordRaw("New password: "));
        }
        else
            return err("Wrong password!\n", 1);
    }
    return 0;
};

int FileSystem::switchUser(std::string name)
{
    int userIndex = getUserIndex(name);
	if(userIndex==-1) return err("User "+name+" does not exist.", 2);
    std::string pass = users[userIndex].password;
    if((pass!="")&&(encrypt(requestPasswordRaw("Enter password: "))!=pass))
        return err("Incorrect password!", 3);
    currentUserIndex = userIndex;
    return 0;
};

std::string FileSystem::requestPasswordRaw(std::string str)
{
    std::cout << RESET << BOLD;
    std::cout << str;
    std::cout << RESET;
    std::string input;
    std::getline(std::cin, input);
    return input;
};

int FileSystem::createUser(std::string name, std::string groupName)
{
    if(userCount>=SIZE)
		return err("Sin espacio para user: " + (std::string)name, 1);
	int userIndex = getUserIndex(name);
	if(userIndex!=-1) return err("Ya existe user " + name, 2);
    int groupIndex = getGroupIndex(groupName);
    if(groupIndex==-1) return err("No existe grupo " + groupName, 3);
    if(currentUserIndex!=0)
        return err("Only "+getUserNameAt(0)+" can create users", 4);
    
    std::string password = encrypt(requestPasswordRaw("New password for "+name+": "));
    
    users[userCount].id = userCount;
    users[userCount].name = name;
    users[userCount].password = password;
    users[userCount].primaryGroupId = groupIndex;
    
    userCount++;
    return 0;
};

/// ******************************************
/// METHODS THAT CHECK FILE PERMISSIONS
/// ******************************************

std::string FileSystem::requestFileContents(std::string filename)
{
    if(currentUserIndex == -1) return "";
    int entryIndex = getFileIndex(filename);
    if(entryIndex==-1) return "";
    if(!canCurrentUserReadFile(filename))
        return "";
    
    return getStringFrom(directorio[entryIndex].bloqueInicial);
}

int FileSystem::changemode(std::string filename, int flagindex, bool b)
{
    int entryIndex = getFileIndex(filename);
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
    int entryIndex = getFileIndex(filename);
    if(entryIndex==-1) return err("File " + filename + " does not exist.", 2);
    if(!canCurrentUserReadFile(filename))
        return err("User cannot read " + filename, 3);

    std::cout << GREEN << FAINT;
    std::cout << "↓ " << filename << "\n";
    std::cout << RESET;
    std::cout << getStringFrom(directorio[entryIndex].bloqueInicial);
    std::cout << GREEN << FAINT;
    std::cout << "↑ (EOF)" << "\n";
    std::cout << RESET;
    
    return 0;
};

int FileSystem::writeInFileAt(std::string name, int index, char caracter)
{
	int entryIndex = getFileIndex(name);
	if(entryIndex==-1) return err("No existe "+name+". Abortando escritura de "+chToStr(caracter)+".", 1);
	std::string str = getStringFrom(directorio[entryIndex].bloqueInicial);
	if(index>=str.length()) return err("out of bounds", 2);
    if(!canCurrentUserModifyFile(name))
        return err("User cannot modify " + name, 3);
        
	int indexEnMemoria = getBlockInChainAtIndex(
							directorio[entryIndex].bloqueInicial,
							index, 0);

	unidadDeMemoria[indexEnMemoria] = caracter;
	
	return 0;
};

int FileSystem::appendToFile(std::string name, char caracter)
{
	int entryIndex = getFileIndex(name);
	if(entryIndex==-1)
		return err("No existe "+
					name+". Abortando intento de agregar "
					+chToStr(caracter)+".", 1);
	// index del bloque nuevo
	int bloque = getAvailableBlock();
	if(bloque==-1)
		return err("Sin espacio para char: "+chToStr(caracter),2);
    if(!canCurrentUserModifyFile(name))
        return err("User cannot modify " + name, 3);
    
	// Asignando un name al entry por primera vez?
	if(directorio[entryIndex].bloqueInicial == -1)
	{
		directorio[entryIndex].bloqueInicial = bloque;
	}
	else
	{
		// Modificar el penultimo FAT.
		// para que apunte al bloque nuevo.
		int ini = directorio[entryIndex].bloqueInicial;
		int penultimo = getLastBlockInChainFrom(ini);
		fat[penultimo] = bloque;
	}
	// Apuntamos FAT del bloque a si mismo (para que sea el ultimo)
	fat[bloque] = bloque;
	// editar ese ultimo bloque de memoria
	unidadDeMemoria[bloque] = caracter;
	
	return 0;
};

int FileSystem::appendToFile(std::string name, std::string str)
{
    for(int i = 0; i < str.length(); i++) {
        if(appendToFile(name, (char)str[i])!= 0)
        {
            return err("Failed to add char.", 1);
            break;
        }
	}
	return 0;
};

int FileSystem::removeFile(std::string name, bool seguro)
{
	int entryIndex = getFileIndex(name);
	if(entryIndex==-1) return err("No existe " + name, 1);

    if(!canCurrentUserModifyFile(name))
        return err("User cannot modify " + name, 3);
    
	// Eliminar del FAT
	this->secureDeletion = seguro;
	removeFileRecursive(directorio[entryIndex].bloqueInicial);

	// Eliminar de directorio
	directorio.erase(directorio.begin()+ entryIndex);
    entradasTotales--;
	
	return 0;
};

/// ******************************************
/// DEBUG METHODS
/// ******************************************

void FileSystem::usarAsignacionAleatoria(bool usar)
{
	randomizedBlockSearching = usar;
};

void FileSystem::imprimirStrings()
{
	std::cout << RESET << BOLD << UNDERLINE;
	std::cout << "Strings:" << "\n";
	std::cout << RESET;
	for(int i = 0; i < directorio.size(); i++)
	{
		File entrada = directorio[i];
		std::cout << entrada.name << ": ";
		std::cout << getStringFrom(entrada.bloqueInicial);
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
};

void FileSystem::imprimirDirectorio()
{
	std::cout << RESET << BOLD;
	std::cout << "Files:" << "\n";
	std::cout << " Índice|    Nombre| Bloque Inicial| Contenido" << "\n";
	std::cout << RESET;
	for(int i = 0; i < directorio.size(); i++)
	{
		std::cout << std::setw(7) << i << "|";
		std::cout << std::setw(10) << directorio[i].name << "|";
		int ini = directorio[i].bloqueInicial;
		std::string inistr = std::to_string(ini);
		if(ini==-1) inistr += " (n/a)";
		std::cout << std::setw(15) << inistr << "|";
        std::cout << " ";
        printReplacing(getStringFrom(directorio[i].bloqueInicial), '\n', '#');
		std::cout << "\n";
	}
	std::cout << "\n";
};

void FileSystem::imprimirFAT(int cols)
{
	int espacio = 4;
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
	std::cout << RESET << BOLD;
	std::cout << "Memory Unit:";
    std::cout << RESET;
    printReplacing(" (\n = newline)", '\n', '#');
    std::cout << "\n";
	for(int i = 0; i < SIZE; i++) {
		std::cout << RESET << FAINT;
		std::cout << std::setw(3) << i;
		std::cout << RESET << BOLD;
		if (!unidadDeMemoria[i])
			std::cout << "   ";	
		else 
        {
            std::cout << "  ";
            printReplacing(chToStr(unidadDeMemoria[i]), '\n', '#');
        }
		std::cout << RESET << FAINT;
		std::cout << "|";
		
		if( (i+1)%cols == 0) std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << RESET;
};

void FileSystem::imprimirUnidad(int cols)
{
    imprimirFAT(cols);
	imprimirMemoria(cols);
    imprimirDirectorio();
	std::cout << "\n";
};

int FileSystem::loadTest()
{
    // en teoria se ejecuta luego de agregar el root
    createGroup("votingCenters");
    createUserBypass("vc1", "votingCenters", "");
    //createUserBypass("vc2", "votingCenters", "");
    createGroup("intermediaries");
    createUserBypass("int1", "intermediaries", "");
    //createUserBypass("int2", "intermediaries", "");
    createGroup("databases");
    createUserBypass("db1", "databases", "");
    //createUserBypass("db2", "databases", "");
    switchUser("db1");
    // load votes.txt 
    // load padrone.txt
    return 0;
};

int FileSystem::loadTestOld()
{
    // en teoria se ejecuta luego de agregar el root
    createGroup("students");
    createUserBypass("alice", "students", "");
    switchUser("alice");
    createFile("a.txt");
    appendToFile("a.txt", "ants!");
    createUserBypass("bob", "students", "");
    switchUser("bob");
    createFile("b.txt");
    appendToFile("b.txt", "birds!");
    createGroup("teachers");
    createUserBypass("carol", "teachers", "");
    switchUser("carol");
    createFile("c.txt");
    appendToFile("c.txt", "cats!");
    createFile("board.txt");
    appendToFile("board.txt", "dsjkfhakls");
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
    createUserBypass("dave", "guests", "");
    switchUser("dave");
    createFile("d.txt");
    appendToFile("d.txt", "dragons!");
    return 0;
};
