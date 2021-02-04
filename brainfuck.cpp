#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct ParseTreeElement{
	int type;// 1 content, 2 pionter, 3 loop
	ParseTreeElement *child = nullptr;
	ParseTreeElement *parent = nullptr;
	ParseTreeElement *nextElement = nullptr;
	int valInterpretation; // 1 relative, 2 absolute
	int value;
};

struct ParseTree{
	ParseTreeElement firstElement;
};

std::string output;
int amountOfTabs = 1;
int loopDepth = 0;
int addressIsRelative = 0;


void printNTabs(int n){
	for(int i = 0; i < n; i++){
		output.append("\t");
	}
}

void printValue(int value){
	std::stringstream ss;
	if(value > 1){
		ss << "memory[ptr]+=";
		ss << value;
		ss << ";\n";
	}else if(value == 1){
		ss << "memory[ptr]++";
		ss << ";\n";
	}else if(value < -1){
		ss << "*memory[ptr]-=";
		ss << -value;
		ss << ";\n";
	}else if(value == -1){
		ss << "memory[ptr]--";
		ss << ";\n";
	}
	printNTabs(amountOfTabs);
	output.append(ss.str());
}

void printAdress(int value){
	std::stringstream ss;
	if(value > 1){
		ss << "ptr+=";
		ss << value;
		ss << ";\n";
	}else if(value == 1){
		ss << "ptr++";
		ss << ";\n";
	}else if(value < -1){
		ss << "ptr-=";
		ss << -value;
		ss << ";\n";
	}else if(value == -1){
		ss << "ptr--";
		ss << ";\n";
	}
	printNTabs(amountOfTabs);
	output.append(ss.str());
	
}



int main(int argc, const char **argv){
	output = "#include <iostream>\n#include<stdio.h>\n#include<stdint.h>\nint main(){uint16_t memory[120000] = {0}; uint16_t ptr = 0;uint16_t a;\n";
	std::ifstream inputFile(std::string(argv[1]) ,std::ifstream::binary);
	if(inputFile){
	    inputFile.seekg (0, inputFile.end);
	    int length = inputFile.tellg();
	    inputFile.seekg (0, inputFile.beg);

		std::vector<char> fileBuffer;
		fileBuffer.resize(length);
		inputFile.read (fileBuffer.data(),length);

		int addressDelta = 0;
		int useAddressDelta = 0;

		int useValueDelta = 0;
		int valueDelta = 0;


		for(char c: fileBuffer){
			switch (c)
			{
			case '>':
				useAddressDelta = 1;
				addressDelta++;
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				//output.append("ptr++;");
				break;
			case '<':
				useAddressDelta = 1;
				addressDelta--;
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				//output.append("ptr--;");
				break;
			case '+':
				useValueDelta = 1;
				valueDelta ++;
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
					
				}
				//output.append("++*ptr;");
				break;
			case '-':
				useValueDelta = 1;
				valueDelta --;
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
				}
				//output.append("--*ptr;");
				break;
			case '.':
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
				}
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				printNTabs(amountOfTabs);
				output.append("putchar(memory[ptr]);\n");
				break;
			case ',':
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
				}
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				printNTabs(amountOfTabs);
				output.append("*ptr=getchar();\n");
				break;
			case ':':
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
				}
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				printNTabs(amountOfTabs);
				output.append("std::cout << (int)memory[ptr];\n");
				break;
			case ';':
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
				}
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				printNTabs(amountOfTabs);
				output.append("std::cin >> a; memory[ptr]=a;\n");
				break;
			case '[':
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
				}
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				printNTabs(amountOfTabs);
				loopDepth ++;
				output.append("while (memory[ptr]) {\n");
				amountOfTabs ++;
				break;
			case ']':
				if(useAddressDelta){
					printAdress(addressDelta);
					useAddressDelta = 0;
					addressDelta = 0;
				}
				if(useValueDelta){
					printValue(valueDelta);
					useValueDelta = 0;
					valueDelta = 0;
				}
				loopDepth --;
				amountOfTabs --;
				printNTabs(amountOfTabs);
				output.append("}\n");
				break;
			
			default:
				break;
			}
		}
		if(useValueDelta){
			std::stringstream ss;
			ss << "memory[ptr]+=";
			ss << valueDelta;
			ss << ";\n";
			useValueDelta = 0;
			valueDelta = 0;
			output.append(ss.str());
		}
		if(useAddressDelta){
			std::stringstream ss;
			ss << "ptr+=";
			ss << addressDelta;
			ss << ";\n";
			useAddressDelta = 0;
			addressDelta = 0;
			output.append(ss.str());
		}


		

		output.append("\treturn 0;\n}");
		std::ofstream outputFile;
		outputFile.open("temp.cpp");
		outputFile << output;
		outputFile.close();
		system("cp temp.cpp bu.cpp");
		std::stringstream sourceName(argv[1]);
		std::string fileName;
		std::getline(sourceName, fileName, '.');
		system((std::string("g++ -O3 -ffast-math -o ") + fileName + "GCC -march=native temp.cpp").c_str());
		system((std::string("g++ -O3 -ffast-math -o ") + fileName + "CLANG -march=native temp.cpp").c_str());
		//system("g++ -O3 -ffast-math -o asmGCC.s -march=native temp.cpp -S -masm=intel");
		//system("clang++ -O3 -ffast-math -o asmCLANG.s -march=native temp.cpp -S -masm=intel && rm temp.cpp");
	}
}