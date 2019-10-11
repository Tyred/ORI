#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <regex>
#include "helper_functions.h"
#include "registersearch.h"
#include "registerdeleted.h"
#include "listtable.h"

using namespace std;

// CT Command operation
bool create_table(const Table &table, string fields){
    string available_tables;
    string better_fields;
    string line;

    // Checks if file exists
    if(!file_exists("metadados.txt")){
        ofstream index;
        index.open("metadados.txt", ios_base::out);
        index.close();
    }

    // Replaces separator ; by tab
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
        if(split(split(available_tables, '\n', i), '\t', 0) == table){
            cout << "A tabela ja existe" << endl;
            return false;
        }
    }

    // Writes name to file if it doesn't exists
    ofstream ondex;
    ondex.open("metadados.txt", ios_base::app);
    ondex << table << '\t' << better_fields << '\n';
    ondex.close();

    // Creates Table file
    ofstream aux;
    aux.open("Table_" + table + ".txt");
    string header;

    // Writes table header
    for(int i=0;i<get_delimiters(fields, ';'); i++){
        header += split(split(fields, ';', i), ':', 1);
        if(i < get_delimiters(fields, ';')-1)
            header += '\t';
    }
    header += '\n';
    aux << header;
    aux.close();
    return true;
}

// AT Command operation
bool show_metadata(const Table &table, bool verbose=true){
    string data = readfile("metadados.txt");
    string extracted;
    bool found = false;

    // Checks line by line
    for(int i=0;i<get_delimiters(data, '\n'); i++){
        extracted = split(split(data, '\n', i), '\t', 0);
        if(extracted == table){
            if(verbose){
                cout << split(data, '\n', i) << endl;
            }
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
string get_metadata(const Table &table){
    string data = readfile("metadados.txt");
    string extracted;
    bool found = false;

    // Checks line by line
    for(int i=0;i<get_delimiters(data, '\n'); i++){
        extracted = split(split(data, '\n', i), '\t', 0);
        if(extracted == table){
            return split(data, '\n', i);
        }
    }
    return "";
}

// IR Command validation
string validate_registry(const Table &table, string fields){
    int n_fields = get_delimiters(fields, ';');
    string meta;
    string output;

    // Gets fields in metadata file
    if(show_metadata(table, false))
        meta = get_metadata(table);
    else{
        cout << "Nome de tabela inexistente" << endl;
        return "";
    }

    // Checks if number of input has correct number of fields
    if(n_fields != get_delimiters(meta, '\t')-1){
        cout << "Numero de campos do registro invalido" << endl;
        return "";
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
            regex r("\\D+");
            if(!regex_match(input[i], r)){
                cout << "Erro: o campo " << i+1 << " deve ser STR" << endl;
                return "";
            }
            output += input[i] + ';';
        }
        else if(types[i] == "INT"){
            regex r("\\d+");
            if(!regex_match(input[i], r)){
                cout << "Erro: o campo " << i+1 << " deve ser INT" << endl;
                return "";
            }
            output += input[i] + ';';

        }
        else if(types[i] == "FLT"){
            regex r("\\d+,\\d+");
            if(!regex_match(input[i], r)){
                cout << "Erro: o campo " << i+1 << " deve ser FLT" << endl;
                return "";
            }
            output += input[i] + ';';

        }

        else if(types[i] == "BIN"){
            if(!file_exists(input[i])){
                cout << "Erro: o arquivo binario no campo " << i+1 << " deve existir" << endl;
                return "";
            }
            cout << readfile(input[i]) << endl;
            output += readfile(input[i]) + ';';
        }

    }

    cout << output << endl;

    output = get_substring(output, 0, output.length()-1);
    return output;
}

// IR Command Operation
bool add_registry(const Table &table, string fields){
    string better_fields;

    fields = validate_registry(table, fields);
    if(fields == ""){
        return false;
    }

    // Fixes fields separator
    for(char const &c: fields){
        if(c != ';')
            better_fields += c;
        else
            better_fields += '\t';
    }
    cout << better_fields << endl;

    // Writes registry to table
    ofstream ondex;
    ondex.open("Table_" + table + ".txt", ios_base::app);
    ondex << better_fields << '\n';
    ondex.close();
    cout << "Registro adicionado com sucesso" << endl;
    return true;
}

// RT Command operation
bool remove_table(const Table &table){
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
        if(extracted == table){
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
        cout << "Tabela " << table << " removida da base" << endl;
        ofstream metadados;
        metadados.open("metadados.txt", ios_base::out);
        metadados << all_data;
        metadados.close();

        string filename = "Table_" + table + ".txt";
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
int get_correct_field(const Table &table, string field){
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
bool singular_search(const Table &table, string key, const vector<RegisterDeleted> &deletados){
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
bool deep_search(const Table &table, string key, const vector<RegisterDeleted> &deletados){
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

// RR remove register operation
bool remove_register(const Table &table, vector<RegisterDeleted> &deletados, vector<RegisterSearch> &resultados){
    return false;
}

// include list searchs
bool list_search_table(const Table &tabela, vector<RegisterSearch> &resultados){
    return false;
}

bool verifyType(string type){
    if(type == "INT" || type == "FLT" || type == "STR" || type == "BIN"){
        return true;
    }

    cout << "Campo com formato invalido" << endl;
    return false;
}

class interpreter {
public:
    bool parse(string input){
        int fields = get_delimiters(input, ' ');
        string command;
        string camps;
        string reg;
        string type;
        string search;
        string key;
        string tableName;
        Table table;
        vector<RegisterSearch> listaResultados;
        vector<RegisterDeleted> listaRemovidos;
        ListTable listaTabelas;

        // Look for 1 field commands
        if(fields == 1){
            //Comando para sair
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
            tableName = split(input, ' ', 1);
            table.setNameTable(tableName);

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

                if(remove_register(table, listaRemovidos, listaResultados)){
                    cout << "Valores dos registros da ultima busca removidos da tabela " << table << ':' << endl;
                } else{
                    cout << "Não foi possível remover o registro da tabela " << table << ':' << endl;
                }

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
                tableName = split(input, ' ', 1);
                table.setNameTable(tableName);
                camps = split(input, ' ', 2);

                if(add_registry(table, camps))
                    return true;
                return false;
            }
            else if (command == "GI"){
                tableName = split(input, ' ', 1);
                table.setNameTable(tableName);
                key = split(input, ' ', 2);
                cout << "Gerado o indice da chave " << key << ' ' << "da tabela " << table << endl;
                cout << "Comando " << command << " interpretado" << endl;
                return true;
            }
            else if(command == "RI"){
                tableName = split(input, ' ', 1);
                table.setNameTable(tableName);
                key = split(input, ' ', 2);
                cout << "Removido o indice da chave " << key << ' ' << "da tabela " << table << endl;
                cout << "Comando " << command << " interpretado" << endl;
                return true;
            }
            // CT commands parsing
            else if(command == "CT"){

                tableName = split(input, ' ', 1);
                table.setNameTable(tableName);
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
                listaTabelas.addTable(table);
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
                if(deep_search(table, search, listaRemovidos)) return true;
                else return false;
            }
            else if(command == "BR" && type == "U"){
                if(singular_search(table, search, listaRemovidos)) return true;
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

#endif // HELPER_FUNCTIONS_H
