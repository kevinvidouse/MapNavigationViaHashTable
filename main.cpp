#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include "HashTable.cpp"

using namespace std;







int main() {
	cout << "Reading all places:\n";
	string inp;
	HashTable table = HashTable();
	table.readFile();


	cout << "Enter a city name:";
	cin >> inp;
	table.printAllFound(inp);
	cout<<"Enter a state:";
	cin >> inp;
	table.printFromState(inp);

}
