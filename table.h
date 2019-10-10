#ifndef TABLE_H
#define TABLE_H
#include <vector>
#include <iterator>
#include <algorithm>
#include "field.h"

#include "string"
using namespace std;

class Table {
private:
    vector<Field> campos;
    string name_table;
public:
    Table(const string nameTable = "");
    string getNameTable() const;
    vector<Field> getField() const;
    bool getField(string nameField, Field &field) const;
    bool getField(int position, Field &field) const;

    void setNameTable(string newName);

    bool addField(Field field);
    bool addField(string nameField, string typeField);

    bool removeField(int position);
    bool removeField(string name);

    bool setNameField(string name, string newName);
    bool setNameField(int position, string newName);

    bool existField(string nameField) const;
    string& operator+(string& txt);


};
string operator+(const string& txt, const Table& table);
ostream& operator<<(ostream& os, const Table& table);

bool operator==(const string& txt, const Table& table);
bool operator==(const Table& table, const string& txt);

#endif // TABLE_H
