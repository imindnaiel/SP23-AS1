/*
name: daniel nguyen, 5007379554, cs 202-1007, assignment 1
description: this program is a virtual machine that simulates 
			 assembly for a hypothetical processor. the assembly 
			 of the code happens in 2 passes, with the first one 
			 removing comments, remembering labels, and saving
			 everything else onto a temporary output file. the 
			 second pass uses the output file to pack the code 
			 into a memory array, which is then read by the
			 interpreter which executes all the commands.
input: the user inputs the filename, and any of the inputs 
	   neccessary for the chosen program
output: the program outputs all of the data from the get and put 
		commands, as well as any errors that occur
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <cstring>

using namespace std;

// Global constant variables

const int LABEL_COUNT = 100; //< default number of labels
const int OPCODE_COUNT = 18; //< default number of opcodes
const int MAX_CHARS = 5;	 //< maximum number of characters
const int STACK_SIZE = 1000; //< maximum capacity of array

///< list of opcodes needed for interpretation
const string OPCODE_LIST =
"const get put ld st add sub mul div cmp jpos jz j jl jle jg jge halt"; 


struct Symbol {
	string name; // stores symbol name
	int mem;     // stores memory location

	// used to store symbol attributes 
	void store(string name, int mem) {
		this->name = name;
		this->mem = mem;
	}

	Symbol() {
		name = "";
		mem = -1;
	}
	
};

struct Opcode {
	string name; // stores the instruction
	int code;    // stores the numeric representation

	Opcode() {
		name = "";
		code = 0;
	}
};

// used for breaking the global opcode list down and putting 
// the words into an array of opcodes
void splitOpcodes(string inst, Opcode*opcodes) {
	for(int i = 0; i < OPCODE_COUNT ; i++) {
		static stringstream ss(inst); // sstream object for opcodes
		getline(ss, opcodes[i].name, ' '); 
		opcodes[i].code = i;
	}
}

// returns true if the passed string is 
// an opcode otherwise it returns false
bool isOpcode(string &word, Opcode *opcodes) {
	for(int i = 0 ; i < OPCODE_COUNT ; i++) {
		if(word == opcodes[i].name)
			return true; 
	}
	return false;
}

// gets the numerical opcode value from a string
int getOpcode(string&word, Opcode *opcodes) {
	// looping through the array of predefined opcodes 
	for(int i = 0 ; i < OPCODE_COUNT ; i++) {
		if(word == opcodes[i].name)
			return opcodes[i].code; 
	}
	return -1;
}

// gets the location of the label passed
int getLocation(string& name, Symbol *labels, int nextLabel) {
	// loop breaks once the name matches the label, and so we 
	for(int i = 0 ; i < nextLabel ; i++) {
		if(name == labels[i].name) 
			return labels[i].mem;
	}
	return -1;
}

// validates the string when it is an integer value
bool isNumber(string &str) {
	int test = 0;
	stringstream ss(str);
	if(ss >> test) {
		return true;
	}
	return false;
}

// heart of the interpreter; it executes the commands 
// through the passed instruction pointer
void runCommand(int memory[], int &ip, int &reg, int addr) {
	switch(memory[ip] / 1000) {
		case 0:  // const    - defines constant or memory location
			break;
		case 1:  // get      - reads numbers from input
			cin >> reg;
			if(cin.fail()) {
				cout << "break\nERROR: invalid input!\n"
				<< "** Program terminated with an error code** ";
				exit(0);
			}
			cout << "read: " << reg << endl;
			ip++;
			break;
		case 2:  // put      - writes contents to output
			cout << "result: " << reg << endl;
			ip++;
			break;
		case 3:  // ld (x)   - loads reg with contents from (x)
			reg = memory[addr];
			ip++;
			break;
		case 4:  // st (x)   - stores (x) with contents from reg
			memory[addr] = reg;
			ip++;
			break;
		case 5:  // add (x)  - adds (x) to reg
			reg += memory[addr];
			ip++;
			break;
		case 6:  // sub (x)  - subtracts (x) from reg
		case 9:  // cmp (x)  - compares (x) and reg
			reg -= memory[addr];
			ip++;
			break;
		case 7:  // mul (x)  - multiplies (x) and reg
			reg *= memory[addr];
			ip++;
			break;
		case 8:  // div (x)  - divides (x) by reg
			reg /= memory[addr];
			ip++;
			break;
		case 10: // jpos (x) - jumps to (x) if reg is positive
			(reg > 0) ? ip = addr : ip++;
			break;
		case 11: // jz (x)   - jumps to (x) if reg = zero
			(reg == 0) ? ip = addr : ip++;
			break;
		case 12: // j (x)    - jumps to (x)
			ip = addr;
			break;
		case 13: // jl (x)   - jumps to (x) if reg < 0
			(reg < 0) ? ip = addr : ip++;
			break;
		case 14: // jle (x)  - jumps to (x) if reg <= 0
			(reg <= 0) ? ip = addr : ip++;
			break;
		case 15: // jg (x)   - jumps to (x) if reg > 0
			(reg > 0) ? ip = addr : ip++;
			break;
		case 16: // jge (x)  - jumps to (x) if reg >= 0
			(reg >= 0) ? ip = addr : ip++;
			break;
		case 17: // halt     - stops execution
			cout << "** Program terminated **" << endl << endl;
			ip = -1;
			break;
		default:
			ip = -1;
	}
}

/// Entry point of the main program.
/// @param argc the number of command-line arguments.
/// @param argv the vector of command-line arguments.
/// @return 0 if the program runs to complition.
int main(int argc, char *argv[]) {
	Symbol symbols[STACK_SIZE]; // symbols read into array

	string tempFile = "firstpass"; // stores the 
	
	Opcode opcodes[OPCODE_COUNT]; // create the table of opcodes
	splitOpcodes(OPCODE_LIST, opcodes); // parse opcodes into array

	// check command-line arguments
	if(argc != 2) {
		cout << "error, incorrect usage \n./a.out <filename> " << '\n';
		return 1; // ends the progrma with error code 1
	}

	ifstream infile; // input file object
	infile.open(argv[1]); // opens the file specified in the commandline
	if(!infile.is_open()) {
		cout << "break\nerror: invalid file!\n"
		<< "** program terminated with an error code** ";
		exit(0);
	}

	ofstream outfile; // output file object
	outfile.open(tempFile);

// PASS #1 -----------------------------------------------------

	int symbolCount = 0; 

	// loop reads the given assembly file to help generate the memory
	while(!infile.eof()) {

		// creating a temp string variable to put line into ss 
		string tempLine; 
		getline(infile, tempLine); // reading line into the temp string


		stringstream ss(tempLine); // putting string into sstream object
		// checking for a comment (;) or a blank linie
		if(ss.peek() == ';' || tempLine == "") {

			continue; // looping to next line if semicolon
		} 
		
		string word = ""; // for reading each word

		// used to check if ss is on its first 
		//word, too tired to find a better way atm
		bool firstWord = true; 

		// while loop reads in the current word to 
		// check if it is a specific 'type' of word
		while(ss >> word) {
			if (isOpcode(word, opcodes)) {
				outfile << word << ' '; // outputting into firstpass file
			} else if(firstWord) {
				symbols[symbolCount].store(word, symbolCount); 
			symbols[symbolCount].store(word, symbolCount); 
				symbols[symbolCount].store(word, symbolCount); 
			} else if(word[0] == ';') {
				break;
			} else if(!firstWord) {
				outfile << word << ' ';
			} 
			
			firstWord = false;
		}

		outfile << '\n';
		symbolCount++;
	}

// pASS 2------------------------------ (dying very much)
	outfile.close();
	infile.close();
	infile.open(tempFile);

	int memory[STACK_SIZE] = { }; // stores all instructions + locations
	int memorySize = 0;

	while(!infile.eof()) {
		string tempLine = "";      // creating
		getline(infile, tempLine); // stringstream
		stringstream ss(tempLine); // object

		string word = "";
		int memoryValue = 0; // integer to store the memory value

		// loop to read the entire line
		while(ss >> word) {
			memory[memorySize] = 0; // initializing value

			if(isOpcode(word, opcodes)) {
				memoryValue += (getOpcode(word, opcodes) * (STACK_SIZE));
			} else if(isNumber(word)) {
				memoryValue += stoi(word);
			} else {
				memoryValue += getLocation(word, symbols, STACK_SIZE);
			}
		}
		memory[memorySize] = memoryValue;
		memorySize++;
	}
	
	// THE INTERPRETER BEGINS
	cout << "Running program..." << endl;
// dumpMemory(memory, memorySize);

	int reg = 0; // register variable
	int ip = 0; //instruction pointer

	while(ip > -1) {
		int addr = memory[ip] % STACK_SIZE;
		//cout << memory[ip] << ": " 
		// << opcodes[memory[ip] / STACK_SIZE].name 
		// << ".  reg: " << reg << ".  ip: " << ip 
		// << ".  addr: " << addr << endl;
		// ^ cout used to check all the values to help debug
		runCommand(memory, ip, reg, addr);
	}
	
	return EXIT_SUCCESS;
}