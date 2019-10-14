#include "registerdeleted.h"

RegisterDeleted::RegisterDeleted(){
}

RegisterDeleted::RegisterDeleted(Table tabela){
    this->setTable(tabela);
}

Table RegisterDeleted::getTable() const{
    return this->tabela;
}

void RegisterDeleted::setTable(Table novo){
    this->tabela = novo;
}

bool RegisterDeleted::removeInfo(const Information &info){

    int i = 0;
    for(Information aux:this->listaRemovidos){
        if(aux == info){
            this->listaRemovidos.erase(this->listaRemovidos.begin() + i);
            return true;
        }
        i++;
    }

    return false;
}

bool RegisterDeleted::addInfo(const Information &info){

    if(!this->exist(info)){
        this->listaRemovidos.push_back(info);
        return true;
    }

    return false;
}

bool RegisterDeleted::exist(const Information &info) const{
    int i = 0;

    for(Information aux:this->listaRemovidos){
        if(aux == info){
            return true;
        }

        i++;
    }
    return false;
}

bool RegisterDeleted::exist(const vector<Information> &info) const{

    for(Information removidos : this->listaRemovidos){

        for(Information atual :info){
            if (removidos == atual){
                return true;
            }
        }
    }

    return false;
}

vector<RegisterDeleted>& operator-(vector<RegisterDeleted>&registros, const vector<Table>& tabelas){
    int i = 0;
    for(Table atual : tabelas){
        registros = registros - atual;
        i++;
    }
    return registros;
}

vector<RegisterDeleted>& operator-(vector<RegisterDeleted>&registros, const Table& tabela){
    int i = 0;
    for(RegisterDeleted atual : registros){

        if(tabela.getNameTable() == atual.getTable().getNameTable()){ //se for igual
            registros.erase(registros.begin()+i);
        }

        i++;
    }
    return registros;
}


vector<RegisterDeleted>& operator+(vector<RegisterDeleted>&registros, const vector<Table>& tabelas){

    for(Table atual : tabelas){
        registros = registros + atual;
    }
    return registros;
}

vector<RegisterDeleted>& operator+(vector<RegisterDeleted>&registros, const Table& tabela){
    bool existe = false;

    for(RegisterDeleted atual : registros){
        if(tabela.getNameTable() == atual.getTable().getNameTable()){ //se for igual
            atual.setTable(tabela);
            existe = true;
            break;
        }
    }

    if(!existe){
        registros.push_back(tabela);
    }

    return registros;
}

bool existeOcorrencia(RegisterSearch &novo, vector<RegisterDeleted> &deletados){

    for(RegisterDeleted aux : deletados){

        if(novo.getTable() == aux.getTable()){

            if(aux.exist(novo.getSearch())){
                return true;
            }
        }
    }

    return false;
}
