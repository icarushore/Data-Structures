//
// Created by Nehir İlkim Gönençer on 9.12.2023.
//

#ifndef BST_H
#define BST_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;



/**
 * THINK KEY -> string word
 * VALUE -> vector*
*/
template <class Key, class Value>
class BST;

template <class Key, class Value>
class BSTNode
{
    Key  word;
    Value value;
    BSTNode *left;
    BSTNode *right;
    int  height;

    BSTNode( const Key & theElement,
             const Value & theVal, BSTNode *lt, BSTNode *rt, int h = 0)
            : word( theElement ), value( theVal ), left( lt ), right( rt ), height(h ) { }

    friend class BST<Key, Value>;
};

template<class Key,class Value>
class BST{
private:

    BSTNode<Key,Value> * root;

    const Value ITEM_NOT_FOUND;

    const Value & elementAt( BSTNode<Key,Value> *t ) const;

    BSTNode<Key,Value>* insert( const Key& x,const Value&v, BSTNode<Key,Value>* & t );
    BSTNode<Key,Value>* remove(const Key& x,BSTNode<Key,Value>* & t );

    BSTNode<Key,Value>* findMin( BSTNode<Key,Value> *t ) const;
    BSTNode<Key,Value>* findMax(BSTNode<Key,Value> *t ) const;
    BSTNode<Key,Value>* find( const Key & x, BSTNode<Key,Value>*t );
    void makeEmpty( BSTNode<Key,Value> * & t ) const;
    void printTree(BSTNode<Key,Value> *t) const;
    BSTNode<Key,Value> * clone( BSTNode<Key,Value>*t) const;

    // Avl manipulations
    int height(BSTNode<Key,Value>*t);
    int max(int lhs, int rhs) const;
    BSTNode<Key,Value>* rightRotate(BSTNode<Key,Value>*t);
    BSTNode<Key,Value>* leftRotate(BSTNode<Key,Value>*t);



public:
    explicit BST( const Value & notFound );
    BST(const BSTNode<Key,Value> & rhs);
    ~BST();

    const Key & findMin( ) const;
    const Key & findMax( ) const;
    const Value & find(const Key & x);
    bool isEmpty( ) const;
    void printTree( ) const;

    void makeEmpty( );
    void insert(const Key & x ,const Value&v);
    void remove(const Key & x );



    //functions that are need for program implementation
    bool contains(const Key &x);
    int getBalance(BSTNode<Key,Value>*t);
    Value& getValue();

};

//CONSTRUCTOR
template<class Key, class Value>
BST<Key, Value>::BST(const Value & notFound)
        : ITEM_NOT_FOUND(notFound), root(nullptr) {}

//DESTRUCTOR
template<class Key,class Value>
BST<Key,Value>::~BST(){
    makeEmpty();
}
//COPY CONSTRUCTOR
template<class Key,class Value>
BST<Key,Value>::BST(const BSTNode<Key,Value> & rhs):root( nullptr ), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND )
{
    *this = rhs;
}
/**
 * If t is NULL (i.e., the node doesn't exist),
 *  it returns the constant ITEM_NOT_FOUND. Otherwise,
 * it returns the value stored in the node.
*/
template<class Key,class Value>
const Value & BST<Key,Value>::elementAt(BSTNode<Key, Value>*t) const{
    return t == nullptr ? ITEM_NOT_FOUND: t->value;
}
/**
    * Find item x in the tree.
    * Return the matching item or ITEM_NOT_FOUND if not found.
*/
//Public method: find
template<class Key, class Value>
const Value & BST<Key,Value>::find(const Key& x){

    return elementAt(find(x, root));
}

/**
    * Internal method to find an item in a subtree.
    * x is item to search for.
    * t is the node that roots the tree.
    * Return node containing the matched item.
*/
template<class Key, class Value>
BSTNode<Key,Value>* BST<Key,Value>::find(const Key& x, BSTNode<Key, Value>*t ){
    //if the tree is empty
    if(t==nullptr){
        return nullptr;
    }
    else if(x < t->word){ //if x is alphabetically before the root word
        return find(x,t->left); // go to left subtree
    }
    else if(t->word < x){ //if x is alphabetically after the root word
        return find(x,t->right); //go yo right subtree
    }
    return t; //match
}
template<class Key, class Value>
int BST<Key,Value>::height(BSTNode<Key,Value>* t) {
    if(t==nullptr){
        return -1;
    }
    return t->height;
}

