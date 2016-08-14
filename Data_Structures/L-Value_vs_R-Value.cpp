#include <iostream>
using namespace std;

template <class Something>
void testMe(Something& val) {
	cout << "This is an lval!\n";
}

template <class Something>
void testMe(const Something&& val) {
	cout << "This is an rval!\n";
}

int& dummyRef(int& thing) {
	return thing;
}

int dummy(int& thing) {
	return thing;
}

string dummyString() {
	return "test";
}

int main() {
	// Which 'testMe' do you expect to be called?
	// Make a guess before you run the code!

	cout << "For ints:\n";
	int blah = 5;
	testMe(blah);
	testMe(5);

	cout << "\nFor chars:\n";
	char thing = 'a';
	testMe(thing);
	testMe('a');

	cout << "\nFor addresses/pointers:\n";
	int* point = &blah;
	testMe(point);
	testMe(&blah);

	cout << "\nFor return type:\n";
	testMe(dummyRef(blah));
	testMe(dummy(blah));

	cout << "\nFor strings:\n";
	string test = "hello";
	testMe(test);
	// These are tricky, for reasons that aren't immediately obvious:
	testMe("hello");
	testMe((string)"hello");
	testMe(test + "hello");
	testMe(dummyString());
	
	cin.get();
}