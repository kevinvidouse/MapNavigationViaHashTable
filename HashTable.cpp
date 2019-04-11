#include<iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

struct place
{
	string state;
	string place_name;
	int population;
	double area;
	double latitude;
	double longitude;
	int rep_intersection;
	double dist_to_intersection;
	place(string s, string pl, int po, double a, double lat, double lon, int rep, double dist)
	{
		state = s;
		place_name = pl;
		population = po;
		area = a;
		latitude = lat;
		longitude = lon;
		rep_intersection = rep;
		dist_to_intersection = dist;


	}

	void print() 
	{
		cout << place_name << ", " << state << ". Pop:" << population << ", Area: " << area << endl;
		cout << "lat: " << latitude << ", long:" << longitude << ", rep_int:" << rep_intersection 
			<< ", dist:" << dist_to_intersection << endl;
	}
};

class HashNode
{
public:
	place * hashvalue;
	HashNode * next;

	HashNode(place * p)
	{
		hashvalue = p;
		next = NULL;
	}
	
};


const int HASHTABLE_SIZE = 100;

class HashTable
{
private:
	HashNode ** arr;
public:
	vector<place*> foundPlaces;

	HashTable()
	{
		arr = new  HashNode*[HASHTABLE_SIZE];
		for (int i = 0; i < HASHTABLE_SIZE; i++)
		{
			arr[i] = NULL;
		}
	}

	unsigned hash(string key)
	{
		unsigned int result = 0;
		for (int i = 0;i < key.length();i++)
		{ 
			result = result * 101 + key[i];
		}
		return result % HASHTABLE_SIZE;
	}

	void add(place * p)
	{
		int index = hash(p->place_name);
		HashNode * prev = NULL;
		HashNode * node = arr[index];
		while (node != NULL) {
			prev = node;
			node = node->next;
		}
		if (node == NULL) {
			node = new HashNode(p);
			if (prev == NULL) {
				arr[index] = node;
			}
			else {
				prev->next = node;
			}
		}
		else {
			node->hashvalue = p;
		}
	}
	
	vector<place*> get(string key) {
		int index = hash(key);
		HashNode * node = arr[index];
		vector<place*> matchVec;

		while (node != NULL) {
			if (node->hashvalue->place_name == key) {
				matchVec.push_back(node->hashvalue);
			}
			node = node->next;
		}
		return matchVec;
	}

	string cleanEndSpaces(string s) {

		for (int i = s.length() - 1; i >= 0; i--) {
			if (s[i] != ' ')
				return s.substr(0, i+1);
		}
		return "<EMPTY>";

	}


	void printListOfStates(vector<place*> vec) {
		for (int i = 0; i < vec.size(); i++) {
			cout<<vec[i]->state<<" ";
		}
		cout<<endl;
	}

	void printAllFound(string s) {
		cout << "printing occurences of '" << s << "'\n";
		foundPlaces = get(s);
		if (foundPlaces.size() == 0 ){
			cout<<"No cities found with that name\n";
			return;
		}
		printListOfStates(foundPlaces);
	}
	void printFromState(string st){
		for (int i = 0; i < foundPlaces.size(); i++){
			if (foundPlaces[i]->state == st){
				foundPlaces[i]->print();
				return;
			}
		}
		cout<<"Place not found for that state\n";
	}
	void readFile() {
		//cout << "about to read" << endl;
		ifstream fin("/home/www/class/een318/named-places.txt");

		string state;
		string place_name;
		int population;
		double area;
		double latitude;
		double longitude;
		int rep_intersection;
		double dist_to_intersection;
		cout<<"reading file\n";
		for (string line; getline(fin, line);) {
			state = line.substr(8, 2);
			place_name = cleanEndSpaces(line.substr(10, 49));
			population = atoi(line.substr(59, 7).c_str());
			area = atof(line.substr(66, 14).c_str());
			latitude = atof(line.substr(80, 10).c_str());
			longitude = atof(line.substr(90, 11).c_str());
			rep_intersection = atoi(line.substr(101, 5).c_str());
			dist_to_intersection = atof(line.substr(106, 8).c_str());

			add(new place(state, place_name, population, area,
				latitude, longitude, rep_intersection, dist_to_intersection));
		}
		cout<<"done reading file\n";
	}


};
