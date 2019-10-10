#ifndef SEARCH_H
#define SEARCH_H
#include "table.h"
#include "information.h"
#include <vector>

class RegisterSearch {
    private:
        Table tabela;
        vector<Information> index_results;
        void clearSearch();
    public:
        RegisterSearch(Table tabela);
        RegisterSearch();

        bool exist(const Information &info) const;

        Table getTable() const;
        void setTable(Table novo);

        bool addSearch(Information info);
        bool addSearch(const vector<Information> &list);
        bool removeSearch(const Information &info);

};

#endif // SEARCH_H
