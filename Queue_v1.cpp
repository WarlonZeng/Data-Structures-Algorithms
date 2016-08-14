template <class T>
class Queue {
private:
	class Node {
	public:
		Object  data;
		Node    *next;
		Node(const Object& d = Object(), Node* n = nullptr)
			: data{ d }, next{ n } { }
		Node(Object&& d, Node* n = nullptr)
			: data{ std::move(d) }, next{ n } { }
	};
	int amount;
	Node* front;
	Node* back;
public:
	Queue(int length = 69) { amount = 0;  front = nullptr; back = nullptr; }
	bool empty() const { return front->next == nullptr; }
	void clear() {
		while (!empty())
			pop_front();
	}
	~Queue() {}
	Queue front() { return *begin() }
	Queue back() { return }
};

template<typename T>
struct Node
{
	Node(const T& val, Node* nxtptr) :value(val), next(nxtptr) {}
	T value;
	Node* next;
};

template<typename T>
class Queue
{
public:
	Queue() :head(nullptr), last(nullptr), size(0) {}
	void push(const T& val);
	void pop();
	T& front();
	bool empty() const;
private:
	Node<T>* head;
	Node<T>* last;
	unsigned int size;
};

template<typename T>
bool Queue<T>::empty()
{
	return (!head && !last&&size == 0);
}

template<typename T>
void Queue<T>::push(const T& val)
{
	if (empty())
	{
		head = last = new Node<T>(val, nullptr); last = last->next; ++size;
	}
	else
	{
		last = new Node<T>(val, nullptr); lats = last->next; ++size;
	}
}

template<typename T>
void Queue<T>::pop()
{
	if (!empty())
	{
		Node<T>* discard = head;
		head = head->next; --size;
		delete discard;
	}
}

template<typename T>
T& Queue<T>::front()
{
	if (!empty())
		return head->value;
}