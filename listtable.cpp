#include "listtable.h"

ListTable::ListTable(){}

bool ListTable::addTable(Table table){

    if(this->existTable(table.getNameTable())){
        return false;
    }

    this->tabelas.push_back(table);
    return false;
}

bool ListTable::addTable(string nameTable){
    if(this->existTable(nameTable)){
        return false;
    }

    Table aux(nameTable);
    this->tabelas.push_back(aux);

    return true;
}

bool ListTable::addField(string nameTable, string nameField, string type){
    if(!this->existTable(nameTable)){
        return false;
    }

    for (Table campo : this->tabelas) {
        //se já existe não deixo adicionar
        if(campo.getNameTable() == nameTable){
            return campo.addField(nameField,type);
        }
    }

    return false;
}

bool ListTable::removeTable(const Table &tabela){
    return this->removeTable(tabela.getNameTable());
}

bool ListTable::removeTable(string name){
    int i = 0;
    //verifico se já existe esse campo
    for (Table campo : this->tabelas) {
        if(campo.getNameTable() == name){
            this->tabelas.erase(this->tabelas.begin() + i);
        }

        i++;
    }

    return false;
}

bool ListTable::getTable(string name, Table &tabela) const{

    for (Table aux : this->tabelas) {
        if(aux.getNameTable() == name){
            tabela = aux;
            return true;
        }
    }

    return false;
}

vector<Table> ListTable::getTable() const{
    return this->tabelas;
}

bool ListTable::existTable(string nameTable) const{
    //verifico se já existe esse campo
    for (Table campo : this->tabelas) {

        //se já existe não deixo adicionar
        if(campo.getNameTable() == nameTable){
            return true;
        }
    }

    return false;
}
