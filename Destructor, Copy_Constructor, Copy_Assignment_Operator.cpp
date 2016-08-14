#include <iostream>

using namespace std;

class SomeClass {
	int* ptr;
public:
	SomeClass() { ptr = new int; }
	SomeClass(const SomeClass& rhs);
	SomeClass& operator=(const SomeClass& rhs);
	~SomeClass();
	void setInt(int x) { *ptr = x; }
	int getInt() const { return *ptr; }
};
SomeClass::SomeClass(const SomeClass& rhs) {
	ptr = NULL;
	*this = rhs;
}
SomeClass& SomeClass::operator=(const SomeClass& rhs) {
	if (this == &rhs) // test for self assignment
		return *this;
	delete ptr;
	ptr = new int;
	*ptr = *rhs.ptr;
	return *this;
}

SomeClass::~SomeClass() {
	delete ptr;
}

void myFunc() {
	SomeClass s; //implicit call to constructor
	s.setInt(5);
}//implicit call to destructor

void myFunc2(SomeClass s) {
	s.setInt(5);
}

int main() {
	SomeClass s1;
	for (int i = 0; i < 100; i++)
		myFunc2(s1);
	//	for (int i = 0; i < 100000000; i++)
	//		myFunc();
}