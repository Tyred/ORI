#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <regex>
#include "helper_functions.h"
#include "register.h"
#include "listtable.h"
#include "hash.h"

using namespace std;

vector<Field> splitField(string command){
    int inicio = 0;
    string nameField = "";
    string type = "";

    vector<Field> listaCampos;

    int i = 0;
    for(char atual: command){

        //se for espaço eu removo da string
        if(atual == ' '){
            atual = '_';
        }

        //se for : siginifca que estamos terminando a primeira parte do bloco
        if(atual == ':'){
            type = command.substr(inicio, i - inicio);
            inicio = i+1;
        }

        //se for : siginifca que estamos terminando a segunda parte do bloco
        if(atual == ';'){
            nameField = command.substr(inicio, i - inicio);
            inicio = i+1;
        }

        if((i+1) == command.length()){
            nameField = command.substr(inicio, command.size());
        }
        i++;

        if(nameField != "" && type != ""){
            Field campo(nameField, type);
            listaCampos.push_back(campo);

            nameField.clear();
            type.clear();
            continue;
        }
    }

    return listaCampos;
}

void loadListTable(ListTable &listaTabelas){
    string line;
    string available_tables;

    // Checks if file exists
    if(!file_exists("metadados.txt")){
        ofstream index;
        index.open("metadados.txt", ios_base::out);
        index.close();
    }

    // Opens file for reading
    ifstream index;
    index.open("metadados.txt");
    for(; getline(index, line);) {

        string nameTable = split(line,'\t',0); //obtenho o primeiro nome
        Table atual(nameTable);

        string campos = line.substr(nameTable.size()+1, line.size()); //retiro no nome da tabela da string

        int i = 0;
        for(int i = 0; i < campos.size(); i++){

            if(campos.at(i) == '\t'){
                campos.at(i) = ';';
            }
        }

        vector<Field> listaFields = splitField(campos); //obtenho os campos e seus respectivos formatos
        atual += listaFields; //insiro os campos na tabela

        listaTabelas.addTable(atual); //insiro a tabela na listadas tabelas
    }

    index.close();
}

void loadLists(const ListTable &listaTabelas, vector<Register> &lista){

    for(Table tabelas:listaTabelas.getTable()){

        Register registros;
        registros.setTable(tabelas);
        lista.push_back(registros);
    }
}

/* "CT tabela tipo1:nome1;tipo2:nome2;tipoN:nomeN"
Cria um arquivo vazio associado ao nome tabela e o registra como ativo na
base, sendo campos uma lista de especificações dos
campos que formarão o arquivo.
*/
bool create_table(ListTable &listaTabelas, const Table &table, string fields){

    string better_fields;

    // Checks if file exists
    if(!file_exists("metadados.txt")){
        ofstream index;
        index.open("metadados.txt", ios_base::out);
        index.close();
    }

    if(listaTabelas.existTable(table.getNameTable())){
        //tabela já existe
        cout << "A tabela ja existe" << endl;
        return false;
    } else{

        ofstream ondex; // Writes name to file if it doesn't exists
        ondex.open("metadados.txt", ios_base::app);
        ondex << table << '\t' << table.getField() << '\n';
        ondex.close();

        // Creates Table file
        ofstream aux;
        aux.open(table.getNameFile());
        string header;

        for(Field atual: table.getField()){
            header += atual.getName()+'\t';
        }

        header.erase(header.size()-1, 1); //removo o último '/t'
        aux << header << '\n';
        aux.close();
        return true;
    }
}

