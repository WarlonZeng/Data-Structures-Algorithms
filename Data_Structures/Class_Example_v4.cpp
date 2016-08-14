#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;

class Measurements{
	int ft;
	int in;
public:
	int getIn()const { return in; }
	int getFt()const { return ft; }
	Measurements(){ in = 0; } // default constructor
	Measurements(){ ft = 0; }
	Measurements(int inches){ in = inches; }; // constructor for integer
	void setIn(int& in, int& ft){
		if (in > 11 || in < 0){
			ft += in / 12; // number of inches go into feet; adjusted for feet accordingly
			in = in % 12; // modulo for remaining inches
		}
	}
	
	Measurements operator+(const Measurements& rhs)const; // addition
	Measurements operator-(const Measurements& rhs)const; // subtraction
	bool operator<(const Measurements& rhs)const; // establish boolean
	bool operator==(const Measurements& rhs)const { return !((*this < rhs) || (rhs < *this)); } // equality
	bool operator!=(const Measurements& rhs)const { return (*this < rhs) || (rhs < *this); } // inequality
	Measurements& operator++(){ // incrementation PRE
		++in;
		return *this;
	}
	Measurements operator++(int){// incrementation POST
		Measurements temp(*this);
		operator++();
		return temp;
	}
	friend ostream& operator<<(ostream& outs, const Measurements& rhs); // output for F'I
};

//Measurements::Measurements(const int& rhs){ // constructor for integer
//	in = NULL;
//	*this = rhs;
//} ignore this

Measurements Measurements::operator+(const Measurements& rhs) const{
	int temp = in + rhs.in;
	return temp;
}

Measurements Measurements::operator-(const Measurements& rhs) const{
	int temp = in - rhs.in;
	return temp;
}

ostream& operator<<(ostream& outs, const Measurements& rhs){
	outs << rhs.ft << "'" << rhs.in << "''" << endl;
	return outs;
}