#ifndef REGISTER_H
#define REGISTER_H

#include "table.h"
#include "information.h"

class Register {

private:
    Table tabela;
    vector<Information> listaRemovidos;
    vector<Information> listaPesquisados;
public:
    Register(Table tabela);
    Register();

    Table getTable() const;
    void setTable(Table novo);

    //search
    vector<Information> getSearch() const;
    bool existSearch(const Information &info) const;
    bool existSearch(const vector<Information> &info) const;
    bool addSearch(Information info);
    bool addSearch(const vector<Information> &list);
    bool removeSearch(const Information &info);
    void clearSearch();

    //deleted
    vector<Information> getDeleted() const;
    bool existDeleted(const Information &info) const;
    bool existDeleted(const vector<Information> &info) const;
    bool addDeleted(Information info);
    bool addDeleted(const vector<Information> &list);
    bool removeDeleted(const Information &info);
    void clearDeleted();
};

vector<Register>& operator-(vector<Register>&registros, const vector<Table>& tabelas);
vector<Register>& operator-(vector<Register>&registros, const Table& tabela);
vector<Register>& operator+(vector<Register>&registros, const vector<Table>& tabelas);
vector<Register>& operator+(vector<Register>&registros, const Table& tabela);

bool existeOcorrenciaDeleted(const Table tabela, const Information &novo, vector<Register> &registros);
bool existeOcorrenciaSearch(const Table tabela, const Information &novo, vector<Register> &registros);
void concatenaInformacoesSearch(const Register &registro_novo, vector<Register> &lista_resultados);
void concatenaInformacoesDeleted(const Register &registro_novo, vector<Register> &lista_resultados);

#endif // REGISTER_H