/* "AT TABELA"
Apresenta um resumo dos metadados da tabela indicada: arquivos, campos e índices existentes.
*/
bool show_metadata(const Table &table, bool verbose=true){
    string data = readfile("metadados.txt");
    string extracted;
    bool found = false;

    // Checks line by lineS
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

// Return true if field from table is hash indexed
bool field_hash_exists(const Table &table, string field) {
    string metadata = get_metadata(table);
    string goal = "HASH:" + toUpper(field);

    if (metadata.find(goal) != string::npos)
        return true;
    else
        return false;
}

// Return true if any field from table is hash indexed
bool hash_exists(const Table &table) {
    string metadata = get_metadata(table);
    string goal = "HASH";

    if (metadata.find(goal) != string::npos)
        return true;
    else
        return false;
}

// Return registry count of given table
int registry_count(const Table &table) {
    int res = 0, i;
    string line;
    ifstream tbl("Table_" + table.getNameTable() + ".txt");

    for (i = 0; getline(tbl, line); i++);
    
    return i - 1;
}

// Return index count of given table
int index_count(const Table &table) {
    int res = 0, i = 0, achou = 0;
    string line, tabela_nome;
    ifstream mtd("metadados.txt");

    while (!mtd.eof() && !achou) {
        getline(mtd, line);
        stringstream ss(line);
        ss >> tabela_nome;

        if (tabela_nome == table.getNameTable()) {
            achou = 1;
            while (ss.rdbuf()->in_avail() != 0) {
                ss >> line;
                if (line.find("HASH:") != string::npos || line.find("TREE:") != string::npos)
                    i++;
            }
        }
    }
    
    return i;
}

// Binary file data transformation
// Turns \n into <ENDL&> and ; into <CSVAL> comma-separated-value
string transform_binary(string data){
    regex r("\\\n");
    regex r2(";");
    regex r3("\\\t");
    string replacement = "<ENDL&>";
    string replacement2 = "<CSVAL>";
    string replacement3 = "<TAB&>";

    return regex_replace(regex_replace(regex_replace(data, r, replacement), r2, replacement2), r3, replacement3);   
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
    if (!hash_exists(table)) {
        if(n_fields != get_delimiters(meta, '\t')-1){
            cout << "Numero de campos do registro invalido" << endl;
            return "";
        }
    } else {
        if(n_fields != get_delimiters(meta, '\t')- 1 - index_count(table)){
            cout << "Numero de campos do registro invalido" << endl;
            return "";
        }
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
            if(!file_exists(toLower(input[i]))){
                cout << "Erro: o arquivo binario no campo " << i+1 << " deve existir" << endl;
                return "";
            }
            output += transform_binary(readfile(toLower(input[i]))) + ';';
        }

    }

    cout << output << endl;

    output = get_substring(output, 0, output.length()-1);
    return output;
}

