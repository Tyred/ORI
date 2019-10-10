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
        aux = txt + field.getType() + ":"+field.getType();
    }

    return aux;
}

string& Field::operator+(string &txt){

    if(this->getName() != "" && this->getType() != ""){
        txt = this->getType() + ":"+this->getType()+txt;
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

    for(Field aux: field){
        if(aux.getName() != "" && aux.getType() != ""){
            os << concat + aux + '\t';
        }
    }

    return os;
}
