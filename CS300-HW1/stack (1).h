#include <iostream>
using namespace std;


template <typename T>
class Stack{
  private:
  struct Node{
    T data;
    Node* next;

    //Initializer List
    Node(const T& newData, Node* newNode = nullptr): data(newData),next(newNode){}
  };
  Node* top;
  public:
  //Constructor
  Stack();
  //Copy Constructor
  Stack(const Stack&rhs);
  //Destructor
  ~Stack();

  //Functions
  bool isEmpty() const;
  void makeEmpty();
  void pop();
  void push(const T&newData);
  const T & topData() const;
  T getTopData() const;
  //operator overloading
  const Stack & operator=(const Stack & rhs);

  friend class Maze;

};

template <typename T>
Stack <T>::Stack(): top(nullptr){}
/**
 * Destructing
*/
template <typename T>
Stack<T>::~Stack(){
   makeEmpty();
}
/**
 * Copy Constructor
*/
template <typename T>
Stack <T>:: Stack(const Stack&rhs){
   top = nullptr;
   *this = rhs;
}
/**
 * if it's empty return true,
 * else return false
*/
template <typename T>
bool Stack<T>::isEmpty() const{
   return top == nullptr;
}
/***
 * make stack logically empty
*/
template <typename T>
void Stack<T>::makeEmpty(){
   while(!isEmpty()){
      pop();
   }
}
/**
 * pop the item
 * stored in the top element
 * and delete its node after
 * changing the top to the node
 * one below
*/
template <typename T>
void Stack <T>::pop(){
   
    T item = top->data;
   Node* tempTop = top;
   top = top->next;
   delete tempTop;
}
/**
 * inserting into the list
*/
template <typename T>
void Stack <T>::push(const T&newData){
   Node* newNode;
   //allocating memory for the new node
   newNode = new Node(newData);
   

   //if the list is empty
   if (isEmpty())
    {
        top = newNode;
        newNode->next = nullptr;
    }
    else    //inserting the new nodebefore top
    {
        newNode->next = top;
        top = newNode;
    }
}
/**
 * return the item
 * stored in the top element,
 * else throw an exception
*/
template <typename T>
const T & Stack<T>::topData() const{
   return top->data;
}
/***getting the top for a case that
 * might me needed in elsewhere of the
 * code
*/
template <typename T>
T Stack<T>::getTopData() const{
   return top->data;
}
/***deep copy*/
template <typename T>
const Stack <T>& Stack<T>::operator=(const Stack & rhs){
   if(this == &rhs){
      return *this;
   }
   else{
      makeEmpty();
      Node* rTop = rhs.top;
      if (rTop != nullptr) //source is not empty so make a copy
        {
            top = new Node(rTop->data);
            Node* temp = top;

            // copying the remaining elements using a while loop
            while (rTop->next != nullptr) {
                rTop = rTop->next;
                temp->next = new Node(rTop->data);
                temp = temp->next;
            }
        }
      return *this;
    }
}









