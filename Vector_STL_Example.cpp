#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>
#include <ctime>

// Warlon Zeng

using namespace std;

template <class T>
class haystack{
	vector<T> hay;
public:
	void push(const T& element);

	//T& operator=(const T& rhs);
	bool operator<(const T& rhs) const;
	bool operator==(const T& rhs) const { return !((*this < rhs) || (rhs < *this)); }
	bool search(const T& needle);
	void print();
};

//template <class T>
//T& haystack::operator=(const haystack& rhs){
//	return this;
//}

template <class T>
void haystack<T>::push(const T& element){
	hay.push_back(element);
}

template <class T>
bool haystack<T>::search(const T& needle){
	int size = hay.size();
	for (int i = 0; i < size; i++)
		if (hay[i] == needle)
			return true;
	return false; // the search is finished
}

template <class T>
void haystack<T>::print(){
	for (int i = 0; i < hay.size(); i++)
		cout << hay[i] << ", ";
}

int main(){
	haystack<int> hay;
	hay.push(99);
	hay.push(98);
	hay.print();
	if (hay.search(97))
		cout << "found needle" << endl;
	cin.get(); // pause screen
}