// CS2134 Fall 2015
// Homework 5 Programming Solution

#include <iostream>
#include <algorithm>
#include <stack>
using namespace std;

// This is a dummy functor I wrote to test remove_if
class HowOdd {
public:
    bool operator() (const int& val) { return val % 2; }
};

/////////////////////////////////////////////////////////////////
// PROVIDED LIST CODE
/////////////////////////////////////////////////////////////////

template <typename Object>
class List {
private:

    struct Node {
        Object  data;
        Node    *next;

        Node (const Object& d = Object (),  Node* n = nullptr)
        : data{ d },  next{ n } { }

        Node (Object&& d, Node* n = nullptr)
        : data{ std::move( d ) }, next{ n } { }
    };

    Node *header;

public:
    class iterator {
    public:

        iterator(): current( nullptr ) {}

        Object & operator* ( ) { return current->data; }
        
        const Object & operator* ( ) const { return  current->data; }
        
        iterator & operator++ () {
            this->current = this->current->next;
            return *this;
        }

        iterator operator++ (int) {
            iterator old = *this;
            ++(*this);
            return old;
        }

        bool operator== (const iterator & rhs) const { return current == rhs.current; }
        
        bool operator!= (const iterator & rhs) const { return !( *this == rhs ); }

    private:
        Node * current;
        iterator( Node *p ) : current{ p } {}
        friend class List<Object>;
    };

    List() { header = new Node; }

    ~List() {
        clear();
        delete header;
    }

