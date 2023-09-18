
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <cstring>

using namespace std;

// Global constant variables

const int LABEL_COUNT = 100; 	///< default number of labels
const int OPCODE_COUNT = 18; 	///< default number of opcodes
const int MAX_CHARS = 5;		///< maximum number of characters
const int STACK_SIZE = 1000;	///< maximum capacity of array

const string OPCODE_LIST = "const get put ld st add sub mul div cmp jpos jz j jl jle jg jge halt"; ///< list of opcodes needed for interpretation

bool a = 1; // for logs

struct Symbol {
	string name;
	int mem;

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
	string name;
	int code;

	Opcode() {
		name = "";
		code = 0;
	}
};

void splitOpcodes(string inst, Opcode*opcodes) {
	// cout << "entered splitOPcodes" << endl; // LOG OUTPUT

	for(int i = 0; i < OPCODE_COUNT ; i++) {
		static stringstream ss(inst);
		getline(ss, opcodes[i].name, ' ');
		opcodes[i].code = i;
		// cout << opcodes[i].name << " - " << opcodes[i].code << '\n';
	}
}

bool isOpcode(string &word, Opcode *opcodes) {
	// looping through the array of predefined opcodes 
	for(int i = 0 ; i < OPCODE_COUNT ; i++) {
		if(word == opcodes[i].name)
			return true; 
	}
	return false;
}

int getOpcode(string&word, Opcode *opcodes) {
	// looping through the array of predefined opcodes 
	for(int i = 0 ; i < OPCODE_COUNT ; i++) {
if(a) cout << "      entered getOpcode loop, comparing \"" << word << "\" to opcode: " << opcodes[i].name << endl;
		if(word == opcodes[i].name)
			return opcodes[i].code; 
	}
	return -1;
}

int getLocation(string& name, Symbol *labels, int nextLabel) {
	// loop breaks once the name matches the label, and so we 
	for(int i = 0 ; i < nextLabel ; i++) {
if(a) cout << "      entered getLocation loop, comparing \"" << name << "\" with struct value:" << labels[i].name << endl;
		if(name == labels[i].name) return labels[i].mem;
	}
	return -1;
}

bool isNumber(string &str) {
if(a) cout << "   entered isNumber" << endl;
	int test = 0;
	stringstream ss(str);
	if(ss >> test) {
		return true;
	}
	return false;
}

// Helper functions

/// Returns the number of spaced needed for padding.
/// @param str the string that needs padding.
/// @param len the maximum number of characters.
/// @return the number of spaces needed.
int padding(std::string str, int len) {
	int n = static_cast<int>(str.length());
	return len + abs(len - n);
}

/// Dumps the name and numerical value that each instrcution represents
/// @param op the array of opcodes.
/// @param count the number of opcodes.
/// @return void.
void dumpOpcodes(Opcode *opcodes, int count) {
	std::cout << "Opcodes" << std::endl;
	int pad = 0;
	for (int i = 0; i < count; ++i) {
		pad = padding(opcodes[i].name, MAX_CHARS);
		std::cout << "> " << opcodes[i].name << std::string(pad, ' ') << opcodes[i].code << std::endl;
	}
}

/// Dumps the name of a label and its corresponding location in memory.
/// @param sym an array of symbols.
/// @param count the number of symbols in the array.
/// @return void.
void dumpSymbols(Symbol* labels, int count) {
	std::cout << "Symbols" << std::endl;
	int pad = 0;
	for (int i = 0; i < count; ++i) {
		pad = padding(labels[i].name, MAX_CHARS);
		std::cout << "> " << "[" << labels[i].name << "] = " << labels[i].mem << std::endl; 
	}
}

/// Dumpds the address of each instruction and label.
/// @param memory the array of addresses.
/// @param count the number of addresses.
/// @return void.
void dumpMemory(int* memory, int count) {
	std::cout << "Memory" << std::endl;
	std::cout.fill('0');
	for (int i = 0; i < count; ++i) {
		std::cout << std::setw(8) << memory[i] << std::endl;
	}
}

