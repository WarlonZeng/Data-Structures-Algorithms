#include <iostream>
#include <vector>
#include <functional>
#include <string>

using namespace std;

template<class HashedObj>
class HashTable {
public:
	explicit HashTable(int size = 101) :currentSize(0) { array.resize(size); }
	hash<HashedObj> hf; // This is a hash functor. Assume that there already exists a default that is used.
	bool find(const HashedObj& x) const;
	void makeEmpty();
	bool insert(const HashedObj& x);
	bool remove(const HashedObj& x);
	enum EntryType { ACTIVE, EMPTY, DELETED };
private:
	struct HashEntry {
		HashedObj element;
		EntryType info;
		HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY)
			: element(e), info(i) {}
	};
	vector<HashEntry> array;
	int currentSize;
	void rehash();
};

/////////////////////////////////////////////////////////////////////////
// You needed to write these three functions: find, insert, and rehash
// Remember: This is done via linear probing, not separate chaining
/////////////////////////////////////////////////////////////////////////

template <class HashedObj>
bool HashTable<HashedObj>::find(const HashedObj& x) const {
	// First, get the appropriate hash so we can look where we expect the element to be
	int i = hf(x) % array.size();

	// We need to keep looking until we find the element, or get to an EMPTY spot
	while (true) {

		// If the spot is EMPTY, then the element is not in the table
		if (array[i].info == EMPTY)
			return false;

		// If the spot is DELETED, we move on, since it may have been placed after
		// this element before it was deleted
		else if (array[i].info == DELETED)
			i = (i + 1) % array.size();

		// If the spot is occupied, check if it's the element we're looking for
		else {
			// If it is the element we're looking for, return true
			if (array[i].element == x)
				return true;

			// Otherwise, we need to keep looking
			i = (i + 1) % array.size();
		};
	};
};

template <class HashedObj>
bool HashTable<HashedObj>::insert(const HashedObj& x) {
	// Get the appropriate hash so we can insert where we expect the element to be
	int i = hf(x) % array.size();

	// Check first if the element is already in this table, so we don't insert duplicates
	if (find(x))
		return false;

	// If the spot we've hashed to is occupied, we need to move on to the next index
	while (array[i].info == ACTIVE)
		i = (i + 1) % array.size();

	// If we've made it here, it means that we've reached a spot where we can insert
	// Set the element here to be 'x' and set the spot to ACTIVE
	array[i].element = x;
	array[i].info = ACTIVE;

	// Make sure that the load balance is respected -- if not, we need to rehash
	if (++currentSize >= array.size() * 0.5)
		rehash();

	return true;
};

template<class HashedObj>
void HashTable<HashedObj>::rehash() {
	// Keep track of the old array before we rehash
	vector<HashEntry> oldarray = array;

	// Clean up the current array and resize it
	array.clear();
	currentSize = 0;
	array.resize(oldarray.size() * 2 + 1);

	// Now re-insert all of the elements to our resized array
	for (int i = 0; i < oldarray.size(); i++)
		if (oldarray[i].info == ACTIVE)
			insert(oldarray[i].element);
};

/////////////////////////////////////////////////////////////////////////
// Just to test that the HashTable works
/////////////////////////////////////////////////////////////////////////

string boolToString(bool val) { return val ? "True" : "False"; }

int main() {

	// Initialize the hash table
	HashTable<string> wordTable;

	// Insert 'something' -- should return true since nothing is in the table yet
	cout << "Insert 'something' Successful? " << boolToString(wordTable.insert("something")) << endl;

	// Check if 'some' is in the table -- should return false since 'some' is not in the table
	cout << "Find   'some'      Successful? " << boolToString(wordTable.find("some")) << endl;

	// Check if 'something' is in the table -- should return true
	cout << "Find   'something' Successful? " << boolToString(wordTable.find("something")) << endl;

	// Insert 'something' -- shoudl return false since 'something' is already in the table
	cout << "Insert 'something' Successful? " << boolToString(wordTable.insert("something")) << endl;

	// Insert many strings to force the table to rehash
	for (int i = 0; i < 100; i++) {
		// This converts the integer to a string, so we can insert the number as a string to the table
		string temp = to_string(i);
		wordTable.insert(temp);
	}

	// Elements can still be found correctly
	cout << "Find   'a'         Successful? " << boolToString(wordTable.find("a")) << endl;
	cout << "Find   '99'        Successful? " << boolToString(wordTable.find("99")) << endl;
	cout << "Find   'something' Successful? " << boolToString(wordTable.find("something")) << endl;
}