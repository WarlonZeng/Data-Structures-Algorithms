#include <iostream>     // std::cout
#include <functional>   // std::minus
#include <numeric>      // std::accumulate
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>
#include <algorithm>

// Warlon Zeng

using namespace std;

void countFromTxtFile(){
	ifstream listTxtFile;
	cout << "Enter filename. DO NOT include the .txt extension: " << endl;
	string fileName;
	cin >> fileName;
	listTxtFile.open(fileName + ".txt");
	while (!listTxtFile){
		cout << "Bad filename: " << endl;
		cin >> fileName;
		listTxtFile.clear();
		listTxtFile.open(fileName);
	}
	string lines;
	int count = 0;
	while (getline(listTxtFile, lines)){ // counts number of lines; process per line.
		count++;
	}
	//cout << count << endl; // creates approiate sized array instead of returning max elements
	ofstream numberTxtFile;
	numberTxtFile.open(fileName + "_Number_of_Lines.txt");
	numberTxtFile << count << endl;
	numberTxtFile.close();
}


int main(){
	countFromTxtFile();
}