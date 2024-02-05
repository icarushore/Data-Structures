//
// Created by Nehir İlkim Gönençer on 10.01.2024.
//

#ifndef UNTITLED4_HASHTABLE_H
#define UNTITLED4_HASHTABLE_H

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

template <class Key, class Value>
class HashTable
{
public:
    explicit HashTable( const Value & notFound, int size = 53); //CONSTURCTOR
    HashTable( const HashTable & rhs ) //   COPY CONSTRUCTOR
            : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND ),
              array(rhs.array), currentSize( rhs.currentSize ) { }

    const Value & find(const Key & x) const;

    void makeEmpty();
    void insert( const Key& x,const Value&v);
    void remove(const Key& x);
    void printPreprocess();

    const HashTable & operator=( const HashTable & rhs );

    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry
    {
        Key word;
        Value value;
        EntryType info;

        HashEntry( const Key & e = Key( ),const Value& v = Value(),
                   EntryType i = EMPTY )
                : word(e),value(v),info(i) { }
    };

    vector<HashEntry> array;
    int currentSize; //# of item
    const double LOAD_FACTOR_THRESHOLD = 0.45; // Load factor threshold for rehashing
    const Value ITEM_NOT_FOUND;

    bool isActive( int currentPos ) const;
    int findPos(const Key & x) const;
    int nextPrime(int n);
    bool isPrime(int n);
    int hash (const Key & key, int tableSize) const;
    void rehash();

};
/**
 * Construct the hash table.
 */
template <class Key, class Value>
HashTable<Key,Value>::HashTable( const Value & notFound,
                                 int size )
        : ITEM_NOT_FOUND(notFound), array( nextPrime( size ) )
{
    for (int i = 0; i < array.size(); ++i)
    {
        array[i] = HashEntry(); // Ensure proper initialization of HashEntry elements
    }
}
/**
 * Method that performs quadratic probing resolution.
 * Return the position where the search for x terminates.
 */
template <class Key, class Value>
int HashTable<Key, Value>::findPos(const Key &x) const
{
    int currentPos = hash(x, array.size());
    int originalPos = currentPos;

    while (array[currentPos].info != EMPTY &&
           array[currentPos].word != x)
    {
        currentPos = (currentPos + 1) % array.size();  // Linear probing
        if (currentPos == originalPos) {
            // If we've gone through the entire array, stop to avoid an infinite loop
            break;
        }
    }

    return currentPos;
}

/**
  * Return true if currentPos exists and is active.
  */
template <class Key, class Value>
bool HashTable<Key,Value>::isActive( int currentPos ) const
{
    return array[currentPos].info == ACTIVE;
}
/**
 * Remove item x from the hash table.
 *  x has to be in the table
 */
template <class Key, class Value>
void HashTable<Key,Value>::remove( const Key& x )
{
    int currentPos = findPos(x);
    if ( isActive(currentPos ) ){
        array[ currentPos ].info = DELETED;
    }
}
/**
 * Find item x in the hash table.
 * Return the matching item, or ITEM_NOT_FOUND, if not found.
 */
template <class Key, class Value>
const Value& HashTable<Key,Value>::find(const Key & x)
const
{
    int currentPos = findPos(x);
    if (isActive(currentPos)){
        return array[currentPos].value;
    }
    return  ITEM_NOT_FOUND;
}
/**
  * Insert item x into the hash table. If the item is
  * already present, then do nothing.
  */
template <class Key, class Value>
void HashTable<Key, Value>::insert(const Key &x, const Value &v)
{
    // Insert x as active
    int currentPos = findPos(x);
    if (isActive(currentPos)){
        return;
    }
    // Check load factor and rehash if necessary
    if (static_cast<double>(currentSize) / array.size() > LOAD_FACTOR_THRESHOLD){
        rehash();
    }
    array[currentPos] = HashEntry(x, v, ACTIVE);
    ++currentSize;
}
/**
 * Expand the hash table.
 */
template <class Key, class Value>
void HashTable<Key, Value>::rehash()
{
    vector<HashEntry> oldArray = array;

    // Create new double-sized, empty table
    array.resize(nextPrime(2 * oldArray.size()));
    for (int j = 0; j < array.size(); j++){
        array[j].info = EMPTY;
    }

    // Copy table over
    currentSize = 0;
    for (int i = 0; i < oldArray.size(); i++)
    {
        if (oldArray[i].info == ACTIVE)
        {
            int newPos = findPos(oldArray[i].word);
            array[newPos] = HashEntry(oldArray[i].word, oldArray[i].value, ACTIVE);
            ++currentSize; // Increment currentSize for each inserted element
        }
    }


}

/**
        * Internal method to return a prime number
       * at least as large as n.  Assumes n > 0.
        */
template<class Key,class Value>
int HashTable<Key,Value>:: nextPrime( int n )
{
    if (n % 2 == 0){
        n++;
    }
    for ( ; ! isPrime(n); n += 2 ){
        ;
    }
    return n;
}
/**
         * Internal method to test if a positive number is prime.
         * Not an efficient algorithm.
         */
template<class Key,class Value>
bool HashTable<Key,Value>:: isPrime( int n )
{
    if (n == 2 || n == 3){
        return true;
    }


    if (n == 1 || n % 2 == 0){
        return false;
    }


    for ( int i = 3; i * i <= n; i += 2 ){
        if ( n % i == 0 ){
            return false;
        }
    }
    return true;
}
/**
  * Make the hash table logically empty.
  */
template <class Key,class Value>
void HashTable<Key,Value>::makeEmpty( )
{
    for( int i = 0; i < array.size( ); i++){
        array[i].makeEmpty();
    }
    // destroy the lists but not the vector!
}
template<class Key,class Value>
int HashTable<Key,Value>:: hash (const Key & key, int tableSize) const
{
    int hashVal = 0;

    for (int i = 0; i < key.length();i++){
        hashVal = 37 * hashVal + key[ i ];
    }

    hashVal = hashVal % tableSize;

    if (hashVal < 0){
        hashVal = hashVal + tableSize;
    }
    return(hashVal);
}
template <class Key, class Value>
void HashTable<Key,Value>:: printPreprocess(){
    cout << "After preprocessing, the unique word count is " << currentSize << ". Current load ratio is: " << static_cast<double>(currentSize) / array.size();
}
#endif


#endif //UNTITLED4_HASHTABLE_H
