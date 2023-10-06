/*
    Author: Patrick Tyler

    Compiled with g++:
        g++.exe (Rev6, Built by MSYS2 project) 13.1.0
        Copyright (C) 2023 Free Software Foundation, Inc.

    Special Thanks: 
        My parents, my family, and friends. As well as Dr. Labouseur (of course).
        Without them I dont know how I would have gotten through this assignment.

    Date Due: 10/6/2023
*/
#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>

using namespace std;

// Dirty dirty constants
// IMPORTANT: Edit this variable if you want to change the amount of the sorted list that is shown
int truncationCount = 10;
const int DEVILS_NUMBER = 666; // "funny" name for length of items file 

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
        cout << value << ", ";
    }
    cout << endl;

    for(int num: numbers ){
        queue.enqueue(num);
    }

    while(!queue.isEmpty()) {
        value = queue.dequeue();
        cout << value << ", ";
    }
    cout << endl;
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
        cout << value << ", ";
    }
    cout << endl;

    for(int num: numbers ){
        stack.push(num);
    }

    while(!stack.isEmpty()) {
        value = stack.pop();
        cout << value << ", ";
    }
    cout << endl;

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

void cpalindromes(string* candidates){
    for(int i=0; i < DEVILS_NUMBER; i++){
        if(isPalindrome(candidates[i])){
            cout << candidates[i] << endl;
        }
    }
}

// reading in magic items from relative path
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
void fisherYatesShuffle(string* items, int length = DEVILS_NUMBER){
    int swapIndex;

    // Reseeding the number generator with the psuedo random time
    // This means that multiple shuffles within the program can be linked through their delay between shuffle invocations
    // If "better" randomness is desired the random library could be considered
    srand(static_cast<unsigned>(time(nullptr)));

    for(int end = length-1; end > 0; end--){
        // +1 to include current index
        swapIndex = rand() % (end + 1); 
        if(swapIndex == end) { continue; }
        // probably internally done using an XOR swap https://en.wikipedia.org/wiki/XOR_swap_algorithm
        //  of the chars in the string
        // Using the inbuilt swap is more elegant since we can interface an object type into this sort if needed
        swap(items[end], items[swapIndex]);
    }
}


// Sorts the items in ascending order in O(n^2) time 
// Finds the min of the remaining items and swaps it to the end of the remaining items
int selectionSort(string* items, int length=DEVILS_NUMBER) {
    int comparisonCounter = 0;
    int minIndex;
    for(int i = 0; i < length; i++){
        minIndex = i;
        // finds the min index from i to length - 
        for(int j = i + 1; j < length; j++){
            comparisonCounter++;
            if(items[j] < items[minIndex]){
                minIndex = j;
            }
        }
        if(minIndex == i){ continue; }
        swap(items[i], items[minIndex]);
        // items 0 to i is now sorted
    }

    return comparisonCounter;
}

// Sorts the items in ascending order in O(n^2) time
int insertionSort(string* items, int length=DEVILS_NUMBER){
    int comparisonCounter = 0;
    string selectedItem;
    int prevIndex;
    for(int i=1; i < length; i++){
        selectedItem = items[i];
        prevIndex = i - 1;
        // finding the correct index to insert into array
        while(prevIndex >= 0) {
            // used to have this check reversed as a condition for the while
            //  put here just to ensure that comparisons are counted only for comparison between elements
            comparisonCounter++;
            if(items[prevIndex] <= selectedItem){
                break;
            }
            items[prevIndex + 1] = items[prevIndex];
            prevIndex--;
        }
        items[prevIndex + 1] = selectedItem;
        // items 0 to i are relatively in order that is items0 < items1...  < itemsi
        //   0 to i might not be in the right order for the whole list
    }

    return comparisonCounter;
}

// Merge sort functions to sort in ascending order in O(nlogn) time


// helper to merge two sorted arrays
// I know that if I have an argument for the original array I am be able
//    to make less memory allocations possibly, but I wanted this to be a standalone function
pair<string*, int> mergeSorted(string* items1, string* items2, int size1, int size2){
    int count = 0;
    int mergedLength = size1 + size2;
    string* mergedItems = new string[mergedLength];
    int i1 = 0;
    string item1;
    int i2 = 0;
    string item2;
    for(int iMerged = 0; iMerged < mergedLength; iMerged++){
        // check for out of bounds
        if(i1 == size1){
            mergedItems[iMerged] = items2[i2];
            i2++;
            continue;
        }
        if(i2 == size2){
            mergedItems[iMerged] = items1[i1];
            i1++;
            continue;
        }

        // compare items to find min of the two and place in new array
        //  only iterates the the array index of the one that is added
        item1 = items1[i1];
        item2 = items2[i2];
        count++;
        if(item1 < item2){
            mergedItems[iMerged] = item1;
            i1++;
        } else {
            mergedItems[iMerged] = item2;
            i2++;
        }
    }
    return make_pair(mergedItems, count);
}

// Sort which works in O(n*logn) by splitting the array until length 0 or one and then reemerging the smaller sorted arrays
void mergeSort(string* items, int* count, int istart=0, int iend=DEVILS_NUMBER-1){
    // base case as we have reached our sorted array of at 1 or 0 elements!
    if (istart >= iend){
        return;
    }

    int imiddle = istart + (iend - istart) / 2;
    int leftLength = imiddle - istart + 1;
    string left[leftLength];
    int rightLength = iend - imiddle; 
    string right[rightLength];
    // recursively call mergeSort to ensure that we are using the sorted array when we merge
    //   with the two other half of the array
    mergeSort(items, count, istart, imiddle);
    mergeSort(items, count, imiddle + 1, iend);


    // populating left and right sub arrays to be merged
    for(int i = 0; i < leftLength; i++){
        left[i] = items[istart + i];
    }
    for(int i = 0; i < rightLength; i++){
        right[i] = items[imiddle + 1 + i];
    }
    
    auto mergedResult = mergeSorted(left, right, leftLength, rightLength); 
    string* mergedArray = mergedResult.first;

    // this is the syntax to increment the value of the pointer
    (*count) += mergedResult.second;
    int mergedLength = leftLength + rightLength;

    for(int imerged = 0; imerged < mergedLength; imerged++){
        items[istart + imerged] = mergedArray[imerged];
    }
    delete[] mergedArray;
}

