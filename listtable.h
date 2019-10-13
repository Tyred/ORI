#ifndef LISTTABLE_H
#define LISTTABLE_H
#include <vector>
#include "table.h"

class ListTable{

private:
    vector<Table> tabelas;
public:
    ListTable();
    bool existTable(string nameTable) const;
    bool getTable(string name, Table &tabela) const;
    vector<Table> getTable() const;
    bool addTable(Table table);
    bool addTable(string nameTable);
    bool removeTable(string name);
    bool removeTable(const Table &tabela);

    bool addField(string nameTable, string nameField, string type);
    bool removeField(string nameTable, string nameField);


};

#endif // LISTTABLE_H
