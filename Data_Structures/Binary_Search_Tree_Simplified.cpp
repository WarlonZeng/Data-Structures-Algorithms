#include <iostream>
#include <exception>

using namespace std;

template< class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode
{
    Comparable  element;
    BinaryNode *left;
    BinaryNode *right;
    int size;
    
    BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt, int sz = 1 ) : element( theElement ), left( lt ), right( rt ), size( sz ) { }
    BinaryNode( Comparable && theElement, BinaryNode *lt, BinaryNode *rt, int sz = 1 ) : element( std::move(theElement) ), left( lt ), right( rt ), size( sz ) { }
    
    friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree
{
public:
    typedef BinaryNode<Comparable> Node;
    
    BinarySearchTree( ) : root( NULL ) { } // Construct the tree.
    ~BinarySearchTree( ){ makeEmpty( ); } // Destructor for the tree.
    

    // add function declarations
    
    
    bool isEmpty( ) const { return root == NULL;}
    void makeEmpty( ) { makeEmpty( root ); }
    void insert( const Comparable & x ) { insert( x, root ); }
    
private:
    Node * root;

    // add function declarations
    
    int treeSize( Node *t ) const { return t == NULL ? 0 : t->size; }
    Comparable const * elementAt( Node *t ) const;
    void insert( const Comparable & x, Node * & t );
    void makeEmpty( Node * & t ) const;
    
};


template <class Comparable>
Comparable const * BinarySearchTree<Comparable>::elementAt( Node *t ) const
{
    if( t == NULL )
        return NULL;
    else
        return &(t->element);
}


template <class Comparable>
void BinarySearchTree<Comparable>::insert( const Comparable & x, Node * & t )
{
    if( t == NULL )
        t = new Node( x, NULL, NULL, 0 );
    else if( x < t->element )
        insert( x, t->left );
    else if( t->element < x )
        insert( x, t->right );
    else
        throw exception( );
    
    t->size++;
}


template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( Node * & t ) const
{
    if( t != NULL )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
        t = NULL;
    }
}
