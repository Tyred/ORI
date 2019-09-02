#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <regex>

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

string readfile(string filename){
	ifstream infile;
	string data, line;
	infile.open(filename);

	for(line; getline(infile, line); ) {
		data += line + "\n";
	}

	infile.close();
	return data;
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

// Gets amount of fields in a string
int get_delimiters(string s, const char delim){
	int amount = 1;

	for(char const &c : s){
		if(c == delim){
			amount++;
		}
	}
	return amount;
}

// Splits and returns string*
string* split(std::string input, const char delim){
	int j=0;
	string* aux = new string[get_delimiters(input, delim)];

	for(int i=0;i<input.length();i++){
		if(input[i] != delim){
			aux[j] += input[i];
		}
		else if(input[i] == delim){
			j++;
		}
	}
	return aux;
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

// Read file and return array of lines
string* readlines(string name, int &size){
	string* lines;	
	string all_data;
	ifstream infile;
	infile.open(name);
	while(!infile.eof()){
		infile >> all_data;
	}
	infile.close();

	lines = new string[get_delimiters(all_data, '\n')];
	for(int i=0;i<get_delimiters(all_data, '\n'); i++){
		lines[i] = split(all_data, '\n', i);
	}
	size = get_delimiters(all_data, '\n');
	return lines;
}

// Inspired by http://www.cplusplus.com/forum/general/219004/
// Checks whether a file exists or not
bool file_exists(string filename){
	ifstream f;
	f.open(filename);
	if(f) return true;
	else return false;
}

// CT Command operation
bool create_table(string name, string fields){
	string available_tables;
	string better_fields;
	string line;

	// Checks if file exists
	if(!file_exists("metadados.txt")){
		ofstream index;
		index.open("metadados.txt", ios_base::out);
		index.close();
	}

	// Fixes fields separator
	for(char const &c: fields){
		if(c != ';')
			better_fields += c;
		else
			better_fields += '\t';
	}

	// Opens file for reading
	ifstream index;
	index.open("metadados.txt");
	for(line; getline(index, line); ) {
		available_tables += line + "\n";
	}

	index.close();

	// Checks line by line
	for(int i=0;i<get_delimiters(available_tables, '\n'); i++){
		if(split(split(available_tables, '\n', i), '\t', 0) == name){
			cout << "A tabela ja existe" << endl;
			return false;
		}
	}

	// Writes name to file if it doesn't exists
	ofstream ondex;
	ondex.open("metadados.txt", ios_base::app);
	ondex << name << '\t' << better_fields << '\n';
	ondex.close();
		
	// Creates Table file
	ofstream table;
	table.open("Table_" + name + ".txt");
	string header;

	// Writes table header
	for(int i=0;i<get_delimiters(fields, ';'); i++){
		header += split(split(fields, ';', i), ':', 1);
		if(i < get_delimiters(fields, ';')-1)
			header += '\t';
	}
	header += '\n';
	table << header;
	table.close();
	return true;
}

// AT Command operation
bool show_metadata(string name, bool verbose=true){
	string data = readfile("metadados.txt");
	string extracted;
	bool found = false;

	// Checks line by line
	for(int i=0;i<get_delimiters(data, '\n'); i++){
		extracted = split(split(data, '\n', i), '\t', 0);
		if(extracted == name){
			if(verbose)
				cout << split(data, '\n', i) << endl;
			found = true;
			break;
		}
	}	
	if(!found){
		if(verbose)
			cout << "Registro nao encontrado" << endl;
		return false;
	}
	return true;
}

// Gets table name metadata
string get_metadata(string name){
	string data = readfile("metadados.txt");
	string extracted;
	bool found = false;

	// Checks line by line
	for(int i=0;i<get_delimiters(data, '\n'); i++){
		extracted = split(split(data, '\n', i), '\t', 0);
		if(extracted == name){
			return split(data, '\n', i);
		}
	}	
	return "";
}

// IR Command validation
bool validate_registry(string name, string fields){
	int n_fields = get_delimiters(fields, ';');
	string meta;

	if(show_metadata(name, false))
		meta = get_metadata(name);
	else{
		cout << "Nome de tabela inexistente" << endl;
		return false;
	}

	if(n_fields != get_delimiters(meta, '\t')-1){
		cout << "Numero de campos do registro invalido" << endl;
		return false;
	}

	string* data = split(meta, '\t'); 
	string* types = new string[get_delimiters(meta, '\t')-1];
	string* input = split(fields, ';');

	// Get field types
	for(int i=0;i<n_fields;i++){
		types[i] = split(data[i+1], ':', 0);
	}

	// Check type matching
	for(int i=0;i<n_fields;i++){
		if(types[i] == "STR"){
			regex r("[a-zA-Z]+");
			if(!regex_match(input[i], r)){
				cout << "Erro: o campo " << i+1 << " deve ser STR" << endl;
				return false;
			}
		}
		if(types[i] == "INT"){
			regex r("[0-9]+");
			if(!regex_match(input[i], r)){
				cout << "Erro: o campo " << i+1 << " deve ser INT" << endl;
				return false;
			}
		}
		if(types[i] == "FLT"){
			regex r("[0-9]+,[0-9]+");
			if(!regex_match(input[i], r)){
				cout << "Erro: o campo " << i+1 << " deve ser FLT" << endl;
				return false;
			}
		}

		if(types[i] == "BIN"){
			cout << "BIN\n";
			if(!file_exists(input[i])){
				cout << "Erro: o arquivo binario no campo " << i+1 << " deve existir" << endl;
				return false;
			}
		}

	}

	return true;
}

// IR Command Operation
bool add_registry(string name, string fields){
	string better_fields;

	if(!validate_registry(name, fields)){
		return false;
	}

	// Fixes fields separator
	for(char const &c: fields){
		if(c != ';')
			better_fields += c;
		else
			better_fields += '\t';
	}

	// Writes registry to table
	ofstream ondex;
	ondex.open("Table_" + name + ".txt", ios_base::app);
	ondex << better_fields << '\n';
	ondex.close();	
	cout << "Registro adicionado com sucesso" << endl;
	return true;
}


// RT Command operation
bool remove_table(string name){
	string all_data;
	string available_tables;
	bool found = false;
	string extracted;

	// Checks if file exists
	if(!file_exists("metadados.txt")){
		ofstream index;
		index.open("metadados.txt", ios_base::out);
		index.close();
	}

	// Opens file for reading
	available_tables = readfile("metadados.txt");

	// Checks line by line
	for(int i=0;i<get_delimiters(available_tables, '\n'); i++){
		extracted = split(split(available_tables, '\n', i), '\t', 0);
		if(extracted == name){
			// If found in tables index
			found = true;
		}
		else if(extracted == ""){
			continue;
		}
		else{
			all_data += split(available_tables, '\n', i) + '\n';
		}
	}
	if(found){
		cout << "Tabela " << name << " removida da base" << endl;
		ofstream metadados;
		metadados.open("metadados.txt", ios_base::out);
		metadados << all_data;
		metadados.close();

		string filename = "Table_" + name + ".txt";
		const char* char_name = filename.c_str();
		remove(char_name);
		return true;
	}
	return false;	
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

// Gets field position of a table
int get_correct_field(string table, string field){
	string metadata = readfile("metadados.txt");
	string* metaline = split(metadata, '\n');

	for(int i=0;i<get_delimiters(metadata, '\n');i++){
		if(split(metaline[i], '\t', 0) == table){
			for(int j=1; j<get_delimiters(metaline[i], '\t');j++){
				if(split(split(metaline[i], '\t', j), ':', 1) == split(field, ':', 0)){
					return j-1;
				}
			}
		}
	}
	return -1;
}

// BR U Command operation
bool singular_search(string table, string key){
	int pos = get_correct_field(table, key);

	if(pos < 0){
		cout << "Campo nao encontrado" << endl;
		return false;
	}

	string read_data = readfile("Table_" + table + ".txt");
	string* data = split(read_data, '\n');

	for(int i=0; i<get_delimiters(read_data, '\n'); i++){
		string key_value = split(key, ':', 1);
		if(split(data[i], '\t', pos) == key_value){
			cout << data[i] << endl;
			return true;
		}
	}
	cout << "Registro nao encontrado" << endl;
	return false;
}

// BR N Command operation
bool deep_search(string table, string key){
	bool found = false;
	int pos = get_correct_field(table, key);

	if(pos < 0){
		cout << "Campo não encontrado" << endl;
		return false;
	}

	string read_data = readfile("Table_" + table + ".txt");
	string* data = split(read_data, '\n');

	for(int i=0; i<get_delimiters(read_data, '\n'); i++){
		string key_value = split(key, ':', 1);
		if(split(data[i], '\t', pos) == key_value){
			cout << data[i] << endl;
			found = true;
		}
	}
	if(found)
		return true;

	cout << "Registro nao encontrado" << endl;
	return false;
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
				cout << readfile("metadados.txt") << endl;
				return true;
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
				if(remove_table(table))
					return true;
				else{
					cout << "A tabela nao existe\n";
					return false;
				}
			}
			else if(command == "AT"){
				if(show_metadata(table))
					return true;
				return false;
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

			if(command == "IR"){
				table = split(input, ' ', 1);
				camps = split(input, ' ', 2);

				if(add_registry(table, camps))
					return true;
				return false;
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
    
				for(int i=0; i<get_delimiters(camps, ';'); i++){
					type = split(split(camps, ';', i), ':', 0);
                    
					if((type != "INT" && type != "FLT" && type != "STR" && type != "BIN") || split(split(camps, ';', i), ':', 1) == ""){
						cout << "Campo invalido" << endl;
						return false;
					}
				}

				if(!create_table(table, camps)){
					return false;
				}

                cout << "Criada tabela com nome: " << table << endl;
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
				if(deep_search(table, search)) return true;
				else return false;
			}
			else if(command == "BR" && type == "U"){						
				if(singular_search(table, search)) return true;
				else return false;
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