#include "main.h"

// TODO: make a real assembler, not a ton of if statements in a while loop

using namespace std;

struct jump {
	string jmpNames[65536];
	string jmpValues[65536];
	int jmpSize;
};

struct config {
	string inputFilename;
	string outputFilename;
};

class Assembler {
	public:
		bool exiting;
		int line;
		int cycle;
		jump jmp;
		config MainConfig;
};
Assembler assembler;

void Err(string errMsg){
	cout << "Error! " << errMsg << endl;
	remove(assembler.MainConfig.outputFilename.c_str());
	assembler.exiting = true;
}

string getRegister(string inp){
	if(inp.find("&a") < 6) return "1";
	if(inp.find("&b") < 6) return "2";
	if(inp.find("&c") < 6) return "3";
	if(inp.find("&d") < 6) return "4";
	Err("Could not find needed Register in line " + to_string(assembler.line) + ": " + inp);
	return "";
}

string getSRegister(string inp){
	if(inp.find("&a") != string::npos) inp = inp.substr(inp.find("&a"), inp.size());
	if(inp.find("&b") != string::npos) inp = inp.substr(inp.find("&b"), inp.size());
	if(inp.find("&c") != string::npos) inp = inp.substr(inp.find("&c"), inp.size());
	if(inp.find("&d") != string::npos) inp = inp.substr(inp.find("&d"), inp.size());
	if(inp.find("&a") != string::npos) return "1";
	if(inp.find("&b") != string::npos) return "2";
	if(inp.find("&c") != string::npos) return "3";
	if(inp.find("&d") != string::npos) return "4";
	Err("Could not find needed Register in line " + to_string(assembler.line) + ": " + inp);
	return "";
}

string getHexValue(string inp){
	if(inp.find("0x")) return inp.substr(inp.find("0x")+2, 4);
	Err("Could not find needed Hex Value in line " + to_string(assembler.line) + ": " + inp);
	return "";
}

string get8HexValue(string inp){
	if(inp.find("0x")) return inp.substr(inp.find("0x")+2, 2);
	Err("Could not find needed Hex Value in line " + to_string(assembler.line) + ": " + inp);
	return "";
}

string hexToStr(int hxValue){
	ostringstream tmp;
	tmp << hex << hxValue;
	string hexStr = tmp.str();
	if(hexStr.length() <= 1) return "000" + hexStr;
	if(hexStr.length() <= 2) return "00" + hexStr;
	if(hexStr.length() <= 3) return "0" + hexStr;
	return hexStr;
}

string jmp(string inp){
	for(int i=0; i<assembler.jmp.jmpSize; i++){
		if(inp.find(assembler.jmp.jmpNames[i]) != string::npos){
			return assembler.jmp.jmpValues[i];
			break;
		}
	}
	Err("Could not find jump label in line " + to_string(assembler.line) + ": " + inp);
	return "";
}

