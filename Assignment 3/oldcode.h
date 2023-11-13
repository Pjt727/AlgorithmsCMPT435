#include<iostream>
#include<fstream>
#include <iomanip>
#include <cmath>
using namespace std;

// helper round function for base 10 numbers
// expected precision to be 10^precision digit
// ex: 92.47 -> 92.5 is round on the -1 precision
double round_prec(double num, int precision) {
    num /= pow(10, precision);
    num = round(num);
    num *= pow(10, precision);
    return num;
}

// reading in magic items from relative path
// refactored into vector
vector<string>* getMagicItems(string filepath){
    vector<string>* magicItems= new vector<string>;
    ifstream magicItemsStream(filepath);
    string line; 
    while(getline(magicItemsStream, line)){
        magicItems->push_back(line);
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