#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;

// task: function takes an POINTER TO INT DYNAMIC ARRAY
// sort while copy


//const int CAPACITY = 1;
//int* ptrToDynamicArray = new int[CAPACITY];



void sortCopy(int* ptrToDynamicArray[], int* newDynamicArray[], int size){ // (ptr to dynamic array, size) ex. 5, 4, 3, 2, 1 size: 5
	int i = 0;
	int* newDynamicArray = new int[size];
	int lowest = INT_MAX;
	int nextLowest = INT_MAX;
	while (i < size){ // copy
		for (int j = 0; j < size; j++){ // sort before every copy
			if (*ptrToDynamicArray[j] < lowest)
				lowest = *ptrToDynamicArray[j];
			else if ((*ptrToDynamicArray[j] > lowest) && (*ptrToDynamicArray[j] < nextLowest)) // [1 4 10 5 3]; lowest = 1; nextLowest = 3
				nextLowest = *ptrToDynamicArray[j];
		}
		*newDynamicArray[i] = lowest; //copyToSortedArray(&newDynamicArray[size], i, lowest);
		lowest = nextLowest;
		i++;
	}
	*ptrToDynamicArray = *newDynamicArray;
	delete *newDynamicArray;
}