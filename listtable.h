#ifndef LISTTABLE_H
#define LISTTABLE_H
#include <vector>
#include "table.h"

class ListTable{

private:
    vector<Table> tabelas;
    bool existTable(string nameTable) const;

public:
    ListTable();

    bool getTable(string name, Table &tabela) const;
    bool addTable(string nameTable);
    bool removeTable(string name);

    bool addField(string nameTable, string nameField, string type);
    bool removeField(string nameTable, string nameField);


};

#endif // LISTTABLE_H
