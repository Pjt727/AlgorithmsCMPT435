#include<fstream>
using namespace std;

string leftPad(string toPad, int size){
    int curLength = toPad.length();
    while(curLength < size){
        toPad = " " + toPad;
        curLength++;
    }
    return toPad;
}

string rightPad(string toPad, int size){
    int curLength = toPad.length();
    while(curLength < size){
        toPad += " ";
        curLength++;
    }
    return toPad;
}

string middlePad(string toPad, int size){
    int curLength = toPad.length();
    bool doPadLeft = true;
    while(curLength < size){
        if(doPadLeft){
            toPad = " " + toPad;
        } else {
            toPad += " ";
        }
        doPadLeft = !doPadLeft;
        curLength++;
    }
    return toPad;
}

template <typename T>
struct Node{
    T data;
    Node<T>* next;
};

// adds and remove elements in a LIFO manner
// only ever stores one pointer
// contains and hides the node class to maintain order of nodes
template <typename T>
class Stack {
    private:
        Node<T>* top = nullptr;
    
    public:
        // add an element in constant time
        void push(T value) {
            Node<T>* node = new Node<T>;
            node->data = value;
            node->next = top;
            top = node;
        }
        // remove an element in constant time if there are elements
        T pop(){
            if(isEmpty()){
                throw logic_error("Cannot pop from empty stack.");
            }
            Node<T>* trash = top;
            T value = top->data;
            top = top->next;
            delete trash;

            return value;
        }

        // constant time
        bool isEmpty(){
            return top == nullptr;
        }
};