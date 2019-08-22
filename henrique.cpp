#include <iostream>
#include <string>
#include <istream>

#include "interpreter.h"

int main(){
	using namespace std;

	string input;
	interpreter a;

	while(input != "EB"){
		cout << "Digite:" << endl;
		getline(std::cin, input);

		a.parse(input);
	}

}