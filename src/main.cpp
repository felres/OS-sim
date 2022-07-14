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
 * - Add Protocols to respective process() https://docs.google.com/spreadsheets/d/1dI58FnXOyTGvsmeoWwhXEr6RcQyrihM0KbHcKZc9K40/edit?usp=sharing
 */

// ttps://docs.google.com/spreadsheets/d/1dI58FnXOyTGvsmeoWwhXEr6RcQyrihM0KbHcKZc9K40/edit?usp=sharing

//

//

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
	Interface *console = new Interface(fs,
                                        padroneApp,
                                        voteApp);
	console->run();
}