string assemble(string inp){
	if(assembler.exiting) return "";
	if(inp.find(";") == 0) return "";
	if(inp.find(";") > 0) inp = inp.substr(0, inp.find(";"));
	if(inp == "" || inp == "\n" || inp == "\t" || inp == "\r") return "";

	if(inp.find(":") != string::npos){
		assembler.jmp.jmpNames[assembler.jmp.jmpSize] = inp.substr(0, inp.find(":"));
		assembler.jmp.jmpValues[assembler.jmp.jmpSize] = hexToStr(assembler.cycle);
		assembler.jmp.jmpSize++;
		return "";
	}

	if(inp.find("nop") != string::npos) return "00000000 ";
	if(inp.find("hlt") != string::npos) return "01000000 ";
	if(inp.find("add") != string::npos) return "02" + getRegister(inp) + getHexValue(inp) + "0 ";
	if(inp.find("adr") != string::npos) return "03" + getRegister(inp) + getSRegister(inp) + "0000 ";
	if(inp.find("dec") != string::npos) return "04" + getRegister(inp) + getHexValue(inp) + "0 ";
	if(inp.find("der") != string::npos) return "05" + getRegister(inp) + getSRegister(inp) + "0000 ";
	if(inp.find("set") != string::npos) return "06" + getRegister(inp) + getHexValue(inp) + "0 ";
	if(inp.find("jmp") != string::npos) return "07" + jmp(inp) + "00 ";
	if(inp.find("jmr") != string::npos) return "08" + getRegister(inp) + "00000 ";
	if(inp.find("jmz") != string::npos) return "09" + jmp(inp) + "00 ";
	if(inp.find("jmo") != string::npos) return "0a" + jmp(inp) + "00 ";
	if(inp.find("cmp") != string::npos) return "0b" + getRegister(inp) + getHexValue(inp) + "0 ";
	if(inp.find("cmr") != string::npos) return "0c" + getRegister(inp) + getSRegister(inp) + "0000 ";
	if(inp.find("je ") != string::npos) return "0d" + jmp(inp) + "00 ";
	if(inp.find("inb") != string::npos) return "0e" + getRegister(inp) + "00000 ";
	if(inp.find("oub") != string::npos) return "0f" + getHexValue(inp) + "00 ";
	if(inp.find("our") != string::npos) return "10" + getRegister(inp) + "00000 ";
	if(inp.find("sin") != string::npos) return "11" + getRegister(inp) + "00000 ";
	if(inp.find("sou") != string::npos) return "12" + getHexValue(inp) + "00 ";
	if(inp.find("sor") != string::npos) return "13" + getRegister(inp) + "00000 ";
	if(inp.find("mem") != string::npos) return "14" + getRegister(inp) + getHexValue(inp) + "0 ";
	if(inp.find("mer") != string::npos) return "15" + getRegister(inp) + getSRegister(inp) + "0000 ";
	if(inp.find("lem") != string::npos) return "16" + getRegister(inp) + getHexValue(inp) + "0 ";
	if(inp.find("ler") != string::npos) return "17" + getRegister(inp) + getSRegister(inp) + "0000 ";
	if(inp.find("vrm") != string::npos) return "18" + getRegister(inp) + getHexValue(inp) + "0 ";
	if(inp.find("vrr") != string::npos) return "19" + getRegister(inp) + getSRegister(inp) + "0000 ";
	if(inp.find("ret") != string::npos) return "1a000000 ";
	if(inp.find("xra") != string::npos) return "1b" + getHexValue(inp) + "00 ";
	if(inp.find("xro") != string::npos) return "1c" + getHexValue(inp) + "00 ";

	Err("Could not assemble line " + to_string(assembler.line) + ": " + inp);
	return "";
}

void assembleFile(string filename, string outFilename){
	ifstream inputFile(filename);
	ofstream outputFile(outFilename);
	outputFile << "v2.0 raw" << endl;
	while(inputFile.is_open() && inputFile.good()){
		string inp;
		getline(inputFile, inp);
		if(inp.find(":") != string::npos){
			assembler.jmp.jmpNames[assembler.jmp.jmpSize] = inp.substr(0, inp.find(":"));
			assembler.jmp.jmpValues[assembler.jmp.jmpSize] = hexToStr(assembler.cycle);
			assembler.jmp.jmpSize++;
		}
		assembler.cycle += 0x0001;
	}
	inputFile.close();
	assembler.cycle = 0;
	inputFile.open(filename);
	while(inputFile.is_open() && inputFile.good() && !assembler.exiting){
		if(assembler.exiting){ break; }
		assembler.line++;
		string inp;
		getline(inputFile, inp);
		if(outputFile.is_open()) outputFile << assemble(inp);
		assembler.cycle += 0x0001;
	}
}

void help(){
    cout << "MALA Help" << endl << "---------" << endl;
	cout << "MALA version 1.1" << endl << endl;
	cout << "mala -i <filename> -o <filename>" << endl << endl;
	cout << "-i		specify input file" << endl;
	cout << "-o		specifiy output file" << endl;
}

int main(int argc, char* argv[]){
	if(argc <= 1) Err("to few arguments!\nUse -help for help.");
  	for (size_t i = 1; i < argc; ++i){
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) help();
		if (strcmp(argv[i], "-o") == 0) assembler.MainConfig.outputFilename = argv[i+1];
    		if (strcmp(argv[i], "-i")) assembler.MainConfig.inputFilename = argv[i+1];
  	}
	assembleFile(assembler.MainConfig.inputFilename, assembler.MainConfig.outputFilename);
	if(assembler.exiting) return 1;
	return 0;
}
