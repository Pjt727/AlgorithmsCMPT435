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


template <typename T>
struct BinNode{
    T data;
    BinNode<T>* left;
    BinNode<T>* right;
};

template <typename T>
class BinarySearchTree {
    private:
        BinNode<T>* head = nullptr;
    
    public:
        void insert(T data){
            BinNode<T>* newNode = new BinNode<T>;
            newNode->data = data;
            newNode->left = nullptr;
            newNode->right = nullptr;

            // empty case
            if(head == nullptr){
                head = newNode;
                cout << runningPath << "HEAD" << endl;
                return;
            }

            // trailing node to get previous after termination
            BinNode<T>* trailingNode;
            BinNode<T>* consideredNode = head;
            // terminates when left or right is null
            while(consideredNode != nullptr){
                trailingNode = consideredNode;
                if(data >= consideredNode->data){
                    consideredNode = consideredNode->right;
                } else {
                    consideredNode = consideredNode->left;
                }
            }
            // recheck check side
            // if comparison is expensive can store a bool from
            //  the while instead of rechecking
            if(data >= trailingNode->data){
                trailingNode->right = newNode;
            } else {
                trailingNode->left = newNode;
            }
        }

        int search(T data){
            auto consideredNode = head;
            // start at one for the first while check
            int comparisonCount = 1;
            while(consideredNode != nullptr && consideredNode->data != data){
                comparisonCount++;
                if(data >= consideredNode->data){
                    consideredNode = consideredNode->right;
                } else {
                    consideredNode = consideredNode->left;
                }
            }

            return comparisonCount;
        }

        BinNode<T>* getHead(){
            return this->head;
        }
};