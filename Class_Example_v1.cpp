#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;

class Vehicle{
	int* VIN = new int;;
	double* mileage = new double;
public:
	virtual void display(){ cout << endl << "This is a vehicle"; }
	virtual void setVIN(int ID){ *VIN = ID; }
	virtual void setMileage(double newMileage){ *mileage = newMileage; }
	virtual int getVIN(){ return *VIN; }
	virtual double getMileage(){ return *mileage; }
	virtual double calcGasUsed(int milesDriven){
		double gasMileage = getMileage();
		return (milesDriven / gasMileage);
	}
	virtual void drive(){
		cout << calcGasUsed(rand()) << endl;
	};
};

class SUV :public Vehicle{
	bool* fourWDStatus = new bool;
	double* fourWDGasMileage = new double;
public:
	virtual void SUV::display(){
		Vehicle::display();
		if (getfourWDStatus())
			cout << ", it is also four wheeled.";
		else
			cout << ", it is not four wheeled.";
	}
	virtual void setfourWDStatus(bool state){ *fourWDStatus = state; }
	virtual void setfourWDGasMileage(double fourWDMileage){ *fourWDGasMileage = fourWDMileage; }
	virtual bool getfourWDStatus(){ return *fourWDStatus; }
	virtual double getfourWDGasMileage(){ return *fourWDGasMileage; }
	virtual double calcGasUsed(int milesDriven){
		if (getfourWDStatus()){
			double fourGasMileage = getfourWDGasMileage();
			return (milesDriven / fourGasMileage);
		}
		else{
			double gasMileage = getMileage();
			return (milesDriven / gasMileage);
		}
	}
};

int main(){
	Vehicle** theArray = new Vehicle*[3]; // make array of pointers
	Vehicle* ptrToV1;
	Vehicle V1;
	ptrToV1 = &V1;
	ptrToV1->setMileage(2); // set mileage to 2.
	SUV* ptrToSUV1;
	SUV SUV1;
	ptrToSUV1 = &SUV1;
	ptrToSUV1->setMileage(2); // set mileage to 2.
	ptrToSUV1->setfourWDGasMileage(4); 
	ptrToSUV1->setfourWDStatus(false); // a SUV is a vehicle; this however is NOT four wheeled. if this is replaced with true, gas burned is halved because of virtual late initialization.
	SUV* ptrToSUV2;
	SUV SUV2;
	ptrToSUV2 = &SUV2;
	ptrToSUV1->setMileage(2);
	ptrToSUV2->setfourWDGasMileage(4); // set four wheeled mileage to 4.
	ptrToSUV2->setfourWDStatus(true); // a SUV is a vehicle; this however IS four wheeled.
	theArray[0] = ptrToV1;
	theArray[1] = ptrToSUV1;
	theArray[2] = ptrToSUV2;
	for (int i = 0; i < 3; i++)
		theArray[i]->drive(); // rand() is miles driven and mileage is gas rating
	cin.get(); // one way to pause screen, OS-independent.
	delete []theArray;
}