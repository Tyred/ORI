#include <iostream>
#include <string>

using namespace std;

// Breaks the string into pieces and get nth separation
std::string split(std::string input, const char delim, const int n){
	int separation = 0;
	std::string aux = "";

	for(int i=0;i<input.length();i++){
		if(input[i] != delim && separation == n){
			aux += input[i];

		}
		else if(input[i] == delim){
			separation++;
			if(separation>n){
				return aux;
			}
		}
	}
	return aux;
}

// Gets amount of fields in a string
int get_delimiters(string s, const char delim){
	int amount = 1;

	for(int i=0;i<s.length();i++){
		if(s[i] == delim){
			amount++;
		}
	}
	return amount;
}


class interpreter{
public:
	void parse(string input){
		int fields = get_delimiters(input, ' ');
		string command;
		string table;
		string camps;
		string reg;
		string type;
		string search;

		// Look for 1 field commands
		if(fields == 1){
			if(input == "LT"){
				cout << "Listando o nome de todas as tabelas da base" << endl;
			}
			else{
				cout << "Comando Inexistente" << endl;
			}
		}
		// Look for 2 fields commands
		else if(fields == 2){
			command = split(input, ' ', 0);
			if(command == "RT" || command == "AT" || command == "AR" || command == "RR"){
				cout << "Comando " << command << " interpretado" << endl;
			}
			else{
				cout << "Comando Inexistente" << endl;
			}
		}
		// Look for 3 fields commands
		else if(fields == 3){
			command = split(input, ' ', 0);
			if(command == "CT" || command == "IR"){
				cout << "Comando " << command << " interpretado" << endl;
			}
			else{
				cout << "Comando Inexistente" << endl;
			}
		}
		// Look for 4 fields commands
		else if(fields == 4){
			command = split(input, ' ', 0);
			type = split(input, ' ', 1);
			if(command == "BR" && (type == "N" || type == "U")){
				cout << "Comando " << command << " " << type << " interpretado" << endl;
			}
			else{
				cout << "Comando Inexistente" << endl;
			}
		}
		else{
			cout << "Número de campos inválido" << endl;
		}
	}
};