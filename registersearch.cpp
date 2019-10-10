#include "registersearch.h"

RegisterSearch::RegisterSearch(){
}

RegisterSearch::RegisterSearch(Table tabela){
    this->tabela = tabela;
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
