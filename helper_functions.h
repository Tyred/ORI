#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <string>
#include <fstream>
#include <regex>

using namespace std;

// Pega uma substring da string text da posição begin até a end
// get_substring("Teste", 1, 5)  -> "este"
string get_substring(string text, int begin, int end){
    string output;

    for(int i=begin;i<end;i++)
        output += text[i];

    return output;
}

// Remove espaços múltiplos seguidos da string input
string prepare_string(string input){
    // Erase n* spaces
    regex r("\\s\\s*");
    input = regex_replace(input, r, " ");

    // Fix initial and last separators
    if(input[0] == ' '){
        input = get_substring(input, 1, input.length());
    }

    if(input[input.length()-1] == ';'){
        input = get_substring(input, 0, input.length()-1);
    }

    return input;
}

// Ignore initial spaces
void ignoreSpaces(string &str){
    str.erase(0, str.find_first_not_of(" \n\t\r\f\v"));
}

// Lê o arquivo e retorna seu conteúdo em data
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

// Quebra a string input em pedaços usando o char delim como delimitador e pega a posição n
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

// Pega o número de delimitadores +1 pr saber quantos campos uma string s possui
int get_delimiters(string s, const char delim){
    int amount = 1;

    for(char const &c : s){
        if(c == delim){
            amount++;
        }
    }
    return amount;
}

// Faz igual a função split, mas retorna a lista de separações no delimitador delim
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

// Passa o texto para maiúsculo
string toUpperString(string &input){

    for(int i = 0; i < input.size();i++){
        input.at(i) = toupper(input.at(i));
    }

    return input;
}

// Lê um arquivo e retorna uma lista de linhas
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
// Checa se um arquivo existe ou não
bool file_exists(string filename){
    ifstream f;
    f.open(filename);
    if(f) return true;
    else return false;
}

#endif // HELPER_FUNCTIONS_H
