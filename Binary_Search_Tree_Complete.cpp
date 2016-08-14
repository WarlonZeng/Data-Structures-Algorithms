// CS2134 Fall 2015
// HW7 PQ2 Solution

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////
// Binary Search Tree Class (Based on Lecture Slides)
///////////////////////////////////////////////////////

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode {
    Comparable element;
    BinaryNode* left;
    BinaryNode* right;
    int size;

    BinaryNode (const Comparable& theElement, BinaryNode* lt = nullptr, BinaryNode* rt = nullptr, int sz = 0)
    : element(theElement), left(lt), right(rt), size(sz) {}

    BinaryNode (Comparable&& theElement, BinaryNode* lt = nullptr, BinaryNode* rt = nullptr, int sz = 0)
    : element(move(theElement)), left(lt), right(rt), size(sz) {}

    friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree {
public:
    typedef BinaryNode<Comparable> Node;

    BinarySearchTree() : root(nullptr) {}
    // You should also have the Big 5 here, but for brevity
    // on this solution, they have been removed.

    // These are some functions used to help test/write our code
    bool isEmpty () const { return root == nullptr; }
    int size() const { return root == nullptr ? 0 : root->size; }
    void insert(const Comparable& x) { insert(x, root); }
    void inOrderTraversal() const { inOrderTraversal(root); cout << endl; }

    // Driver functions for the things we need to implement
    bool find (const Comparable& x) const { return find(x, root); }
    void printRange (const Comparable& low, const Comparable& high) const { printRange(low, high, root); }
    void negateTree() { negateTree(root); }
    // Notice that if the tree is empty, we return -1 -- this is to differentiate from when there's only one node
    double average_node_depth() const { return isEmpty() ? -1 : average_node_depth(root, 0) / size(); }

private:
    Node* root;

    // I make use of this function just to test my code
    // Notice that we're passing the pointer in BY REFERENCE
    void insert(const Comparable& x, Node*& t) {
        if (t == nullptr)
            t = new Node(x);
        else if (x < t->element)
            insert(x, t->left);
        else if (x > t->element)
            insert(x, t->right);
        else
            throw exception();
        t->size++;
    }

    // Using inOrderTraversal to check that the inserts work
    void inOrderTraversal (Node* t) const {
        if (t == nullptr) return;
        inOrderTraversal(t->left);
        cout << t->element << " ";
        inOrderTraversal(t->right);
    }

    // Signatures for functions we need to write
    Node* find(const Comparable& x, Node* t) const;
    void printRange (const Comparable& low, const Comparable& high, Node* t) const;
    double average_node_depth(Node* t, int depth) const;
    void negateTree(Node* t);
};

///////////////////////////////////////////////////////
// PQ2 Solutions
///////////////////////////////////////////////////////

// PART A: find()
template <class Comparable>
typename BinarySearchTree<Comparable>::Node* BinarySearchTree<Comparable>::find (const Comparable& x, Node* t) const {
    if (t == nullptr) return nullptr;
    if (x > t->element) return find(x, t->right);
    if (x < t->element) return find(x, t->left);
    return t;
}

// PART B: printRange()
template <class Comparable>
void BinarySearchTree<Comparable>::printRange (const Comparable& low, const Comparable& high, Node* t) const {
    if (t == nullptr) return;

    // If the element at this node is lower than the low bound, we don't need to look any lower
    // We do, however, need to look higher
    if (t->element <= low) printRange(low, high, t->right);

    // Same concept as above, but we need to look at the left children (they're smaller)
    if (t->element >= high) printRange(low, high, t->left);

    // If you're exactly on the bounds, we'll do some printing
    if (t->element == low || t->element == high) cout << t->element << " ";

    // If you're between the two bounds, then we need to print and check both children
    if (t->element > low && t->element < high) {
        cout << t->element << " ";
        printRange(low, high, t->left);
        printRange(low, high, t->right);
    }
}

// PART C: negateTree()
template <class Comparable>
void BinarySearchTree<Comparable>::negateTree (Node* t) {
    if (t == nullptr) return;

    t->element = -(t->element);
    Node* temp = t->left;
    t->left = t->right;
    t->right = temp;

    negateTree(t->left);
    negateTree(t->right);
}

// PART D: average_node_depth()
template <class Comparable>
double BinarySearchTree<Comparable>::average_node_depth (Node* t, int depth) const {
    if (t == nullptr) return 0;
    return depth + average_node_depth(t->right, depth+1) + average_node_depth(t->left, depth+1);
}

///////////////////////////////////////////////////////
// It's always important to test your code!
///////////////////////////////////////////////////////

string boolToString (bool flag) { return flag ? "yes" : "no"; }

int main () {
    BinarySearchTree<int> aTree;
    cout << "\nCreated a new tree with no elements" << endl;
    cout << "Size: " << aTree.size() << endl;
    cout << "Average Node Depth: " << aTree.average_node_depth() << endl;

    aTree.insert(5);
    cout << "\nInserted one element (value 5)" << endl;
    cout << "Size: " << aTree.size() << endl;
    cout << "Average Node Depth: " << aTree.average_node_depth() << endl;

    aTree.insert(10);
    aTree.insert(1);
    aTree.insert(2);
    aTree.insert(7);
    cout << "\nInserted four more elements {10, 1, 2, 7}" << endl;
    cout << "Size: " << aTree.size() << endl;
    cout << "Inorder Traversal: ";
    aTree.inOrderTraversal();
    cout << "Average Node Depth: " << aTree.average_node_depth() << endl;
    cout << "Is 0 in this tree? " << boolToString(aTree.find(0)) << endl;
    cout << "Is 7 in this tree? " << boolToString(aTree.find(7)) << endl;

    aTree.negateTree();
    cout << "\nNegated the tree" << endl;
    cout << "Size: " << aTree.size() << endl;
    cout << "Inorder Traversal: ";
    aTree.inOrderTraversal();
    cout << "Average Node Depth: " << aTree.average_node_depth() << endl;
    cout << "Is 0 in this tree? " << boolToString(aTree.find(0)) << endl;
    cout << "Is 7 in this tree? " << boolToString(aTree.find(7)) << endl;
    cout << "Is -7 in this tree? " << boolToString(aTree.find(-7)) << endl;

    cout << "\nPrint range between -8 and -1 (inclusive):" << endl;
    aTree.printRange(-8, -1);
    cout << "(Notice that this doesn't need to print in order.)\n";

    cout << endl;
}
