#pragma once

#include <string>
#include "FileSystem.hpp"
#include "Padrone.hpp"
#include "VotesReg.hpp"

class Interface{
private:
	std::string input;
    Padrone *padroneApp;
    VotesReg *voteApp;
public:
	Interface(Padrone* padroneApp, VotesReg* voteApp)
    {
        this->padroneApp = padroneApp;
        this->voteApp = voteApp;
    }
	~Interface(){}

    /**
     * Read user input from cout. 
     * 
     * @param FileSystem object to send user requests to.
     * @return 0 if ok.
     */	
    int run(FileSystem* fs){
		while(true)
	    {
	        std::cout << RESET << BOLD << GREEN;
	        std::cout << fs->getName() << ":~$ ";
	        std::cout << RESET;
	        std::getline(std::cin, input);
	        if(process(input, fs)==1)
	            break;
	    }
		return 0;
	}
	
	
	/**
     * Analizes line input from user,
     * sends relevant workload to FileSystem.
     * 
     * @param line string to analise.
     * @param *fs file system to use.
     * @return 0 if can recieve more input. 1 if user requested exit.
     */
	int process(std::string line, FileSystem *fs)
	{
	    std::cout << RESET;
	    
        std::vector<std::string> argv = tokenize(line, " ");
	    int argc = argv.size();
	    
	    if(argc > 0)
	    {
	        if(argv[0] == "marco")
	        {
	            std::cout << "polo\n";
	        }
	        else if(argv[0] == "hola")
	        {
	            std::cout << "¡Hola!\n";
	        }
	        else if( (argv[0] == "help")
	                || (argv[0] == "-help")
	                || (argv[0] == "-h")
	                || (argv[0] == "-H")
	                || (argv[0] == "h")
	                || (argv[0] == "H"))
	        {
	            std::cout << RESET << BOLD;
		        std::cout << "     Command| Description" << "\n";
	            std::cout << RESET;
	            printWithUnderlineAt("    filelist| list files\n",4,8);
	            printWithUnderlineAt("    userlist| list users\n",4,8);
	            printWithUnderlineAt("   grouplist| list groups\n",3,8);
	            printWithUnderlineAt("     fileadd| create file\n",5,9);
	            printWithUnderlineAt("     useradd| create user\n",5,9);
	            printWithUnderlineAt("    groupadd| create group\n",4,9);
	            printWithUnderlineAt("  switchuser| switch user\n",2,8);
	            printWithUnderlineAt("        read| read file\n",8);
	            printWithUnderlineAt("       write| write in file\n",7);
	            printWithUnderlineAt("      append| append to file\n",6);
	            printWithUnderlineAt("      remove| remove file\n",6,8);
	            printWithUnderlineAt("  changemode| change file modes\n",2,3,8,9,10);
                printWithUnderlineAt("loadrealfile| load a real file\n",0,4,8);
                printWithUnderlineAt("  padroneapp| open a file as a padrone\n",2,9);
                printWithUnderlineAt("     voteapp| open a file as a vote register\n",5,9);
                printWithUnderlineAt("    password| set or change password\n",4,8);
	            printWithUnderlineAt("        exit| close the program\n");
	            std::cout << RESET << BOLD;
		        std::cout << "      (DEBUG ONLY)" << "\n";
	            std::cout << RESET;
	            std::cout << "       print| print full memory unit" << "\n";
	            std::cout << "        test| add template users, groups and files"
	                            << "\n";
	            std::cout << "\n";
	        }
	        else if((argv[0] == "filelist")||(argv[0] == "fl")||(argv[0] == "ls"))
	        {
	            fs->printFiles();
	        }
	        else if((argv[0] == "userlist")||(argv[0] == "ul"))
	        {
	            fs->printUsers();
	        }
	        else if((argv[0] == "grouplist")||(argv[0] == "gl"))
	        {
	            fs->printGroups();
	        }
	        else if( (argv[0] == "fileadd")
	                || (argv[0] == "touch")
	                ||(argv[0] == "fa") )
	        {
	            if(argc > 1)
	            {
	                if(fs->createFile(argv[1])==0)
	                    std::cout << "Created file " + argv[1] + ".\n";
	            }
	            else
	                std::cout << "Use: fileadd [file name]\n";
	        }
	        else if((argv[0] == "useradd")||(argv[0] == "ua"))
	        {
	            if(argc > 2)
	            {
	                if(fs->createUser(argv[1], argv[2])==0)
	                    std::cout << "Added user.\n";
	            }
	            else
	                std::cout << "Use: useradd [user name] [primary group name]\n";
	        }
	        else if((argv[0] == "groupadd")||(argv[0] == "ga"))
	        {
	            if(argc > 1)
	            {
	                if(fs->createGroup(argv[1])==0)
	                    std::cout << "Created group " + argv[1] + ".\n";
	            }
	            else
	                std::cout << "Use: groupadd [group name]\n";
	        }
	        else if((argv[0] == "switchuser")||(argv[0] == "su"))
	        {
	            if(argc > 1)
	            {
	                if(fs->switchUser(argv[1])==0)
	                    std::cout << "Switched to user " + argv[1] + ".\n";
	            }
	            else
	                std::cout << "Use: switchuser [user name]\n";
	        }
	        else if((argv[0] == "append")||(argv[0] == "a"))
	        {
	            if(argc > 2)
	            {
	                if(fs->appendToFile(argv[1], argv[2])==0)
	                    std::cout << "Appended " + argv[2] +
	                                " to " + argv[1] + ".\n";
	            }
	            else
	                std::cout << "Use: append [file name] [string]\n";
	        }
	        else if((argv[0] == "remove")||(argv[0] == "rm"))
	        {
	            if(argc > 1)
	            {
	                if(fs->removeFile(argv[1], false)==0)
	                    std::cout << "Removed file " + argv[1] + ".\n";
	            }
	            else
	                std::cout << "Use: remove [file name]\n";
	        }
	        else if((argv[0] == "read")||(argv[0] == "cat")||(argv[0] == "r"))
	        {
	            if(argc > 1)
	            {
	                fs->readFile(argv[1]);
	            }
	            else
	                std::cout << "Use: read [file name]\n";
	        }
	        else if((argv[0] == "write")||(argv[0] == "w"))
	        {
	            if(argc > 3)
	            {
	                if(fs->writeInFileAt(argv[1], stoi(argv[2]), argv[3][0])==0)
	                    std::cout << "Wrote " + chToStr(argv[3][0]) +
	                                " to " + argv[1] + 
	                                " in " + argv[2] + ".\n";
	            }
	            else
	                std::cout << "Use: write [file name] [index] [char]\n";
	        }
	        else if((argv[0] == "changemode")||(argv[0] == "chmod"))
	        {
	            if(argc > 3)
	            {
	                if(fs->changemode(argv[1], stoi(argv[2]),
	                                    strToBool(argv[3]))==0)
	                    std::cout << "Changed " + chToStr(argv[1][0]) +
	                                " flag " + argv[2] + 
	                                " to " + argv[3] + ".\n";
	            }
	            else
	                std::cout << "Use: changemode [file name] [flag(0-8)] [t/f]\n";
	        }
            else if((argv[0] == "loadrealfile")||(argv[0] == "lrf"))
	        {
	            if(argc > 1)
	            {
	                fs->loadRealFile(argv[1]);
	            }
	            else
	                std::cout << "Use: loadrealfile [REAL file name]\n";
	        }
            else if((argv[0] == "padroneapp")||(argv[0] == "pa"))
	        {
                if(argc > 1)
	            {
                    if( fs->canCurrentUserExecuteFile(argv[1]) )
                        padroneApp->run(argv[1]);
                    else
                        std::cout << "No permission to execute " << argv[1] << ".\n";
                }
                else
	                std::cout << "Use: padroneapp [file name]\n";
	        }
            else if((argv[0] == "voteapp")||(argv[0] == "va"))
	        {
	            if(argc > 1)
	            {
                    if( fs->canCurrentUserExecuteFile(argv[1]) )
                        voteApp->run(argv[1]);
                    else
                        std::cout << "No permission to execute "
                                << argv[1] << ".\n";
	            }
	            else
	                std::cout << "Use: voteapp [file name]\n";
	        }
            else if((argv[0] == "password")||(argv[0] == "passwd")||(argv[0] == "pw"))
	        {
	            fs->changePassword();
	        }
	        else if((argv[0] == "close")||(argv[0] == "exit"))
	        {
	            std::cout << "¡Hasta luego!" << "\n";
	            return 1;
	        }
	        /* DEBUG COMMANDS */
	        else if(argv[0] == "print")
	        {
	            fs->imprimirUnidad(10);
	        }
	        else if(argv[0] == "test")
	        {
	            fs->loadTest();
	        }
	        // Final conditional
	        else if(argv[0] != "")
	        {
	            std::cout << "Unknown command: " << argv[0] <<
	                " (type \"help\" for commands).\n";
	        }
	    }
	    
	    return 0;
	}
};