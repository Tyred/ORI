#ifndef FIELD_H
#define FIELD_H

#include "string"
#include <vector>

using namespace std;

class Field {

private:
    string name;
    string type;
public:
    Field(string name, string type);
    string getName() const;
    string getType() const;
    void setName(string newName);
    string& operator+(string& txt);
    bool operator==(const Field& field)const;
};

string operator+(const string& txt, const Field& field);
ostream& operator<<(ostream& os, const Field& field);
ostream& operator<<(ostream& os, const vector<Field>& field);

#endif // FIELD_H
