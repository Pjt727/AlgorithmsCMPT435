#include<iostream>
#include<fstream>
#include<vector>

using namespace std;


// Wraps a generic, T, typed data in a node to reference other nodes
// For certain types data may be more appropriately a pointer
template <typename T>
struct Node{
    T data;
    Node* next;
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
        // deleted the removed node from memory
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

// adds and remove elements in a FILO manner
// only ever stores one pointer
// contains and hides the node class to maintain order of nodes
template <typename T>
class Stack {
    private:
        Node<T>* top;
    
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
// Add and remove elements from a queue 
void testQueue() {
    int numbers[10] = {1,2,3,4,5,6,7,8,9,10};

    Queue<int> queue;
    for(int num: numbers ){
        queue.enqueue(num);
    }

    int value;
    while(!queue.isEmpty()) {
        value = queue.dequeue();
        cout << value << endl;
    }

    for(int num: numbers ){
        queue.enqueue(num);
    }

    while(!queue.isEmpty()) {
        value = queue.dequeue();
        cout << value << endl;
    }
}

// add and remove elements from a stack
void testStack() {
    int numbers[10] = {1,2,3,4,5,6,7,8,9,10};

    Stack<int> stack;
    for(int num: numbers ){
        stack.push(num);
    }

    int value;
    while(!stack.isEmpty()) {
        value = stack.pop();
        cout << value << endl;
    }

    for(int num: numbers ){
        stack.push(num);
    }

    while(!stack.isEmpty()) {
        value = stack.pop();
        cout << value << endl;
    }

}
// Run tests
int main(){
    testQueue();
    testStack();
    return 0;
}
