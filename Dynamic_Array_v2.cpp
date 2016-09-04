#include <iostream>
#include <fstream>
#include <string>
// Warlon Zeng


// HOW TO USE: MAKE SOMETHING LIKE A TXT FILE "array.txt" IN SAME DIRECTORY AND WRITE INTEGERS WITH END LINE. EX:
// 1
// 2
// 3
// 2
// 3
// 5


using namespace std;


const int CAPACITY = 50;


bool isInVector(int Arr2[], int searchInt, int size = CAPACITY){ // size = 50 is default variable if ignored
	int i = 0;
	while (i < size){
		if (Arr2[i] == searchInt){
			return true;
		}
		i++;
	}
}


int makeUnique(int Arr[], int Arr2[], int size = CAPACITY){
	int z = 0; // used to take a loop-local variable out of loop
	for (int j = 0, k = 0; j < size; j++){
		if (isInVector(Arr2, Arr[j], size) != true){
			Arr2[k] = Arr[j];
			k++;
		}
		z = k;
	}
	size = z;
	return size;
}


int fillVectFromFile(ifstream& inFile, string prompt, int Arr[], int size){
	cout << prompt;
	string filename;
	cin >> filename;
	inFile.open(filename);
	while (!inFile){
		cout << "Bad filename" << endl;
		cout << prompt;
		cin >> filename;
		inFile.clear();
		inFile.open(filename);
	}
	int ints;
	while (inFile >> ints){
		Arr[size] = ints;
		size++;
	}
	return size; // creates approiate sized array instead of returning max elements
}


void createUniqInts(int Arr[], int Arr2[], int size = CAPACITY){
	size = makeUnique(Arr, Arr2, size);
	ofstream outFile;
	outFile.open("uniq.ints");
	for (int i = 0; i < size; i++){
		outFile << Arr2[i] << endl;
	}
}


int main(){
	ifstream ArrFile;
	int Arr[CAPACITY];
	int Arr2[CAPACITY];
	int size = 0;
	size = fillVectFromFile(ArrFile, "Enter array of integers file: ", Arr, size); // successfully reads the inFile
	createUniqInts(Arr, Arr2, size);
}