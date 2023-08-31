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

// A queue needs to add and remove elements in a FIFO structure
// A queue object itself only ever stores two pointers
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
                throw logic_error("Cannot dequeue from empty array.");
            }
            Node<T>* throwInTheGarbage = first;
            T value = first->data;
            first = first->next;
            delete throwInTheGarbage;

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

// Run tests
int main(){
    testQueue();
    return 0;
}
