#ifndef SEARCH_H
#define SEARCH_H
#include "table.h"
#include "information.h"
#include <vector>

class RegisterSearch {
    private:
        Table tabela;
        vector<Information> index_results;

    public:
        RegisterSearch(Table tabela);
        RegisterSearch();

        bool exist(const Information &info) const;

        Table getTable() const;
        void setTable(Table novo);

        vector<Information> getSearch() const;

        bool addSearch(Information info);
        bool addSearch(const vector<Information> &list);
        bool removeSearch(const Information &info);

        void clearSearch();
};

vector<RegisterSearch>& operator-(vector<RegisterSearch>&registros, const vector<Table>& tabelas);
vector<RegisterSearch>& operator-(vector<RegisterSearch>&registros, const Table& tabela);

vector<RegisterSearch>& operator+(vector<RegisterSearch>&registros, const vector<Table>& tabelas);
vector<RegisterSearch>& operator+(vector<RegisterSearch>&registros, const Table& tabela);

void concatenaInformacoes(const RegisterSearch &registro_novo, vector<RegisterSearch> &lista_resultados);

#endif // SEARCH_H
