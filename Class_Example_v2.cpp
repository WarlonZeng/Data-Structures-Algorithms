#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;


void menu(){
	int i = 0;
	int size = 5;
	int optionNum;
	vector<string> options;
	while (i < size){
		cout << i + 1 << options[i] << endl;
		i++;
	}
	cin >> optionNum;
}

class DeliveryTruck{ // class has both properties and actions
	string name;
	vector<int> thePackages;
	int weight;
	vector<int> ID;

public:
	void option1(); // display name
	void option2(); // makes new name and displays name
	string setName(string newName){ name = newName; }
	string getName(){ return name; }
	vector<int> getPackages(){ return thePackages; }
	vector<int> option3(ifstream& inFile);
	void option4(); // gets weights from file into packages
	bool option5(); // search for package by ID
	vector<int> givePackageID(){ return ID; }
};

void DeliveryTruck::option1(){
	cout << getName() << endl;
	thePackages = getPackages();
	for (int i = 0; i < thePackages.size(); i++)
		cout << thePackages[i] << endl;
}


void DeliveryTruck::option2(){
	string newName;
	getName();
	cout << "New ad name?";
	cin >> newName;
	setName(newName);
	name = getName();
	cout << "New name is " << name << endl;
}

vector<int> DeliveryTruck::option3(ifstream& inFile){
	cout << "Enter Filename" << endl;
	string filename;
	cin >> filename;
	inFile.open(filename);
	while (!inFile){
		cout << "Bad filename" << endl;
		cin >> filename;
		inFile.clear();
		inFile.open(filename);
	}
	int ints;
	int size = 0;
	while (inFile >> ints){
		thePackages[size] = ints;
	}
	return thePackages;
}

void DeliveryTruck::option4(){
	thePackages = getPackages();
	int sum = 0;
	for (int i = 0; i < thePackages.size(); i++)
		sum += thePackages[i];
	cout << sum << endl;
}

vector<int> DeliveryTruck::givePackageID(){
	thePackages = getPackages();
	for (int i = 0; i < thePackages.size(); i++)
		ID[i] = i;
	return ID;
}


bool DeliveryTruck::option5(){
	int packageID;
	cout << "ID #?" << endl;
	cin >> packageID;
	getPackages();
	for (int i = 0; i < thePackages.size(); i++)
		if (packageID == ID[i])
			return true;
}