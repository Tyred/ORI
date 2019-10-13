#include "field.h"

Field::Field(string name, string type){
    this->type = type;
    this->name = name;
}

string Field::getName() const{
    return this->name;
}

string Field::getType() const{
    return this->type;
}

void Field::setName(string newName){
    this->name = newName;
}

string operator+(const string& txt, const Field& field){

    string aux = "";
    if(field.getName() != "" && field.getType() != ""){
        aux = txt + field.getType() + ":"+field.getName();
    }

    return aux;
}

string& Field::operator+(string &txt){

    if(this->getName() != "" && this->getType() != ""){
        txt = this->getType() + ":"+this->getName() + txt;
    }

    return txt;
}

ostream& operator<<(ostream& os, const Field& field){

    if(field.getName() != "" && field.getType() != ""){
        os << field.getType() + ":"+field.getName();
    }

    return os;
}

ostream& operator<<(ostream& os, const vector<Field>& field){
    string concat = "";

    int i = 0;

    for(int i = 0; i < field.size();i++){

        if(field.at(i).getName() != "" && field.at(i).getType() != ""){ //se não tiver campo vazio
            concat += field.at(i).getType() + ":"+ field.at(i).getName();

            if(i+1 < field.size()){ //se não for o último
                concat += '\t';
            }
        }

    }

    os << concat;
    return os;
}

bool Field::operator==(const Field& field)const{
    return this->name == field.name;
}