/**
   * Return maximum of lhs and rhs.
*/
template<class Key, class Value>
int BST<Key,Value>:: max( int lhs, int rhs ) const{ //the inputs will be heights
    return (lhs > rhs)? lhs : rhs;
}
//checks if the tree is empty or not
template<class Key,class Value>
bool BST<Key,Value>::isEmpty() const{
    if(root == nullptr){ //if the tree is empty
        return true;
    }
    return false;
}
//external findMin function
template<class Key,class Value>
const Key & BST<Key,Value>::findMin() const{
    return elementAt(findMin(root));
}
//internal findMin function
template<class Key, class Value>
BSTNode<Key,Value>* BST<Key, Value>::findMin(BSTNode<Key,Value>* t) const {
    // keep traversing left until you reach the leftmost leaf
    while (t != nullptr && t->left != nullptr){
        t = t->left;
    }
    return t;
}
//external findMax function
template<class Key,class Value>
const Key& BST<Key,Value>::findMax() const{
    return elementAt(findMax(root));
}
//intern findMax function
template<class Key,class Value>
BSTNode<Key,Value>* BST<Key,Value>::findMax(BSTNode<Key,Value>*t) const{
    // keep traversing left until you reach the rightmost lead
    while(t!= nullptr && t ->right != nullptr){
        t = t->right;
    }
    return t;
}
//external makeEmpty
template<class Key,class Value>
void BST<Key,Value>::makeEmpty(){
    makeEmpty(root);
}
//internal makeEmpty
template<class Key, class Value>
void BST<Key,Value>::makeEmpty(BSTNode<Key,Value>* & t ) const{
    if(t != nullptr){
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = nullptr;
}
/*
    external printTree function
    FOR DEBUG PURPOSES
*/
template<class Key, class Value>
void BST<Key,Value>::printTree() const{
    if( isEmpty()){
        cout << "Empty tree" << endl;
    }
    else{
        printTree(root);
    }
}
//internal printTree functions
template<class Key,class Value>
void BST<Key,Value>::  printTree(BSTNode<Key,Value>*t) const{
    if (t != nullptr) {
        // printing the left subtree
        printTree(t->left);
        //printing the current node
        cout << t->word << " (Height: " << t->height << ") ";
        //printing the right subtree
        printTree(t->right);
    }
}
template<class Key,class Value>
bool BST<Key,Value>:: contains(const Key &x){
    return find(x, root) != nullptr;
}
template<class Key, class Value>
BSTNode<Key,Value>* BST<Key,Value>::clone(BSTNode<Key,Value>* t) const {
    if (t == nullptr) {
        return nullptr;
    }

    // recursively clone left and right subtrees
    BSTNode<Key,Value>* cloneLeft = clone(t->left);
    BSTNode<Key,Value>* cloneRight = clone(t->right);

    // Create a new WordItem for the current node with the cloned left and right subtrees
    BSTNode<Key,Value>* clone = new BSTNode<Key,Value>(t->word);
    clone->left = cloneLeft;
    clone->right = cloneRight;
    clone->value = t->value;

    return clone;
}
/**
    * Insert x into the tree; duplicates are ignored.
*/
template<class Key, class Value>
void BST<Key,Value>:: insert(const Key & x,const Value&v) {
    insert(x,v, root); //insert it into the list
}
template<class Key, class Value>
BSTNode<Key,Value>* BST<Key, Value>::insert(const Key& x, const Value& v, BSTNode<Key, Value>* & t) {
    if (t == nullptr) { //if the tree is empty
        t = new BSTNode<Key, Value>(x, v, nullptr, nullptr);

    } else if (x < t->word) { // If x is alphabetically before the root word
        // Go to the left subtree
        if(t->left != nullptr){
            insert(x, v, t->left);
        }
        else{
            t->left = new  BSTNode<Key, Value>(x, v, nullptr, nullptr);
        }
        // bu insertion islemini yapiyo
    } else if (t->word < x) { // If x is alphabetically after the root word
        // Go to the right subtree
        if(t->right != nullptr){
            insert(x, v, t->right);
        }
        else{
            t->right = new  BSTNode<Key, Value>(x, v, nullptr, nullptr);
        }
    }

    //updating the height of the current node
    t->height = max(height(t->left), height(t->right)) + 1;
    return t;
}
//external remove function
template<class Key,class Value>
void BST<Key,Value>::remove(const Key&x){
    root = remove(x, root);
}
template<class Key, class Value>
BSTNode<Key, Value>* BST<Key, Value>::remove(const Key& x, BSTNode<Key, Value>* & t) {
    // Base case: if the tree is empty
    if (t == nullptr) {
        return t;
    }

    // Recursive case: search for the node to be deleted
    if (x < t->word) {
        t->left = remove(x, t->left);
    } else if (t->word < x) {
        t->right = remove(x, t->right);
    } else {
        // Node with only one child or no child
        if (t->left == nullptr || t->right == nullptr) {
            BSTNode<Key, Value>* temp = t->left ? t->left : t->right;

            // No child case
            if (temp == nullptr) {
                temp = t;
                t = nullptr;
            } else { // One child case
                *t = *temp; // Copy the contents of the non-empty child
            }

            delete temp;
        } else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            BSTNode<Key, Value>* successor = findMin(t->right);

            // Copy the inorder successor's data to this node
            t->word = successor->word;
            t->value = successor->value;

            // Delete the inorder successor
            t->right = remove(successor->word, t->right);
        }
    }

    // If the tree had only one node, return
    if (t == nullptr) {
        return t;
    }

    // Update the height of the current node
    t->height = max(height(t->left), height(t->right)) + 1;
    return t;
}

#endif //BST_H
