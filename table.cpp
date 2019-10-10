#include "table.h"

Table::Table(const string nameTable){
    this->name_table = nameTable;
}

string Table::getNameTable() const{
    return this->name_table;
}

vector<Field> Table::getField() const{
    return this->campos;
}

bool Table::getField(string nameField, Field &field) const{

    for (Field campo : this->campos) {

        if(campo.getName() == nameField){
            field = campo;
            return true;
        }
    }

    return false;

}

bool Table::getField(int position, Field &field) const{

    if(position > this->campos.size()){
        return false;
    }

    field = this->campos.at(position);
    return true;
}

void Table::setNameTable(const string newName){
    this->name_table = newName;
}

bool Table::addField(const Field novo){

    if(this->existField(novo.getName())){
        return false;
    }

    this->campos.push_back(novo); //adiciono no final
    return true;
}

bool Table::addField(const string nameField, const string typeField){

    Field novo(nameField, typeField);
    return this->addField(novo);
}

bool Table::removeField(const int position){

    //verifico se o index está de acordo com a quantidade
    if(position >= 0 && this->campos.size() < position){
        return false;
    }

    this->campos.erase(this->campos.begin() + (position -1)); //removo de acordo com o index
    return true;
}

bool Table::removeField(const string nameField){
    int position = -1;
    int i = 0;
    for (Field campo : this->campos) {

        if(campo.getName() == nameField){
            position = i;
        }

        i++;
    }

    return this->removeField(position);
}

bool Table::setNameField(const string name, const string newName){

    //procuro o campo a ser alterado
    for (Field campo : this->campos) {

        if(campo.getName() == name){
            //se encontrar altero o nome do campo
            campo.setName(newName);
            return true;
        }
    }

    return false;
}

bool Table::setNameField(const int position, const string newName){

    //verifico se o index está de acordo com a quantidade
    if(position >= 0 && this->campos.size() < position){
        return false;
    }

    this->campos.at(position).setName(newName);
    return true;
}

bool Table::existField(string nameField) const{

    //verifico se já existe esse campo
    for (Field campo : this->campos) {
        if(campo.getName() == nameField){
            return true;
        }
    }

    return false;
}

//escrever o nome da tabela
std::ostream& operator<<(std::ostream& stream, const Table& aux) {
    stream << aux.getNameTable();
    return stream;
}

string& Table::operator+(string &txt){
    txt =  this->getNameTable() + txt;
    return txt;
}

string operator+(const string& txt, const Table& table){
    return txt + table.getNameTable();
}

bool operator==(const string& txt, const Table& table){
    return txt == table.getNameTable();
}

bool operator==(const Table& table, const string& txt){
    return txt == table.getNameTable();
}

