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
