#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;

template <class T>
class LList;

template <class T>
class LListNode{
	LListNode<T>* next;
public:
	T data;
	LListNode(T newdata = T(), LListNode<T>* newNext = NULL) :data(newdata), next(newNext){}// member intialization
	friend class LList < T > ;

};

template <class T>
class LList{
	LListNode<T>* head;
	LListNode<T>* recursiveCopy(LListNode<T>* rhs);
public:
	LList() :head(NULL){}
	~LList(){ clear(); }
	LList(const LList<T>& rhs) :head(NULL){ *this = rhs; }
	bool isEmpty(){ return head == NULL; }
	void push_front(const T&);
	T pop_front();
	void clear(){ while (!isEmpty()) pop_front(); }
	int size()const;
	LListNode<T>* find(T toFind);
	void push_back(const T&);
	T pop_back();
	void push_after(T newElement, LListNode<T>* ptr);
	LList<T>& operator=(const LList<T>& rhs);
	void print()const; // this one works but doesnt work
	void addLList(LList<T>& L1, LList<T>& L2, T temp);
	T LList<T>::find_front(); // returns address
	//void LList<T>::printList(LList<T>& List); // works indirectly
	LListNode<T>* isSubList(LList<T>& L1, LList<T>& L2);
	//friend ostream& operator<<(ostream& outs, const T& rhs);
};
template <class T>
LListNode<T>* LList<T>::recursiveCopy(LListNode<T>* rhs){
	if (rhs == NULL)
		return NULL;
	return new LListNode<T>(rhs->data, recursiveCopy(rhs->next));
}
template <class T>
LList<T>& LList<T>::operator=(const LList<T>& rhs){
	if (this == &rhs)
		return *this;
	clear();

	//Begin EVIL way of doing it
	/*
	LListNode<T>* temp = rhs.head;
	while (temp != NULL)
	push_back(temp->data);
	*/
	//End EVIL way
	head = recursiveCopy(rhs.head);
	return *this;
}
template <class T>
void LList<T>::push_after(T newElement, LListNode<T>* ptr){
	ptr->next = new LListNode<T>(newElement, ptr->next);
}
template <class T>
T LList<T>::pop_back(){
	if (isEmpty())//pop empty????
		return T();
	else if (head->next == NULL)
		return pop_front();
	LListNode<T>* temp = head;
	while (temp->next->next != NULL) //stops on second to last node
		temp = temp->next;
	T retval = temp->next->data;
	LListNode<T>* toDelete = temp->next;
	temp->next = NULL;
	delete toDelete;
	return retval;
}
template <class T>
void LList<T>::push_back(const T& newdata){
	if (isEmpty())
		push_front(newdata);
	else{
		LListNode<T>* temp = head;
		while (temp->next != NULL)
			temp = temp->next;
		push_after(newdata, temp);
	}
}
template <class T>
LListNode<T>* LList<T>::find(T toFind){
	LListNode<T>* temp = head;
	for (; temp != NULL&& temp->data != toFind; temp = temp->next);
	return temp;
}

template <class T>
int LList<T>::size()const{
	int count = 0;
	for (LListNode<T>* temp = head; temp != NULL; temp = temp->next)
		count++;
	return count;

}
template <class T>
T LList<T>::pop_front(){
	if (isEmpty()) // pop on an empty list????
		return T();
	T retval = head->data;
	LListNode<T>* temp = head;
	head = head->next;
	delete temp;
	return retval;
}
template <class T>
void LList<T>::push_front(const T& newdata){
	head = new LListNode<T>(newdata, head);
}

template <class T>
void LList<T>::print()const{
	LListNode<T>* temp = head;
	T retval = temp->data;
	while (temp->next != NULL){ // temp->next != NULL
		cout << retval << " ";
		temp = temp->next;
		retval = temp->data;
		//temp = temp->next;
		//cout << temp->next << endl;
	}
	cout << retval << endl;
}
//template <class T>
//void LList<T>::printList(LList<T>& List){ // indirect way
//	LList<T> temp = List;
//	while (!isEmpty()){
//		cout << pop_front() << " ";
//	}
//	List = temp;
//}
template <class T>
T LList<T>::find_front(){
	if (isEmpty()) // pop on an empty list????
		return T();
	T retval = head->data;
	return retval;
}
template <class T>
void LList<T>::addLList(LList<T>& L1, LList<T>& L2, T temp){ // temp = 7
	cout << "Original List : ";
	L1.print();
	cout << "List to add in : ";
	L2.print();
	LList<T> tempList = L2;
	while (!L2.isEmpty()){
		T temp2 = L2.pop_front();
		push_after(temp2, find(temp)); // push in 6 after finding 7
		temp = temp2;
	}
	cout << "Completed List : ";
	L1.print();
	L2 = tempList;
}
template <class T>
LListNode<T>* LList<T>::isSubList(LList<T>& L1, LList<T>& L2){
	cout << "List to search in : ";
	L1.print();
	cout << "List to search for : ";
	L2.print();
	LListNode<T>* temp = head;
	T retval = temp->data; // get 5
	T tempSearch = L2.pop_front(); // get 6
	LList<T> tempList = L2;
	T tempSearchContinue = tempSearch;
	while (temp->next != NULL || (L2.isEmpty() && (retval == tempSearchContinue))){
		if (retval == tempSearchContinue){ // 6 == 6
			cout << "match" << endl;
			tempSearchContinue = L2.pop_front(); // get 3, 3, size : 2
			cout << "size " << L2.size() << endl;
		}
		else{
			L2 = tempList; // restore 3 2 9
			tempSearchContinue = tempSearch; // after first find fails, restore finder
		}
		temp = temp->next;
		retval = temp->data;
	}
	if (L2.isEmpty()){ // restore list
		cout << "found " << find(tempSearch) << endl;
		L2 = tempList;
		L2.push_front(tempSearch);
		return find(tempSearch);
	}
	return NULL;
}

int main(){
	LList<int> L1;
	LList<int> L2;
	//L1.push_back(5);
	L1.push_back(7);
	L1.push_back(9);
	L1.push_back(1);
	L2.push_back(6);
	L2.push_back(3);
	L2.push_back(2);
	//L1.print();
	//L2.print();
	L1.addLList(L1, L2, 7);
	//end of problem 1
	L2.push_back(9);
	L1.isSubList(L1, L2);
	cin.get();
}