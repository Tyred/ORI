#include "registersearch.h"

RegisterSearch::RegisterSearch(){
}

RegisterSearch::RegisterSearch(Table tabela){
    this->tabela = tabela;
}

Table RegisterSearch::getTable() const{
    return this->tabela;
}
void RegisterSearch::setTable(Table novo){
    this->tabela = novo;
}

bool RegisterSearch::addSearch(Information info){
    this->clearSearch();
    this->index_results.push_back(info);
    return true;
}

bool RegisterSearch::addSearch(const vector<Information> &list){
    this->clearSearch();

    for(Information obj : list){
        this->index_results.push_back(obj);
    }

    return true;
}

void RegisterSearch::clearSearch(){
    this->index_results.clear();
}

bool RegisterSearch::exist(const Information &info) const{
    int i = 0;

    for(Information aux:this->index_results){
        if(aux == info){
            return true;
        }

        i++;
    }
    return false;
}

vector<RegisterSearch>& operator-(vector<RegisterSearch>&registros, const vector<Table>& tabelas){
    int i = 0;
    for(Table atual : tabelas){
        registros = registros - atual;
        i++;
    }
    return registros;
}

vector<RegisterSearch>& operator-(vector<RegisterSearch>&registros, const Table& tabela){
    int i = 0;
    for(RegisterSearch atual : registros){

        if(tabela == atual.getTable()){ //se for igual
            registros.erase(registros.begin()+i);
        }

        i++;
    }
    return registros;
}

vector<RegisterSearch>& operator+(vector<RegisterSearch>&registros, const vector<Table>& tabelas){

    for(Table atual : tabelas){
        registros = registros + atual;
    }
    return registros;
}

vector<RegisterSearch>& operator+(vector<RegisterSearch>&registros, const Table& tabela){
    bool existe = false;

    for(RegisterSearch atual : registros){
        if(tabela == atual.getTable()){ //se for igual
            //atual.setTable(tabela);
            existe = true;
            break;
        }
    }

    if(!existe){
        registros.push_back(tabela);
    }

    return registros;
}
