#ifndef REGISTERDELETED_H
#define REGISTERDELETED_H

#include "table.h"
#include "information.h"


class RegisterDeleted{
    private:
        Table tabela;
        vector<Information> listaRemovidos;
public:
    RegisterDeleted(Table tabela);
    RegisterDeleted();

    bool exist(const Information &info) const;
    Table getTable() const;

    bool removeInfo(const Information &info);
    void setTable(Table novo);
    bool addInfo(const Information &info);

};

#endif // REGISTERDELETED_H
