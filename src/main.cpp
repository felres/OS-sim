/* *****************************************************************************
 * 
 * File System Mockup
 * 
 * This program was made for CI-0122, by:
 * Javier Donato           B92650
 * Ronald León Morales     B33733
 * Tonny Ortiz Salazar     B35054
 * Felipe Vega Vindas      B78244
 * Andrés Zúñiga           B68014
 * 
 * Documentation guide: https://developer.lsst.io/cpp/api-docs.html
 * 
 * To Do:
 * - Fix date
 * - Document code (IN ENGLISH)
 * - terminar transición de arrays a std::vectors
 * - distinguish between index and id
 */

#include <iostream>
#include "FileSystem.hpp"
#include "Helpers.cpp"
#include "Interface.hpp"
#include "Padrone.hpp"
#include "VotesReg.hpp"
#include "Communicator.hpp"



/**
 * Creates and executes user interface.
 */
int main() {
    randomize();
    
    FileSystem *fs = new FileSystem();
    Padrone *padroneApp = new Padrone(fs);
    VotesReg *voteApp = new VotesReg(fs);
    Communicator* comm = new Communicator(fs);
	Interface *console = new Interface(fs,
                                        padroneApp,
                                        voteApp,
                                        comm);
	console->run();
}