/// Entry point of the main program.
/// @param argc the number of command-line arguments.
/// @param argv the vector of command-line arguments.
/// @return 0 if the program runs to complition.
int main(int argc, char *argv[]) {

	int memory[STACK_SIZE] = { }; // stores all of the instructions + locations
	Symbol symbols[STACK_SIZE]; // symbols read into array

	string tempFile = "firstpass";
	
	Opcode opcodes[OPCODE_COUNT]; // create the table of opcodes
	splitOpcodes(OPCODE_LIST, opcodes); // parse the list of opcodes into array

	// check command-line arguments
	if(argc == 1) {
		cout << "Error, incorrect usage \n./a.out <filename> " << '\n';
		return 1; // ends the progrma with error code 1
	}

	ifstream infile; // input file object
	infile.open(argv[1]); // opens the file specified in the commandline
	if(!infile.is_open()) {
		cout << "Error, could not open file" << endl;
		return 0;
	}

if(a) cout << "created input file object" << endl;

	ofstream outfile; // output file object
	outfile.open(tempFile);

if(a) cout << "created output file object" << endl;

// PASS #1 -----------------------------------------------------

	int symbolCount = 0; 

	while(!infile.eof()) {

if(a) cout << endl << "FIRSTPASS LOOP ";

		string tempLine; // creating a temp string variable to put line into ss 
		getline(infile, tempLine); // reading line into the temp string

if(a) cout << "line is: \"" << tempLine << "\"" << endl;

		stringstream ss(tempLine); // putting string into stringstream object
		// checking for a comment (;) or a blank linie
		if(ss.peek() == ';' || tempLine == "") {

if(a) cout << "   detected thing, going to NEXT LINE" << endl;
			continue; // looping to next line if semicolon
		} 
		
		string word = ""; // for reading each word

		bool firstWord = true; // used to check if ss is on its first word, too tired to find a better way
		while(ss >> word) {

if(a) cout << "   ENTERED CHILD LOOP, firstWord is " << boolalpha << firstWord << endl;
if(a) cout << "   checking: " << word << endl;

			if (isOpcode(word, opcodes)) {

// cout << "   detected " << word << " as an opcode" << endl;

				outfile << word << ' '; // outputting into firstpass file
			} else if(firstWord) {
				
if(a) cout << "   storing symbol \"" << word << "\" inside symbol struct at array " << symbolCount << endl;

			symbols[symbolCount].store(word, symbolCount); 

			} else if(word[0] == ';') {

if(a) cout << "   detected semicolon, BREAKING from child loop" << endl;
				break;
			} else if(!firstWord) {

				outfile << word << ' ';

			} 
			
			firstWord = false;
		}

if(a) cout << "symbol array index: " << symbolCount << endl;
		outfile << '\n';
		symbolCount++;
	}

// pASS 2------------------------------ (dying very much)
if(a) cout << "\nclosing and reopening infile with the temp file\n" << endl;
	outfile.close();
	infile.close();
	infile.open(tempFile);

	int memoryCount = 0;

	while(!infile.eof()) {
if(a) cout << "SECONDPASS LOOP, memory count: " << memoryCount << endl;
		string tempLine = "";
		getline(infile, tempLine);
		stringstream ss(tempLine);
if(a) cout << "   line \"" << tempLine << "\"" << endl;

		string word = "";
		int memoryValue = 0; // integer to store the memory value

		while(ss >> word) {
			memory[memoryCount] = 0; // initializing value
if(a) cout << "   entering line loop, word: " << word << endl;
			if(isOpcode(word, opcodes)) {
if(a) cout << "   entered if isOpcode: " << endl;
				memoryValue += (getOpcode(word, opcodes) * (STACK_SIZE));
if(a) cout << "memory value: " << memoryValue << endl << endl;
			} else if(isNumber(word)) {
				memoryValue += stoi(word);
			} else {
if(a) cout << "   word was not opcode, storing in memory as symbol: " << endl;
				memoryValue += getLocation(word, symbols, STACK_SIZE);
if(a) cout << "memory value: " << memoryValue << endl << endl;
			}
		}
		memory[memoryCount] = memoryValue;
		memoryCount++;
	}
	
if(a) {
	dumpOpcodes(opcodes, OPCODE_COUNT);
	dumpSymbols(symbols, symbolCount);
	dumpMemory(memory, memoryCount);
}

	cout << "Running program..." << endl;
	int reg = 0; // register variable
	int ip = 0; //instruction pointer
	int addr = 0;

	while(ip > -1) {
		addr = memory[ip] % 1000;
		
if(a) cout << memory[ip] << ": " << opcodes[memory[ip] / 1000].name << ".  reg: " << reg << ".  ip: " << ip << ".  addr: " << addr << endl;
		switch(memory[ip]/1000) {
			case 0:  // const    - defines constant or memory location
				break;
			case 1:  // get      - reads numbers from input
				cin >> reg;
				if(cin.fail()) {
					cout << "ERROR: invalid input!" << endl;
					return 1;
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
if(a) cout << "memory is: " << memory[addr] << endl;
				ip++;
				break;
			case 4:  // st (x)   - stores (x) with contents from reg
				memory[addr] = reg;
				ip++;
				break;
			case 5:  // add (x)  - adds (x) to reg
				reg += memory[addr];
				ip++;
if(a) cout << "memory is: " << memory[addr] << endl;
				break;
			case 6:  // sub (x)  - subtracts (x) from reg
			case 9:  // cmp (x)  - compares (x) and reg
				reg -= memory[addr];
				ip++;
if(a) cout << "memory is: " << memory[addr] << endl;
				break;
			case 7:  // mul (x)  - multiplies (x) and reg
				reg *= memory[addr];
				ip++;
if(a) cout << "memory is: " << memory[addr] << endl;
				break;
			case 8:  // div (x)  - divides (x) by reg
				reg /= memory[addr];
				ip++;
if(a) cout << "memory is: " << memory[addr] << endl;
				break;
			case 10: // jpos (x) - jumps to (x) if reg is positive
				if(reg > 0) ip = addr;
				break;
			case 11: // jz (x)   - jumps to (x) if reg = zero
				if(reg == 0) ip = addr;
				break;
			case 12: // j (x)    - jumps to (x)
				ip = addr;
				ip++;
				break;
			case 13: // jl (x)   - jumps to (x) if reg < 0
				if(reg < 0) ip = addr;
				break;
			case 14: // jle (x)  - jumps to (x) if reg <= 0
				if(reg <= 0) ip = addr;
				break;
			case 15: // jg (x)   - jumps to (x) if reg > 0
				if(reg > 0) ip = addr;
				break;
			case 16: // jge (x)  - jumps to (x) if reg >= 0
				if(reg >= 0) ip = addr;
				break;
			case 17: // halt     - stops execution
				cout << "** Program terminated **" << endl << endl;
				ip = -1;
				break;
			default:
				ip = -1;
		}
	}
	
	return EXIT_SUCCESS;
}