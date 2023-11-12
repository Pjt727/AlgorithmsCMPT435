#include<iostream>
#include<fstream>
#include <iomanip>
using namespace std;

const int DEVILS_NUMBER = 666;

// reading in magic items from relative path
string* getMagicItems(string filepath){
    string* magicItems= new string[DEVILS_NUMBER];
    ifstream magicItemsStream(filepath);
    string line; 
    int i = 0;
    while(getline(magicItemsStream, line)){
        magicItems[i] = line;
        i++;
    }
    magicItemsStream.close();
    return magicItems;
}

// Wraps a generic, T, typed data in a node to reference other nodes
template <typename T>
struct Node{
    T data;
    Node<T>* next;
};

// adds and remove elements in a FIFO manner
// only ever stores two pointers
// contains and hides the node class to maintain order of nodes
template <typename T>
class Queue {
    private:
        Node<T>* first;
        Node<T>* last;

    public:
        Queue() : first(nullptr), last(nullptr) {};

        // add element in constant time
        void enqueue(T value){
            Node<T>* node = new Node<T>;
            node->data = value;
            node->next = nullptr;
            
            if(first == nullptr){
                first = node;
            }

            if(last == nullptr){
                last = node;
            } else {
                last->next = node;
                last = node;
            }
        }

        // remove element in constant time if there are elements in queue
        // deletes the removed node from memory
        T dequeue() {
            if(isEmpty()) {
                throw logic_error("Cannot dequeue from empty queue.");
            }
            Node<T>* trash = first;
            T value = first->data;
            first = first->next;
            delete trash;

            if (first == nullptr) {
                last = nullptr;
            }

            return value;
        }

        // constant time
        bool isEmpty() {
            return first == nullptr;
        }
};