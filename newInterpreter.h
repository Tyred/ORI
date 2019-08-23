#include <iostream>
#include <string>
#include <fstream>

using namespace std;


// Delete spaces

/*void erasespaces(string &str){
	int i = 0;
	while(i <= str.size()-1 && str[i] != ':'){
		if(str[i] == ' '){
			str.erase(i, str.find_first_not_of(' '));
		}
	i++;
	}
	cout << str << endl;
}*/   // Ignorar por enquanto 

// Ignore initial spaces
void ignoreSpaces(string &str){
    str.erase(0, str.find_first_not_of(" \n\t\r\f\v"));
}

string toUpperString(string &input){
    ignoreSpaces(input);
    input[0] = toupper(input[0]);
    input[1] = toupper(input[1]);

    return input; 
}
void read_metadata(){
ifstream myReadFile;
	myReadFile.open("metadados.txt");
	string output;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			myReadFile >> output;
			cout<<output;
		}
	}
}

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

// INSPIRED BY CPP DOCUMENTATION 
// http://www.cplusplus.com/reference/istream/istream/read/
// Gets file size
int get_file_size(ifstream* file){
	file->seekg(0, file->end);
	int size = file->tellg();
	file->seekg(0, file->beg);

	return size;
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
	bool parse(string input){
		int fields = get_delimiters(input, ' ');
		string command;
		string table;
		string camps;
		string reg;
		string type;
		string search;
		string key;

		// Look for 1 field commands
		if(fields == 1){
			if(input == "EB"){
				cout << "Comando " << input << " interpretado" << endl;
				return true;
			}
			else if(input == "LT"){
				cout << "Metadados disponíveis:" << endl;
				read_metadata(); 
			}
			else{
				cout << "Comando Inexistente" << endl;
				return false;
			}
		}
		// Look for 2 fields commands
		else if(fields == 2){
			command = split(input, ' ', 0);
			table = split(input, ' ', 1);
			if(command == "RT" ){
				cout << "Linha relativa a tabela " << table << ' ' << "apagada dos metadados" << endl;					
				cout << "Comando " << command << " interpretado" << endl;
				return true;
			}
			else if(command == "AT"){
				cout << "Metadados da tabela " << table << ':' << endl;
				// TO DO
				cout << "Comando " << command << " interpretado" << endl;
				return true;
			}
			else if(command == "AR"){
				cout << "Valores dos registros da ultima busca da tabela " << table << ':' << endl;
				// TO DO
				cout << "Comando " << command << " interpretado" << endl;
				return true;
			}
			else if(command == "RR"){
				cout << "Valores dos registros da ultima busca removidos da tabela " << table << ':' << endl;
				// TO DO
				cout << "Comando " << command << " interpretado" << endl;
				return true;
			}
			else{
				cout << "Comando Inexistente" << endl;
				return false;
			}
		}
		// Look for 3 fields commands
		else if(fields == 3){
			command = split(input, ' ', 0);
			if (command == "IR"){
				table = split(input, ' ', 1);
				camps = split(input, ' ', 2);
				for(int i=0; i<get_delimiters(camps, ';'); i++){
					reg = split(camps, ';', i);
					cout << "Inserido o registro " << reg << ' ' << "na tabela " << table << endl;
				}
				cout << "Comando " << command << " interpretado" << endl;
				return true;
			}
			else if (command == "GI"){
				table = split(input, ' ', 1);
				key = split(input, ' ', 2);
				cout << "Gerado o indice da chave " << key << ' ' << "da tabela " << table << endl;
				cout << "Comando " << command << " interpretado" << endl;
				return true;
			}
			else if(command == "RI"){
				table = split(input, ' ', 1);
				key = split(input, ' ', 2);
				cout << "Removido o indice da chave " << key << ' ' << "da tabela " << table << endl;
				cout << "Comando " << command << " interpretado" << endl;
				return true;
			}
			// CT commands parsing
			else if(command == "CT"){
				table = split(input, ' ', 1);
				camps = split(input, ' ', 2);
                cout << "Criada tabela com nome: " << table << endl;
    
				for(int i=0; i<get_delimiters(camps, ';'); i++){
					type = split(split(camps, ';', i), ':', 0);
					cout << type << endl;
                    
					cout << split(split(camps, ';', i), ':', 1) << endl;
					if((type != "INT" && type != "FLT" && type != "STR" && type != "BIN") || split(split(camps, ';', i), ':', 1) == ""){
						cout << "Campo inválido" << endl;
						return false;
					}
				}
				ofstream metadata;
				metadata.open("metadados.txt", std::ofstream::app);
				metadata << table << "\t" << camps << endl;
				metadata.close();
				cout << "Comando CT interpretado" << endl;
				return true;
				

			}
			else{
				cout << "Comando Inexistente" << endl;
				return false;
			}
		}
		// Look for 4 fields commands
		else if(fields == 4){
			command = split(input, ' ', 0);
			type = split(input, ' ', 1);
			table = split(input, ' ', 2) ;
			search = split(input, ' ', 3);
			if(command == "BR" && type == "N"){						
				cout << "Buscando todos " << search << ' ' << "na tabela " << table << endl;
				cout << "Comando " << command << " " << type << " interpretado" << endl;
				return true;
			}
			else if(command == "BR" && type == "U"){						
				cout << "Buscando o primeiro " << search << ' ' << "na tabela " << table << endl;
				cout << "Comando " << command << " " << type << " interpretado" << endl;
				return true;
			}
			else if(command == "CI" &&type == "A"){         // type == "H")
				cout << "Indice para a tabela estruturado como árvore " << table << ' ' << "criado, com chave de busca " << search << endl;;									
				cout << "Comando " << command << " " << type << " interpretado" << endl;				
				return true;
			}
			else if(command == "CI" &&type == "H"){         // type == "H")
				cout << "Indice para a tabela estruturado como hashing" << table << ' ' << "criado, com chave de busca " << search << endl;;									
				cout << "Comando " << command << " " << type << " interpretado" << endl;				
				return true;
			}
			else{
				cout << "Comando Inexistente" << endl;
				return false;
			}
		}
		else{
			cout << "Número de campos inválido" << endl;
			return false;
		}
	}
};