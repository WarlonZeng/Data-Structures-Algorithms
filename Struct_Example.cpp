#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Employees
{
	int employeeID;
	float employeeWage;
	string employeeName;
	int employeeHours;
	float employeePay;
};

int main()
{
	Employees employee[50];
	ifstream infile;
	infile.open("filename.txt");
	int ID;
	float wage;
	string name;
	while (infile >> ID)
	{
		employee[ID].employeeID = ID;
		infile >> wage;
		employee[ID].employeeWage = wage;
		getline(infile, name);
		employee[ID].employeeName = name;
	};

	// finished getting ID, wage, name, pay OK

	int hours;
	ifstream infile2;
	infile2.open("filename2.txt");
	while (infile2 >> ID)
	{
		infile2 >> hours;
		employee[ID].employeeHours = hours;
	};

	// finished getting hours

	for (int ID = 17; ID < 19; ID++)
	{
		cout << employee[ID].employeeName << "\t" << employee[ID].employeeWage * employee[ID].employeeHours << endl;
	}
	cin.get();
}