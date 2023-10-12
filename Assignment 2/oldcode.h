#include<iostream>
#include<fstream>
#include <iomanip>
using namespace std;

const int DEVILS_NUMBER = 666;

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

// Wraps a generic, T, typed data in a node to reference other nodes
template <typename T>
struct Node{
    T data;
    Node<T>* next;
};

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
int selectionSort(string* items, int length = DEVILS_NUMBER) {
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

// helper function to display only part of the list
void cTruncatedItems(string* magicItems, int count=10, int length=666){
    for(int i=0; i < min(count, length); i++){
        cout << magicItems[i] << endl;
    }
    if(length > count){
        cout << "..." << endl;
    }
}