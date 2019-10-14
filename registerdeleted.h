#ifndef REGISTERDELETED_H
#define REGISTERDELETED_H

#include "table.h"
#include "information.h"
#include "registersearch.h"

class RegisterDeleted{
    private:
        Table tabela;
        vector<Information> listaRemovidos;
public:
    RegisterDeleted(Table tabela);
    RegisterDeleted();

    bool exist(const Information &info) const;
    bool exist(const vector<Information> &info) const;
    Table getTable() const;

    bool removeInfo(const Information &info);
    void setTable(Table novo);
    bool addInfo(const Information &info);
};

vector<RegisterDeleted>& operator-(vector<RegisterDeleted>&registros, const vector<Table>& tabelas);
vector<RegisterDeleted>& operator-(vector<RegisterDeleted>&registros, const Table& tabela);

vector<RegisterDeleted>& operator+(vector<RegisterDeleted>&registros, const vector<Table>& tabelas);
vector<RegisterDeleted>& operator+(vector<RegisterDeleted>&registros, const Table& tabela);

bool existeOcorrencia(RegisterSearch &procurados, vector<RegisterDeleted>&deletados);

#endif // REGISTERDELETED_H