    List & operator= ( const List & rhs ) {
        List copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    List( List && rhs ): header{ new Node } {
        header->next = rhs.header->next;
        rhs.header->next = nullptr;
    }

    List & operator= ( List && rhs ) {
        std::swap( header, rhs.header );
        return *this;
    }

    iterator begin () const { return iterator( header->next ); }

    iterator end () const { return iterator( nullptr ); }

    iterator before_begin () const { return iterator( header ); }

    bool empty () const { return header->next == nullptr; }

    void clear () {
        while(!empty())
            pop_front();
    }

    void pop_front () { erase_after( before_begin( ) ); }

    iterator insert_after( iterator itr, const Object & x ) {
        Node *p = itr.current;
        p->next = new Node{ x, p->next };
        return iterator(p->next);
    }

    void remove( const Object & x ) {
        Node * prev = header;
        while ( prev->next != nullptr ) {
            if ( prev->next->data == x )
                erase_after( iterator(prev) );
            else
                prev = prev->next;
        }
    }

    iterator erase_after( iterator itr ) {
        Node *p = itr.current;
        Node *oldNode = p->next;

        p->next = oldNode->next;
        delete oldNode;

        return iterator( p->next );
    }

    // Wrote for myself for debugging
    void printInOrder () const {
        cout << "The current list: ";
        Node* temp = header->next;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    // THINGS TO IMPLEMENT IN THE HOMEWORK
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////

    // PART A: COPY CONSTRUCTOR
    // Note that this is a deep copy and shouldn't affect the passed in list

    List (const List& rhs) : header(new Node) {
        Node* curr = header;
        for (iterator temp = rhs.begin(); temp != rhs.end(); temp++) {
            curr->next = new Node(*temp, nullptr);
            curr = curr->next;
        }
    }

    // PART B: FRONT()
    // Returns a direct reference to the element in the first node
    // The STL specifies undefined behavior if the list is empty

    Object& front () {
        return *begin();
    }

    const Object& front() const {
        return *begin();
    }

    // PART C: MERGE()
    // You're allowed to assume that both lists are sorted already

    void merge(List & alist) {
        Node* lhsprev = header;
        Node* lhscurr = header->next;
        Node* rhscurr = alist.header->next;
        while (lhscurr != nullptr && rhscurr != nullptr) {
            if (lhscurr->data < rhscurr->data) {
                lhscurr = lhscurr->next;
                lhsprev = lhsprev->next;
            } else {
                Node* temp = rhscurr->next;
                rhscurr->next = lhscurr;
                lhsprev->next = rhscurr;
                rhscurr = temp;
                lhsprev = lhsprev->next;
            }
        }
        while (rhscurr != nullptr) {
            Node* temp = rhscurr->next;
            rhscurr->next = nullptr;
            lhsprev->next = rhscurr;
            rhscurr = temp;
        }
        alist.header->next = nullptr;
    }

    // PART D: REVERSE()

    // This will reverse the order of the elements in the list
    // This should NOT invalidate iterators

    // There are *many* ways to implement this, whether you
    // use a stack, do it recursively, or just keep track of
    // multiple pointers. Below is just one approach, which
    // makes use of stacks. (This may not be the *most* efficient
    // approach, but is highly recommended by Professor Sellie,
    // since it very nicely simplifies the problem and makes use of
    // other data structures!)

    void reverse() {
        Node* p = header->next;
        stack<Node*> s;
        while(p->next != nullptr){
            s.push(p);
            p = p->next;
        }
        header->next = p;
        while(!s.empty()){
            p->next = s.top();
            p = p->next;
            s.pop();
        }
        p->next = nullptr;
    }

    // PART E: INSERT_AFTER()
    // Note that this works on rvals (but the core code is the same as
    // the provided insert_after())

    iterator insert_after (iterator itr, Object&& x) {
        Node *p = itr.current;
        p->next = new Node{ x, p->next };
        return iterator(p->next);
    }

    // PART F: REMOVE_IF()
    // Uses erase_after() to remove *ALL* elements that match the functor pred

    template<class Predicate>
    void remove_if (Predicate pred) {
        iterator prev = before_begin();
        iterator curr = begin();
        while (curr != end()) {
            if (pred(*curr)) {
                curr = erase_after(prev);
            } else {
                prev++;
                curr++;
            }
        }
    }
};

/////////////////////////////////////////////////////////////////
// Test code for the implemented functions
/////////////////////////////////////////////////////////////////

int main() {
    // Create the list
    List<int> a;

    // Note that I created a printInOrder() function for debugging
    cout << "\nInitialized empty list:\n(A) ";
    a.printInOrder();

    // Test out the provided insert_after()
    int thing = 1;
    cout << "\nUsing the provided insert_after():" << endl;
    cout << "Insert " << thing << endl;
    a.insert_after(a.before_begin(), thing);
    cout << "(A) ";
    a.printInOrder();

    // Part B: Test the functionality of front()
    cout << "\nUsing front():" << endl;
    cout << "Set front to 5" << endl;
    a.front() = 5;
    cout << "Front element is: " << a.front() << endl;
    cout << "(A) ";
    a.printInOrder();

    // Part E: Test the functionality of insert_after() with rval ref
    cout << "\nUsing insert_after() that we've implemented:" << endl;
    cout << "Insert 3 after 5" << endl;
    a.insert_after(a.begin(), 3);
    cout << "(A) ";
    a.printInOrder();
    cout << "Insert 7 at the front" << endl;
    a.insert_after(a.before_begin(), 7);
    cout << "(A) ";
    a.printInOrder();

    // Part D: Test the functionality of reverse()
    cout << "\nUsing reverse():" << endl;
    cout << "(A) ";
    a.printInOrder();
    List<int>::iterator check = a.begin();
    cout << "Iterator 'check' originally points to: " << *check << endl;
    cout << "Now, reverse the list:" << endl;
    a.reverse();
    cout << "(A) ";
    a.printInOrder();
    cout << "Iterator 'check' now points to: " << *check << endl;

    // Part A: Copy Constructor
    cout << "\nCopy constructor:" << endl;
    List<int> b(a);
    cout << "(A) ";
    a.printInOrder();
    cout << "(B) ";
    b.printInOrder();

    // Add some stuff to B
    cout << "\nInsert more things into B:" << endl;
    // b.insert_after(b.before_begin(), 14);
    b.insert_after(b.before_begin(), 11);
    b.insert_after(b.before_begin(), 8);
    b.insert_after(b.before_begin(), 9);
    b.insert_after(b.before_begin(), 4);
    // b.insert_after(b.before_begin(), 2);
    cout << "(A) ";
    a.printInOrder();
    cout << "(B) ";
    b.printInOrder();

    // Part F: remove_if()
    cout << "\nPerform remove_if on B:" << endl;
    b.remove_if(HowOdd());
    cout << "(A) ";
    a.printInOrder();
    cout << "(B) ";
    b.printInOrder();

    // Part C: merge()
    // We're assuming that both lists are already sorted
    // In this case, the two lists happen to be in ascending order, so this works
    cout << "\nPerform merge():" << endl;
    a.merge(b);
    cout << "(A) ";
    a.printInOrder();
    cout << "(B) ";
    b.printInOrder();
    cout << endl;
}

