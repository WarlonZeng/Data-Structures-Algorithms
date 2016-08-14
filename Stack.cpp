#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;

class MyStack{
	int* arr;
	int* item;
	int* size;
	int* capacity;

public:
	MyStack(){ *capacity = 1; } // initialize
	MyStack(){ size = 0; } // initialize
	MyStack(int capacity){ arr = new int[capacity]; } // default constructor
	MyStack(const MyStack& rhs); // copy constructor
	~MyStack(); // deconstructor
	void push(const int& item ); //inserts a new item on the back of the array. cannot use .push_back because problem states to use dynamic array.
	int pop(); //removes an item from the back of the array and returns it to the caller. cannot use .pop_back because problem states to use dynamic array.
	int& top(){ return arr[(*size - 1)]; }; //returns the last item on the array but does not remove it. cannot use .size() because problem states to use dynamic array.
	void clear();  //empties the entire array. cannot use .clear() because problem states to use dynamic array.
	bool isEmpty() const; //returns true where there is nothing left on the stack
};

MyStack::MyStack(const MyStack& rhs){ // copy constructor body
	arr = NULL;
	*this = rhs;
}

MyStack::~MyStack(){ // deconstructor
	delete[] arr;
	delete item;
	delete size;
	delete capacity;
}

void MyStack::push(const int& item){
	arr[*size++] = item; // size and capacity changes dynamically
	*size++;
	if (*capacity < *size)
		*capacity++; // size cannot be smaller than capacity; size is size of array, capacity is the limits of the array which can be changed if necessary
}

int MyStack::pop(){
	int lastItem = arr[*size];
	arr[*size] = NULL;
	*size--;
	return lastItem;
}

void MyStack::clear(){
	arr = NULL;
	*size = 0;
}

bool MyStack::isEmpty() const{ // probably not a good idea to deference arr after setting it to NULL (blank)
	return *size == 0;
}