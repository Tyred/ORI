#include "register.h"

Register::Register(){
}

Register::Register(Table tabela){
    this->tabela = tabela;
}

Table Register::getTable() const{
    return this->tabela;
}

void Register::setTable(Table novo){
    this->tabela = novo;
}

//search
vector<Information> Register::getSearch() const{
    return this->listaPesquisados;
}

bool Register::existSearch(const Information &info) const{

    for(Information aux:this->listaPesquisados){
        if(aux == info){
            return true;
        }
    }
    return false;
}

bool Register::existSearch(const vector<Information> &info) const{

    for(Information obj : this->listaPesquisados){

        for(Information atual : info){
            if (obj == atual){
                return true;
            }
        }
    }

    return false;
}

bool Register::addSearch(Information info){
    this->clearSearch();
    this->listaPesquisados.push_back(info);
    return true;
}

bool Register::addSearch(const vector<Information> &list){

    this->clearSearch();

    for(Information obj : list){
        this->listaPesquisados.push_back(obj);
    }

    return true;
}

bool Register::removeSearch(const Information &info){

    for (int i = 0; i < this->listaPesquisados.size(); i++) {

        if(this->listaPesquisados.at(i) == info){
            this->listaPesquisados.erase(this->listaPesquisados.begin() + i);
            return true;
        }
    }

    return false;
}

void Register::clearSearch(){
    this->listaPesquisados.clear();
}

//deleted

vector<Information> Register::getDeleted() const{
    return this->listaRemovidos;
}

bool Register::existDeleted(const Information &info) const{

    for(Information removido : this->listaRemovidos){

        if(removido == info){
            return true;
        }
    }
    return false;
}

bool Register::existDeleted(const vector<Information> &info) const{

    for(Information removidos : this->listaRemovidos){

        for(Information atual :info){
            if (removidos == atual){
                return true;
            }
        }
    }

    return false;
}

bool Register::addDeleted(Information info){

    if(!this->existDeleted(info)){
        this->listaRemovidos.push_back(info);
        return true;
    }

    return false;
}

bool Register::addDeleted(const vector<Information> &list){

    for(Information obj : list){
        for (Information removidos : this->listaRemovidos){

            if(obj == removidos){
                continue;
            } else{

                this->listaRemovidos.push_back(obj);
                return true;
            }
        }
    }

    return true;
}

bool Register::removeDeleted(const Information &info){

    for (int i = 0; i < this->listaRemovidos.size(); i++) {

        if(this->listaRemovidos.at(i) == info){
            this->listaRemovidos.erase(this->listaRemovidos.begin() + i);
            return true;
        }
    }

    return false;
}

void Register::clearDeleted(){
    this->listaRemovidos.clear();
}

//operators

vector<Register>& operator-(vector<Register>&registros, const vector<Table>& tabelas){

    for(Table atual : tabelas){
        registros = registros - atual;
    }
    return registros;
}

vector<Register>& operator-(vector<Register>&registros, const Table& tabela){
    int i = 0;
    for(Register atual : registros){

        if(tabela == atual.getTable()){ //se for igual
            registros.erase(registros.begin()+i);
        }

        i++;
    }
    return registros;
}

vector<Register>& operator+(vector<Register>&registros, const vector<Table>& tabelas){
    for(Table atual : tabelas){
        registros = registros + atual;
    }
    return registros;
}

vector<Register>& operator+(vector<Register>&registros, const Table& tabela){
    bool existe = false;

    for(Register atual : registros){
        if(tabela == atual.getTable()){ //se for igual
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

bool existeOcorrenciaSearch(const Table tabela, const Information &novo, vector<Register> &registros){

    for (int i = 0; i < registros.size(); i++) {

        if(registros.at(i).getTable() == tabela){
            if(registros.at(i).existSearch(novo)){
                return true;
            }
        }
    }

    return false;
}

bool existeOcorrenciaDeleted(const Table tabela, const Information &novo, vector<Register> &listaRegistro){

    for (int i = 0; i < listaRegistro.size(); i++) {

        if(listaRegistro.at(i).getTable() == tabela){
            if(listaRegistro.at(i).existDeleted(novo)){
                return true;
            }
        }
    }

    return false;
}

void concatenaInformacoesSearch(const Register &registro_novo, vector<Register> &lista_resultados){

    for (int i = 0; i < lista_resultados.size(); i++) {

        if(lista_resultados.at(i).getTable() == registro_novo.getTable()){ //se for a mesma tabela
            lista_resultados.at(i).addSearch(registro_novo.getSearch());
            break;
        }
    }

}

void concatenaInformacoesDeleted(const Register &registro_novo, vector<Register> &lista_resultados){

    for (int i = 0; i < lista_resultados.size(); i++) {

        if(lista_resultados.at(i).getTable() == registro_novo.getTable()){ //se for a mesma tabela
            lista_resultados.at(i).addDeleted(registro_novo.getDeleted());
            break;
        }
    }

}
