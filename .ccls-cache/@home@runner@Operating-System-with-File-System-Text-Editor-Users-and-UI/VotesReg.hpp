#include "VotesReg.hpp"
#include "FileSystem.hpp"
class VotesReg {
private:
	FileSystem* fs;
public:
	VotesReg(FileSystem* filesys){fs=filesys;};
	~VotesReg();
};