/* "IR tabela valor1;valor2:valorN"
Insere o registro no arquivo de tabela, usando a política de inserção adequada.
*/
bool add_registry(const Table &table, string fields){
    string better_fields;

    fields = validate_registry(table, fields);
    if(fields == ""){
        return false;
    }

    if (hash_exists(table)) {
        cout << "Existe índice hash na tabela, inserção não permitida" << endl;
        return true;
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
    ondex.open("Table_" + table + ".txt", ios_base::app);
    ondex << better_fields << '\n';
    ondex.close();
    cout << "Registro adicionado com sucesso" << endl;
    return true;
}

/* "RT tabela"
Apaga o arquivo relativo a tabela e remove seus metadados da base, bem como estruturas associadas.
*/
bool remove_table(ListTable &listaTabelas, const Table &table){

    if(!listaTabelas.existTable(table.getNameTable())){ //tabela não existe
        return false;
    }

    Table a = Table();

    listaTabelas.getTable(table.getNameTable(), a);
    vector<Field> field = a.getField();

    listaTabelas.removeTable(table.getNameTable());

    // Checks if file exists
    if(!file_exists("metadados.txt")){
        ofstream index;
        index.open("metadados.txt", ios_base::out);
        index.close();
    }

    string fileaux;
    ofstream metadados;
    metadados.open("metadados.txt", ios_base::out);

    for(Table atual : listaTabelas.getTable()){
        metadados << atual << '\t' << atual.getField() << '\n';
    }

    metadados.close();

    string filename = table.getNameFile();
    const char* char_name = filename.c_str();

    remove(char_name);


    cout << "Tabela " << table << " removida da base" << endl;

    for(Field f : field){
        if(f.getType() == "INT"){
            fileaux = "INDEX_" + toUpper(f.getName()) + "_" + toUpper(table.getNameTable()) + ".txt";
            remove(fileaux.c_str());
        }
    }



    return true;
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

/* "BR U tabela busca"
Busca em tabela pelo primeiro registro que satisfaça o critério busca.
*/ 
bool singular_search(const Table &table, string search, vector<Register> &listaRegistros){

    string nomeCampo = split(search,':',0);
    string valorCampo = split(search,':',1);

    Field campo(nomeCampo);

    // int pos = table.getPosField(nomeCampo,campo);
    int pos = 0;

    if(pos < 0){
        cout << "Campo não encontrado" << endl;
        return false;
    }

    if(campo.getType() == "BIN"){
        cout << "Não é possível pesquisar campos em formato binário!" << endl;
        return false;
    }

    if (!field_hash_exists(table, nomeCampo)) {
        string read_data = readfile(table.getNameFile());
        string* data = split(read_data, '\n');

        for(int i = 0; i < get_delimiters(read_data, '\n'); i++){

            if(split(data[i], '\t', pos) == valorCampo){

                Information info(i);
                Register registro(table);
                registro.addSearch(info);

                if(!existeOcorrenciaDeleted(table, info, listaRegistros)){
                    concatenaInformacoesSearch(registro, listaRegistros);
                    cout  << data[i] << endl;
                    return true;
                }
            }
        }
    }
    else {
        ifstream tablefile("Table_" + table.getNameTable() + ".txt");

        if (tablefile.is_open()) {
            string line;
            int chave = stoi(valorCampo);
            string hash_path("INDEX_" + nomeCampo + "_" + table.getNameTable() + ".txt");
            vector<long int> vec = busca_hash(hash_path, chave);

            if (vec.empty())
                return false;

            tablefile.seekg(vec.front());
            getline(tablefile, line);
            cout << "A: " + line << endl;

            return true;
        }
    }

    cout << "Registro não encontrado" << endl;
    return false;

}

/* "BR N tabela busca"
Busca em tabela todos os
registros que satisfaçam o
critério busca.
*/
bool deep_search(const Table &table, string search, vector<Register> &listaRegistros){

    bool found = false;

    string nomeCampo = split(search,':',0);
    string valorCampo = split(search,':',1);

    Field campo(nomeCampo);

    // int pos = table.getPosField(nomeCampo,campo);
    int pos = 0;

    if(pos < 0){
        cout << "Campo não encontrado" << endl;
        return false;
    }

    if(campo.getType() == "BIN"){
        cout << "Não é possível pesquisar campos em formato binário!" << endl;
        return false;
    }

    if (!field_hash_exists(table, nomeCampo)) {
        string read_data = readfile(table.getNameFile());
        string* data = split(read_data, '\n');

        for(int i = 0; i < get_delimiters(read_data, '\n'); i++){

            if(split(data[i], '\t', pos) == valorCampo){

                Information info(i);
                Register registro(table);
                registro.addSearch(info);

                if(!existeOcorrenciaDeleted(table, info, listaRegistros)){
                    concatenaInformacoesSearch(registro, listaRegistros);
                    cout  << data[i] << endl;
                    found = true;
                }
            }
        }
    }
    else {
        ifstream tablefile("Table_" + table.getNameTable() + ".txt");

        if (tablefile.is_open()) {
            string line;
            int chave = stoi(valorCampo);
            string hash_path("INDEX_" + nomeCampo + "_" + table.getNameTable() + ".txt");
            vector<long int> vec = busca_hash(hash_path, chave);

            if (vec.empty())
                found = false;
            else
                found = true;

            for (vector<long int>::iterator it = vec.begin(); it != vec.end(); it++) {
                tablefile.seekg(*it);
                getline(tablefile, line);
                cout << "A: " + line << endl;
            }
        }
    }

    if(found){
        return true;
    } else {
        cout << "Registro não encontrado" << endl;
        return false;
    }

}

/* "RR tabela"
Remove, segundo a política de remoção da
tabela, todos os registros da última busca realizada.
*/
bool remove_register(const Table &table, vector<Register> &registros){

    if (hash_exists(table)) {
        cout << "Existe índice hash na tabela, remoção não permitida" << endl;
        return false;
    }

    for(int i = 0; i < registros.size(); i++){

        if(registros.at(i).getTable() == table){

            if(registros.at(i).getSearch().size() == 0){ //se não houver nenhum registro a ser exibido
                return false;
            }

            for(Information info: registros.at(i).getSearch()){
                registros.at(i).addDeleted(info);
            }

            registros.at(i).clearSearch();
            return true;
        }
    }

    return false;
}

// CI A/H Operation
bool create_index(string type, string table, string field){
    string metadata;
    string accumulator;
    string new_entry;
    string extracted;
    string extracted_name;
    bool found = false;

    if(type == "A"){
        metadata = readfile("metadados.txt");

        // Checks line by line
        for(int i=0;i<get_delimiters(metadata, '\n'); i++){
            extracted = split(metadata, '\n', i);
            extracted_name = split(split(metadata, '\n', i), '\t', 0);
            if(toUpper(extracted_name) == table){
                for(int j=1;j<get_delimiters(extracted, '\t');j++){
                    if(field == toUpper(split(split(extracted, '\t', j), ':', 1))){
                        if(get_substring(split(extracted, '\t', j), 0, 3) == "INT"){
                            found = true;
                            break;
                        }
                    }
                }
                new_entry = split(metadata, '\n', i) + "\tTREE:" + field + '\n';  
            }
            else{
                accumulator += split(metadata, '\n', i) + '\n';
            }
        }
    }
    else if(type == "H"){
        metadata = readfile("metadados.txt");
        // Checks line by line
        for(int i=0;i<get_delimiters(metadata, '\n'); i++){
            extracted = split(metadata, '\n', i);
            extracted_name = split(extracted, '\t', 0);
            if(toUpper(extracted_name) == table){
                for(int j=1;j<get_delimiters(extracted, '\t');j++){
                    if(field == toUpper(split(split(extracted, '\t', j), ':', 1))){
                        if(get_substring(split(extracted, '\t', j), 0, 3) == "INT"){
                            found = true;
                            break;
                        }
                    }
                }
                new_entry = split(metadata, '\n', i) + "\tHASH:" + field + '\n';                        

            }
            else{
                accumulator += split(metadata, '\n', i) + '\n';
            }
        }
    }
    else{
        cout << "Tipo de indexação inválida: (" << type << ")" << endl;
        return false;
    }

    // If table exists
    if(found){
        ofstream metafile;
        ofstream indexfile;
        metafile.open("metadados.txt", ios_base::out);
        indexfile.open("INDEX_" + field + "_" + table + ".txt", ios_base::out);
        indexfile.close();

        if(accumulator[0] == '\n')
            accumulator = strip(accumulator, '\n') + "\n";
        if(accumulator[accumulator.length()-1] == '\n')
            accumulator = get_substring(accumulator, 0, accumulator.length()-2);

        if (accumulator != "")
            metafile << accumulator << "\n" << new_entry;
        else
            metafile << new_entry;

        metafile.close();

        // Cria índice hash
        if (type == "H") {
            Table tbl(table);

            int reg_count = registry_count(tbl);
            int field_pos = 0;
            int reg_pos;

            string hash_path("INDEX_" + field + "_" + table + ".txt");
            string line, field_cont;
            Par p;

            inicializa_hash(hash_path, reg_count);

            ifstream tablefile("Table_" + table + ".txt");
            if (tablefile.is_open()) {
                // Descarta o cabeçalho na tabela
                getline(tablefile, line);

                // Solução temporária para achar a pos do campo, já que 
                // getFieldPos() depende de adicionar manualmente os campos
                stringstream table_fields(line);
                do {
                    if (table_fields.rdbuf()->in_avail() == 0) {
                        field_pos = -1;
                        break;
                    }

                    table_fields >> line;
                    field_pos++;
                } while (line != field);

                if (field_pos == -1) {
                    cout << "Campo " + field + " não encontrado na tabela" << endl;
                    exit(1);
                }
                
                for (int i = 0; i < reg_count; i++) {
                    p.cont = tablefile.tellg();
                    // p.cont = i;
                    getline(tablefile, line);

                    // Gambiarra para pegar o field_pos ésimo elemento. Ainda 
                    // estou pensando numa forma melhor de fazer isso
                    stringstream ssline(line);
                    for (int j = 0; j < field_pos - 1; j++) {
                        ssline >> field_cont;
                    }
                    ssline >> p.chave;

                    insere_hash(hash_path, p);
                }
                // Para debugar os conteúdos da hash
                // imprime_hash(hash_path);
            }
            else {
                cout << "Não pode abrir arquivo da tabela " << table << endl;\
                exit(1);
            }
            tablefile.close();
        }

        return true;
    }
    else{
        cout << "Tabela ou campo inválidos" << endl;
        return false;
    }
}

// RI Operation
bool remove_index(string table, string key){
    string metadata;
    string accumulator;
    string new_entry;
    string extracted;
    bool found;
    bool index;

    metadata = readfile("metadados.txt");

    for(int i=0;i<get_delimiters(metadata, '\n');i++){
        extracted = split(metadata, '\n', i);
        if(toUpper(split(extracted, '\t', 0)) == table){
            new_entry += split(extracted, '\t', 0) + '\t';
            for(int j=1;j<get_delimiters(extracted, '\t');j++){
                index = false;
                if(split(split(extracted, '\t', j), ':', 0) == "TREE" || split(split(extracted, '\t', j), ':', 0) == "HASH"){
                    if(toUpper(split(split(extracted, '\t', j), ':', 1)) == key){
                        found = true;
                        index = true;
                    }
                }
                if(!index)
                    new_entry += split(extracted, '\t', j) + "\t";
            }
        }
        else{
            accumulator += split(metadata, '\n', i) + '\n';
        }
    }

    new_entry = get_substring(new_entry, 0, new_entry.length()-1) + '\n';

    if(found){
        ofstream metafile;
        metafile.open("metadados.txt", ios_base::out);

        if(accumulator[0] == '\n')
            accumulator = strip(accumulator, '\n') + "\n";
        if(accumulator[accumulator.length()-1] == '\n')
            accumulator = get_substring(accumulator, 0, accumulator.length()-2);

        metafile << accumulator << "\n" << new_entry;
        metafile.close();
        
        string fileaux = "INDEX_" + toUpper(key) + "_" + toUpper(table) + ".txt";
        const char *filename = fileaux.c_str();
        remove(filename);
        return true;
    }
    else
        return false;

}

bool typeValid(string type){

    if( type == "INT" || type == "FLT" || type == "STR" || type == "BIN"){
        return true;
    }

    cout << "Campo com formato invalido" << endl;
    return false;
}

class interpreter {

public:
    interpreter(){
        loadListTable(this->listaTabelas);
        loadLists(this->listaTabelas, this->listaRegistros);
    }

    vector<Register> listaRegistros;
    ListTable listaTabelas;

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
                if(remove_table(listaTabelas,table)){
                    listaTabelas.removeTable(table);
                    listaRegistros = listaRegistros - table;
                    return true;
                }
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

                if(remove_register(table, listaRegistros)){
                    cout << "Valores dos registros da ultima busca removidos da tabela " << table << endl;
                } else{
                    cout << "Não foi possível remover o registro da tabela " << table << endl;
                }

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
                if(remove_index(tableName, key)){
                    cout << "Removido o indice da chave " << key << ' ' << "da tabela " << table << endl;
                    return true;
                }
                return false;
            }
            // CT commands parsing
            else if(command == "CT"){

                tableName = split(input, ' ', 1);
                table.setNameTable(tableName);
                camps = split(input, ' ', 2);
                table += splitField(camps);//adiciono os campos nas respectivas variáveis

                for(Field aux:table.getField()){
                    if(!typeValid(aux.getType())){ //se for um formato inadequado
                        cout << "Formato de campo inválido! " <<'\n';
                        return false;
                    }
                }

                if(!create_table(listaTabelas, table, camps)){
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
            tableName = split(input, ' ', 2) ;
            search = split(input, ' ', 3);

            table.setNameTable(tableName);


            /*
            if(!listaTabelas.getTable(tableName, table)){
                cout << "Tabela inexistente!" <<endl;
                return false;
            }
            */

            if(command == "BR" && type == "N"){
                cout << endl;
                if(deep_search(table, search, listaRegistros)){
                    return true;
                } else{
                    return false;
                }
            }
            else if(command == "BR" && type == "U"){
                cout << endl;
                if(singular_search(table, search, listaRegistros)){
                    return true;
                } else {
                    return false;
                }
            }
            else if(command == "CI" && type == "A"){         // type == "H")
                if(create_index(type, tableName, search)){
                    cout << "Indice para a tabela estruturado como árvore " << table << ' ' << "criado, com chave de busca " << search << endl;
                    return true;
                }
                else{
                    return false;
                }
            }
            else if(command == "CI" && type == "H"){         // type == "H")
                if(create_index(type, tableName, search)){
                    cout << "Indice para a tabela estruturado como hashing" << table << ' ' << "criado, com chave de busca " << search << endl;
                    return true;
                }
                else{
                    return false;
                }
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
