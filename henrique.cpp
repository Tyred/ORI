#include <iostream>
#include <string>
#include <istream>
#include <fstream>

#include "interpreter.h"

int main(int args, char** argv){
	using namespace std;

	string input;
	interpreter a;
	string data;
	string aux;
	char* text_data;
	string str_text_data;
	string* commands;
	int i;

	
	// Modo Texto
	if(argv[1] == NULL){
		while(input != "EB"){
			cout << "Digite:" << endl;
			getline(std::cin, input);

			a.parse(input);
		}
	}
	else{ // Modo Arquivo
		// Abre e lê arquivo
		ifstream file;
		file.open(argv[1]);
		text_data = new char[get_file_size(&file)];
		file.read(text_data, get_file_size(&file));
		str_text_data = string(text_data);

		// Separa comando por comando
		commands = new string[get_delimiters(str_text_data, '\n')];
		for(i=0; i<get_delimiters(str_text_data, '\n'); i++){
			commands[i] = split(str_text_data, '\n', i);
			if(!a.parse(commands[i])){
				cout << "Erro detectado, na linha " << i+1 << endl;
				return 1;
			}
		}
		cout << "Fim dos dados!" << endl;
	}
}