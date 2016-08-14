#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <list>
#include <algorithm>

using namespace std;

// Timer Class, from HW1

class timer {
public:
	timer() : start(clock()) {}
	double elapsed() { return (clock() - start) / CLOCKS_PER_SEC; }
	void reset() { start = clock(); }
private:
	double start;
};

// PQ3 Code

template <class Collection>
void fillCollection(Collection& coll) {
	ifstream file("ENABLE.txt");
	if (!file) {
		cout << "Your file is incorrect!\n";
		return;
	}
	string line;
	while (getline(file, line)) {
		coll.insert(coll.end(), line);
	}
}

int main() {

	// PQ3: Fill up the collections
	vector<string> vec;
	set<string> oset;
	unordered_set<string> uset;
	list<string> llist;

	fillCollection(vec);
	fillCollection(oset);
	fillCollection(uset);
	fillCollection(llist);

	// PQ4: Timing
	timer t;
	double numClicks;
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(6);

	string luge = "luge";

	t.reset();
	vector<string>::iterator itr = lower_bound(vec.begin(), vec.end(), luge);
	numClicks = t.elapsed();
	cout << "Vector: " << numClicks << endl;
	// cout << *itr << endl; // Check you got the right iterator

	t.reset();
	oset.find(luge);
	numClicks = t.elapsed();
	cout << "Set: " << numClicks << endl;
	// cout << *itr << endl; // Check you got the right iterator

	t.reset();
	uset.find(luge);
	numClicks = t.elapsed();
	cout << "Unordered: " << numClicks << endl;
	// cout << *itr << endl; // Check you got the right iterator

	t.reset();
	lower_bound(llist.begin(), llist.end(), luge);
	numClicks = t.elapsed();
	cout << "List: " << numClicks << endl;
	// cout << *itr << endl; // Check you got the right iterator
}