// Essentially a sort of three items returning the index of the middle value's index
int getiMiddleOfThree(string* items, int istart, int iend, int* counter){
    int iMedianOfThree;
    int imiddle = (istart + iend) / 2;
    // partial ordering shown after each comparison
    (*counter)++;
    if(items[istart] < items[iend]){
        // istart, iend
        (*counter)++;
        if (items[istart] >= items[imiddle]){
            // imiddle, istart, iend
            iMedianOfThree = istart;
        } else if (items[iend] < items[imiddle]){
            (*counter)++;
            // istart, iend, imiddle
            iMedianOfThree = iend;
        } else {
            (*counter)++;
            // istart, imiddle, iend
            iMedianOfThree = imiddle;
        }
    } else {
        // iend, istart
        (*counter)++;
        if (items[istart] < items[imiddle]){
            // iend, istart, imiddle
            iMedianOfThree = istart;
        } else if (items[iend] > items[imiddle]) {
            (*counter)++;
            // imiddle, iend, istart
            iMedianOfThree = iend;
        } else {
            (*counter)++;
            // iend, imiddle, istart
            iMedianOfThree = imiddle;
        }
    }

    return iMedianOfThree;
}

// places all elements to the right or left of the pivot depending on if they are greater
int partition(string* items, int* counter, int istart, int iend){
    int ipivot = getiMiddleOfThree(items, istart, iend, counter);
    string pivotVal = items[ipivot];

    // arbitrarily Swapping pivot to end to ignore it when swapping around pivot value
    swap(items[ipivot], items[iend]);

    // index to put the lesser elements compared to the pivot
    int ilesser = istart - 1;

    for(int i = istart; i < iend; i++){
        (*counter)++;
        if(items[i] <= pivotVal){
            // preincrement since we start at istart - 1
            ilesser++;
            swap(items[i], items[ilesser]);
        }
        // do nothing if element is already less than pivot since it is in the correct place
    }

    // swapping pivot back to correct spot
    swap(items[ilesser + 1], items[iend]);
    return ilesser + 1;
}


// Sort which works in O(n*logn) [Since it should not degrade] by splitting the array and sorting the item relative to a pivot value
void quickSort(string* items, int* counter, int istart=0, int iend=DEVILS_NUMBER-1){
    if (istart >= iend){
        return;
    }

    int ipivot = partition(items, counter, istart, iend);

    // Recursive calls for all elements around the pivot point
    quickSort(items, counter, istart, ipivot-1);
    quickSort(items, counter, ipivot+1, iend);
}

// helper function to display only part of the list
void cTruncatedItems(string* magicItems, int count=10, int length=666){
    for(int i=0; i < min(count, length); i++){
        cout << magicItems[i] << endl;
    }
    if(length > count){
        cout << "..." << endl;
    }
}

// Run tests
int main(){
    // display for node, stack, and queue
    cout << "Testing node class by implementing a stack and a queue and stack..." << endl << endl;
    cout << "Enqueuing 1-10 and dequeuing x2:" << endl;
    testQueue();
    cout << endl;
    cout << "Pushing 1-10 and popping x2:" << endl;
    testStack();
    cout << endl;
    string* magicItems = getMagicItems();

    // Display for palindromes
    cout << "Showing all 12 of the palindromic magic items: " << endl;
    cpalindromes(magicItems);
    cout << endl;

    // prep for sort
    cout << endl << "Shuffling the list to prepare for sorting..." << endl << endl;
    fisherYatesShuffle(magicItems);


    // Display for selection sort
    int selectionSortCounter = selectionSort(magicItems);
    cout << "Showing first " << truncationCount << " sorted items by selection sort. Entire list sorted in ";
    cout << selectionSortCounter << " comparisons:" << endl;
    cTruncatedItems(magicItems, truncationCount);
    cout << endl << "Shuffling the list..." << endl << endl;
    fisherYatesShuffle(magicItems);

    // Display for insertion sort
    int insertionSortCounter = insertionSort(magicItems);
    cout << "Showing first " << truncationCount << " sorted items by insertion sort. Entire list sorted in ";
    cout << insertionSortCounter << " comparisons:" << endl;
    cTruncatedItems(magicItems, truncationCount);
    cout << endl << "Shuffling the list..." << endl << endl;
    fisherYatesShuffle(magicItems);

    // Display for mergesort
    int mergeSortCounter = 0;
    mergeSort(magicItems, &mergeSortCounter);
    cout << "Showing first " << truncationCount << " sorted items by mergesort. Entire list sorted in ";
    cout << mergeSortCounter << " comparisons:" << endl;
    cTruncatedItems(magicItems, truncationCount);
    cout << endl << "Shuffling the list..." << endl << endl;
    fisherYatesShuffle(magicItems);
    

    // Display for quicksort
    int quickSortCounter = 0;
    quickSort(magicItems, &quickSortCounter);
    cout << "Showing first " << truncationCount << " sorted items by quicksort. Entire list sorted in ";
    cout << quickSortCounter << " comparisons:" << endl;
    cTruncatedItems(magicItems, truncationCount);
    return 0;
}
