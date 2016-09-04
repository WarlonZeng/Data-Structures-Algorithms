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
class dynArray{
	int capacity = 50; // capacity is stuck at 50
	T* arr = new T[capacity];
	int size;
	int* ptrToSize = &size;
	//int capacity = 50;

public:
	//dynArray(){ *capacity = 1; *size = 0; } // initialize	//dynArray(){ *size = 0; } // initialize
	dynArray(){ *ptrToSize = 0; }
	//dynArray(){ arr = new int[capacity]; } // default constructor
	//dynArray(const dynArray& rhs); // copy constructor
	~dynArray(); // deconstructor
	void push(T& item); //inserts a new item on the back of the array. 
	T pop(); //removes an item from the back of the array and returns it to the caller.
	T& top(){ return arr[(*ptrToSize - 1)]; }; //returns the last item on the array but does not remove it.
	void clear();  //empties the entire array. 
	bool isEmpty() const; //returns true where there is nothing left on the stack
	void print();
	//int getSize(){ return size; }
};

//template <class T>
//dynArray<T>::dynArray(const dynArray& rhs){ // copy constructor body
//	arr = NULL;
//	*this = rhs;
//}

template <class T>
dynArray<T>::~dynArray(){ // deconstructor
	delete[] arr;
}

template <class T>
void dynArray<T>::push(T& item){
	//size = getSize();
	arr[*ptrToSize] = item; // size and capacity changes dynamically
	(*ptrToSize)++;
	//if (&capacity < *size)
	//	&capacity++; // size cannot be smaller than capacity; size is size of array, capacity is the limits of the array which can be changed if necessary
}

template <class T>
T dynArray<T>::pop(){
	int lastItem = arr[*ptrToSize];
	arr[*ptrToSize] = NULL;
	(*ptrToSize)--;
	return lastItem;
}

template <class T>
void dynArray<T>::clear(){
	arr = NULL;
	*ptrToSize = 0;
}

template <class T>
bool dynArray<T>::isEmpty() const{ // probably not a good idea to deference arr after setting it to NULL (blank)
	return (*ptrToSize == 0);
}

template <class T>
void dynArray<T>::print(){
	//size = getSize();
	for (int i = 0; i < *ptrToSize; i++)
		cout << arr[i] << ", ";
}

int main(){
	//srand(time(NULL));
	//dynArray<int> dai5;
	//for (int i = 0; i < 5; i++)
	//	dai5.push(rand());
	//dai5.print();
	//cin.get();
	string a = "a";
	dynArray<string> das5;
	for (int j = 0; j < 5; j++)
		das5.push(a);
	das5.print();
	cin.get();
}