// Includes
#include <iostream> 
#include <string>
#include <cctype>

using namespace std; 
// Classes
class cmd{
    string call; 
    string table;
    string fields;
    string parameter;
    string reg; //registro
    string search; 
    string key;
    public:
        setCall(string str){
            call = str;
        } 
        setTable(string str){
            table = str;
        }
        setFields(string str){
            fields = str;
        }
        setParameter(string str){            // Sets...
            Parameter = str;
        }
        setReg(string str){
            reg = str;
        }
        setSearch(string str){
            search = str;
        }
        setKey(string str){
            key = str;
        }
        
        getCall(string str){
            return call;
        } 
        getTable(string str){
            return table;
        }
        getFields(string str){
            return fields;
        }
        getParameter(string str){             // Gets...
            return Parameter;
        }
        getReg(string str){
            return reg;
        }
        getSearch(string str){
            return search;
        }
        getKey(string str){
            return key;
        }
}

class interpreter{

    void findoutCMD(string input){ // Full Input

        string str = toUpperString(input); // First two characters
        if(str == "CT" ||
           str == "RT" ||
           str == "CI" ||
           str == "RI" ||          // If its a valid command...
           str == "GI" ||
           str == "AT" ||
           str == "LT" ||
           str == "IR" ||
           str == "BR" ||
           str == "AR" ||
           str == "RR")
            setCall(str);       // Set the Call (id)
        

        /*else if string == "EB"
            // finalizar processo
        else                                        // else...
            // mensagem de erro
        */ 
    }

    string Read(string input){ // Full Input
        input.erase(0,2);
        string command = getCall();
        if (command == "CT"){
            input.erase(0, (input.find_first_not_of(' '))-1)
            int i = 0;
            string name;
            while (input[i] != ' '){
                name[i] = input[i]
                i++;
            }
            setTable(name);

        }
    }
 
}

class parser{

}

class
// Functions 

string findoutWords(const string input){
    string str;
    int position =  input.find_first_not_of(' ');

    return str; 
}

void ignoreSpaces(string &str){
    if (str[0] == ' '){
        str.erase(0, str.find_first_not_of(' '));
    }
}
string toUpperString(string input){
    ignoreSpaces(input);
    input[0] = toupper(input[0]);
    input[1] = toupper(input[1]);

    return input; 
}




// main
int main (){
    string input; 
    cin << input; 

}
