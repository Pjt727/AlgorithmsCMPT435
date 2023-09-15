#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>

using namespace std;

// Dirty dirty constants
const int DEVILS_NUMBER = 666;


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

// adds and remove elements in a LIFO manner
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

// Add and remove elements from a queue twice
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
bool isPalindrome(string &candidate){
    Queue<char> fifo;
    Stack<char> lifo;
    for(const char ch: candidate) {
        if(ch == ' '){
            continue;
        }
        // same case
        fifo.enqueue(tolower(ch));
        lifo.push(tolower(ch));
    }

    // asymmetrical patten since fifo and lifo should be empty at the same iteration
    while(!fifo.isEmpty() ){
        if(fifo.dequeue() != lifo.pop()){
            return false;
        }
    }

    return true;


}
void cpalindromes(vector<string>& candidates){
    cout << "Showing Palindromes: " << endl;
    for(string& candidate : candidates){
        if(isPalindrome(candidate)){
            cout << candidate << endl;
        }
    }
}

string* getMagicItems(){
    string* magicItems= new string[DEVILS_NUMBER];
    ifstream magicItemsStream("magicitems.txt");
    string line; 
    int i = 0;
    while(getline(magicItemsStream, line)){
        magicItems[i] = line;
        i++;
    }
    magicItemsStream.close();
    return magicItems;
}

// In place fisher yates shuffle https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
void fisherYatesShuffle(string* items){
    int swapIndex;

    // Reseeding the number generator with the psuedo random time
    // This means that multiple shuffles within the program can be linked through their delay between shuffle invocations
    // If "better" randomness is desired the random library could be considered
    srand(static_cast<unsigned>(time(nullptr)));

    for(int end = DEVILS_NUMBER-1; end > 0; end--){
        swapIndex = rand() % (end + 1);
        if(swapIndex == end) { continue; }
        // probably internally done using an XOR swap https://en.wikipedia.org/wiki/XOR_swap_algorithm
        //  of the chars in the string
        // Using the inbuilt swap is more elegant since we can interface an object type into this sort if needed
        // This wont work with primitive types though
        swap(items[end], items[swapIndex]);
    }
}


// Sorts the items in ascending order in O(n^2) time 
// Finds the min of the remaining items and swaps it to the end of the remaining items
void selectionSort(string* items) {
    int minIndex;
    for(int i = 0; i < DEVILS_NUMBER; i++){
        minIndex = i;
        for(int j = i + 1; j < DEVILS_NUMBER; j++){
            if(items[j] < items[minIndex]){
                minIndex = j;
            }
        }
        if(minIndex == i){ continue; }
        swap(items[i], items[minIndex]);
    }
}

// Sorts the items in ascending order in O(n^2) time
void insertionSort(string* items){
    if(DEVILS_NUMBER <= 1) { return; }
    string selectedItem;
    int prevIndex;
    for(int i=1; i < DEVILS_NUMBER; i++){
        selectedItem = items[i];
        prevIndex = i - 1;
        while(( prevIndex >= 0 ) && (items[prevIndex] > selectedItem)) {
            items[prevIndex + 1] = items[prevIndex];
            prevIndex--;
        }
        items[prevIndex + 1] = selectedItem;
    }

}

// Run tests
int main(){
    // testQueue();
    // testStack();
    string* magicItems = getMagicItems();
    // cpalindromes(magicItems);

    fisherYatesShuffle(magicItems);
    // selectionSort(magicItems);
    // insertionSort(magicItems);
    for(int i = 0; i < 9; i++){
        cout << magicItems[i] << endl;
    }
    return 0;
}
