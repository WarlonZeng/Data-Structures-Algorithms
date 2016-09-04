#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

template <class T>
void myswap(T& lhs, T& rhs) {
	T temp = lhs;
	lhs = rhs;
	rhs = temp;
}

template <class T>
void printArray(T* arr, int size) {
	for (int i = 0; i < size; i++)
		cout << "Array[" << i << "] = " << arr[i] << endl;
}

template <class T>
class Stack {
	vector<T> data;
public:
	void push(const T& newdata);
	T pop();
	bool isEmpty() const;

	T& top() { return data[data.size() - 1]; }
	void clear() { data.clear(); }
	void print();
};

template <class T>
void Stack<T>::print() {
	for (int i = 0; i < data.size(); i++)
		cout << data[i] << ", ";

}
template <class T>
void Stack<T>::push(const T& newdata) {
	data.push_back(newdata);
}
template <class T>
T Stack<T>::pop() {
	T temp = top();
	data.pop_back();
	return temp;
}

template <class T>
bool Stack<T>::isEmpty() const {
	return data.size() == 0;
}

int main() {
	srand(time(NULL));


	Stack<int> s1;
	for (int i = 0; i < 100; i++)
		s1.push(rand());

	cout << "S1= ";
	s1.print();
	cout << endl << endl;
	Stack<double> s2;
	for (int i = 0; i < 100; i++)
		s2.push(rand() / 100.);
	cout << "S2= ";
	s2.print();
	cout << endl << endl;
}