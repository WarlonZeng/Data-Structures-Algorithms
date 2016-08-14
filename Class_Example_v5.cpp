#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;


class Car{
	string make;
	string model;
	string licensePlate;
	int efficiencyRating;
	int* gasTank;
	int odometer;
	int fullTank;
	int milesDriven;


public:
	Car(){ fullTank = 15; }
	Car(){ milesDriven = 0; }
	void setMakeName(string newName = "unspecified"){ make = newName; }
	string getNameMake()const { return make; }//cannot be changed

	void setModelName(string newName = "unspecified"){ model = newName; }
	string getModelName()const { return model; }// cannot be changed

	void setLicensePlateName(string newName){ licensePlate = newName; }
	string getLicensePlateName(){ return licensePlate; }

	void setEfficiencyRating(int rating = 25){ efficiencyRating = rating; }
	int getEfficiencyRating()const { return efficiencyRating; }

	void fillGasTankAndGetInfo(int* gasTank);

	int drive(int* gasTank, int odometer = 0){ return milesDriven; }
};

void Car::fillGasTankAndGetInfo(int* gasTank){
	int& gasTank = fullTank;
	getNameMake();
	getModelName();
	getLicensePlateName();
	getEfficiencyRating();
}

int Car::drive(int* gasTank, int odometer = 0){
	int gasWasted;
	while (*gasTank > 0){
		odometer += rand() % 25;
		efficiencyRating = getEfficiencyRating();
		gasWasted = odometer / efficiencyRating;
		milesDriven = odometer;
		*gasTank -= gasWasted;
	}
	return milesDriven;
}