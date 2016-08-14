#include <iostream>

using namespace std;

class BigInt {
	int val;
public:
	BigInt() { val = 0; }
	BigInt(int newVal) { val = newVal; }
	int getVal()const { return val; }
	void setVal(int newVal) { val = newVal; }
	BigInt operator+(const BigInt& rhs)const;
	BigInt& operator+=(const BigInt& rhs);
	bool operator<(const BigInt& rhs) const;
	bool operator>(const BigInt& rhs) const { return rhs < *this; }
	bool operator<=(const BigInt& rhs)const { return !(rhs < *this); }
	bool operator>=(const BigInt& rhs) const { return !(*this < rhs); }
	bool operator!=(const BigInt& rhs) const { return (*this < rhs) || (rhs < *this); }
	bool operator==(const BigInt& rhs) const { return !((*this < rhs) || (rhs < *this)); }

	int operator*() const { return val; } // Val cannot be changed if object is const
	int& operator*() { return val; }

	int operator[](int index) const { return val + index; }
	int& operator[](int index) { return val; }

	friend ostream& operator<<(ostream& outs, const BigInt& rhs);
	friend istream& operator >> (istream& ins, BigInt& rhs);

	friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);

};

ostream& operator<<(ostream& outs, const BigInt& rhs) {
	outs << rhs.val;
	return outs;
}
istream& operator >> (istream& ins, BigInt& rhs) {
	ins >> rhs.val;
	return ins;
}


bool BigInt::operator<(const BigInt& rhs) const {
	return val < rhs.val;
}
BigInt& BigInt::operator+=(const BigInt& rhs) {
	val += rhs.val;
	return *this;
}

BigInt BigInt::operator+(const BigInt& rhs) const {
	int temp = val + rhs.val;
	return temp;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
	//	return rhs.getVal() - rhs.getVal(); // works always
	return lhs.val - rhs.val; // works only if this function is a friend
}