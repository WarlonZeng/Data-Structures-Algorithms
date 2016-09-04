#include <iostream>     // std::cout
#include <functional>   // std::minus
#include <numeric>      // std::accumulate
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>
#include <algorithm>
#include <set>

// Warlon Zeng

using namespace std;

string fileName;

void print(const string& item)
{
	ofstream sortedTxtFile;
	//std::cout << item << std::endl;
	sortedTxtFile.open(fileName + "_Sorted.txt", ios_base::app);
	sortedTxtFile << item << endl;
}

void countFromTxtFile(){
	ifstream listTxtFile;
	cout << "Enter filename. DO NOT include the .txt extension: " << endl;
	cin >> fileName;
	listTxtFile.open(fileName + ".txt");
	while (!listTxtFile){
		cout << "Bad filename: " << endl;
		cin >> fileName;
		listTxtFile.clear();
		listTxtFile.open(fileName);
	}
	string name;
	set<string> sortedItems;
	while (getline(listTxtFile, name)){ // counts number of lines; process per line.
		sortedItems.insert(name);
	}

	ofstream sortedTxtFile;
	sortedTxtFile.open(fileName + "_Sorted.txt", ios_base::trunc);
	sortedTxtFile.close();

	for_each(sortedItems.begin(), sortedItems.end(), &print);
	//cout << count << endl; // creates approiate sized array instead of returning max elements
	
}

int main()
{
	countFromTxtFile();
	return 0;
}