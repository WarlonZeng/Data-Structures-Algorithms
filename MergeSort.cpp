// CS2134 Homework 4
// Programming Question 1

#include <iostream>
#include <vector>
#include <algorithm>
// #include <utility>
using namespace std;

// This is the recursive function that was to be written
template<class RandItr>
void mergeSort(RandItr tmp, RandItr start, RandItr end) {

	// Check to see if we're at the base case (one element)
	if (end - start == 1) return;

	// Find the middle of the current section
	RandItr mid = start + (end - start) / 2;
	int sz = end - start;

	// Divide and conquer call for both halves
	mergeSort(tmp, start, mid);
	mergeSort(tmp + sz / 2, mid, end);

	// Then merge the sorted sides
	merge(start, mid, mid, end, tmp);

	// Important: You must copy the sorted result (tmp) into the original vector
	copy(tmp, tmp + sz, start);
	/* Or, a loop could've been used to copy the vectors:
	RandItr cur = start;
	while(cur != end)
	{
	*cur = move(*tmp);
	tmp++;
	cur++;
	}
	*/
}

// This is the driver provided in the question
template <class RandItr>
void mergeSort(RandItr start, RandItr end) {
	int sz = end - start;
	typedef typename iterator_traits<RandItr>::value_type Object;
	vector<Object> tmp(sz);
	mergeSort(tmp.begin(), start, end);
}

// Some dummy code to test the function
int main() {
	vector<int> test{ 3,6,5,89,-10 };
	cout << "Unsorted: ";
	for (int a : test) cout << a << " ";
	cout << endl << "Sorted:   ";
	mergeSort(test.begin(), test.end());
	for (int a : test) cout << a << " ";
	cout << endl;
}