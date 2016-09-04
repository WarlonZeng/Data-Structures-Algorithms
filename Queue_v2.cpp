#include <iostream>
#include <string>

using namespace std;

template <typename Object>
class Queue {
private:
	struct Node {
		Object data;
		Node *next;
		Node(const Object& d = Object{}, Node* n = nullptr) : data{ d }, next{ n } { }
		Node(Object&& d, Node* n = nullptr) : data{ std::move(d) }, next{ n } { }
	};
	Node* first;
public:
	Queue() : first(nullptr) {}
	
	Object& front() { return first->data; }

	Object& back() {
		Node* temp = first;
		while (temp->next) {
			temp = temp->next;
		}
		return temp->data;
	}

	bool empty() const { return !first; }

	void dequeue() {
		if (first) {
			Node* temp = first;
			first = first->next;
			delete temp;
		}
	}

	void enqueue(const Object& elem) {
		if (!first) {
			first = new Node(elem);
			return;
		}
		Node* temp = first;
		while (temp->next) { temp = temp->next; }
		temp->next = new Node(elem);
	}
};

// I added this method just to make the printouts in main nicer
string boolToString(bool flag) { return flag ? "yes" : "no"; }

int main() {
	Queue<int> someQueue;
	cout << "\nCreated a queue!" << endl;
	cout << "Empty Queue: " << boolToString(someQueue.empty()) << endl;

	someQueue.enqueue(50);
	cout << "\nAdded 50" << endl;
	cout << "Front: " << someQueue.front() << endl;
	cout << "Back: " << someQueue.back() << endl;

	someQueue.enqueue(75);
	cout << "\nAdded 75" << endl;
	cout << "Front: " << someQueue.front() << endl;
	cout << "Back: " << someQueue.back() << endl;

	someQueue.enqueue(100);
	cout << "\nAdded 100" << endl;
	cout << "Front: " << someQueue.front() << endl;
	cout << "Back: " << someQueue.back() << endl;

	someQueue.dequeue();
	cout << "\nDequeued!" << endl;
	cout << "Front: " << someQueue.front() << endl;
	cout << "Back: " << someQueue.back() << endl;
	cout << "Empty Queue: " << boolToString(someQueue.empty()) << endl;

	someQueue.dequeue();
	someQueue.dequeue();
	cout << "\nDequeued 75 and 100" << endl;
	cout << "Empty Queue: " << boolToString(someQueue.empty()) << endl << endl;
}