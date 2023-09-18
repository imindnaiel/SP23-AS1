
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Global constant variables

const int LABEL_COUNT = 100; 	///< default number of labels
const int OPCODE_COUNT = 18; 	///< default number of opcodes
const int MAX_CHARS = 5;		///< maximum number of characters
const int STACK_SIZE = 1000;	///< maximum capacity of array

const std::string OPCODE_LIST = "const get put ld st add sub mul div cmp jpos jz j jl jle jg jge halt"; ///< list of opcodes needed for interpretation

// <--
// YOUR CODE GOES HERE

// -->

// Helper functions

/// Returns the number of spaced needed for padding.
/// @param str the string that needs padding.
/// @param len the maximum number of characters.
/// @return the number of spaces needed.


/// Entry point of the main program.
/// @param argc the number of command-line arguments.
/// @param argv the vector of command-line arguments.
/// @return 0 if the program runs to complition.
int main(int argc, char *argv[]) {
	// <---
	// YOUR CODE GOES HERE
	
	// create the table of opcodes
	// parse the list of opcodes
	
	// check command-line arguments
		//
		//--------------------------------------------
		// PASS #1
		//
		// (1) Removing comments
		// (2) Remember labels
		// (3) Save instruction/opcode and address
		// (4) Write generated code to an output file
		//
	
		//
		// --------------------------------------------
		//
		// PASS #2
		//
		// (1) Read output file
		// (2) Replace numeric values and pack them into
		//
	
		//
		// --------------------------------------------
		//
		// INTEPRETER
		//
		// (1) Execute instructions in sequential order
		//
	std::cout << "Running program..." << std::endl;
	int reg = 0; // register variable
	// -->
	return EXIT_SUCCESS;
}