#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Employee{
	int id;
	string name;
	double payRate;
	double hoursWorked;
};

void openInputFile(ifstream& inFile, string prompt){
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
}

double calcPay(Employee e){
	return e.payRate*e.hoursWorked;

}

void sortEmployees(Employee Arr[], int size){
	for (int i = 0; i < size; i++){
		int indexOfMin = i;
		for (int j = i + 1; j < size; j++){
			if (calcPay(Arr[j]) < calcPay(Arr[indexOfMin]))
				indexOfMin = j;
		}
		Employee temp = Arr[i];
		Arr[i] = Arr[indexOfMin];
		Arr[indexOfMin] = temp;
	}
}

int main(){
	const int CAPACITY = 50;
	Employee Arr[CAPACITY];
	int size = 0;
	ifstream empFile;
	ifstream hoursFile;
	openInputFile(empFile, "Please enter the name of the employee file: ");
	openInputFile(hoursFile, "Please enter the name of the hours worked file: ");
	Employee temp;
	while (empFile >> temp.id >> temp.payRate){
		getline(empFile, temp.name);
		temp.hoursWorked = 0;
		Arr[size++] = temp;
	}

	int id;
	double hours;
	while (hoursFile >> id >> hours){
		for (int i = 0; i < size; i++)
			if (Arr[i].id == id){
				Arr[i].hoursWorked += hours;
				i = size;
			}
	}
	sortEmployees(Arr, size);
	for (int i = 0; i < size; i++)
		cout << Arr[i].name << "\t" << calcPay(Arr[i]) << endl;
}