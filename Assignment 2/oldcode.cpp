/*
    Author: Patrick Tyler

    Compiled with g++:
        g++.exe (Rev6, Built by MSYS2 project) 13.1.0
        Copyright (C) 2023 Free Software Foundation, Inc.

    Special Thanks: 
        My parents, my family, and friends. As well as Dr. Labouseur (of course).
        Without them I dont know how I would have gotten through this assignment.

    Date Due: 10/27/2023
*/

#include<iostream>
#include<fstream>
#include <iomanip>
using namespace std;

// Dirty dirty constant
const int DEVILS_NUMBER = 666; // "funny" name for length of items file 
const int NUM_SEARCHES = 42; // Amount of linear and binary searches

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

// linear search in linear time complexity, funny how that works out
int getLinearSearchComparisons(string* items, string item, int length=DEVILS_NUMBER) {
    // could just use one variable for count and i
    //  index plus 1 could be count
    //  instead of having two variables
    // using two for clarity
    int comparisonCount = 0;
    for(int i=0; i < length; i++){
        comparisonCount++;
        if(items[i] == item){
            break;
        }
    }
    return comparisonCount;
}

// binary search in nlogn time complexity
int getBinarySearchComparisons(string* items, string item, int length=DEVILS_NUMBER) {
    int comparisonCount = 0;
    int istart = 0;
    int iend = length - 1;
    int imiddle = (istart + iend) / 2; 
    while(istart <= iend) {
        comparisonCount++; // Does this count as a comparison?
        if(items[imiddle] == item) {
            break;
        }
        comparisonCount++;
        if(items[imiddle] > item) {
            // item is to the left of the middle value
            iend = imiddle - 1;
        } else {
            // item is to the right of the middle value
            istart = imiddle + 1;
        }
        imiddle = (istart + iend) / 2; 
    }

    return comparisonCount;
}

void cAllLinearSearchComparions(string* items, string* searchItems, 
    int itemsLength=DEVILS_NUMBER, int searchItemsLength=NUM_SEARCHES){
        cout << "Showing linear search comparison count for " << NUM_SEARCHES << " items." << endl;

        int totalCount = 0;
        for(int i=0; i < searchItemsLength; i++){
            int currentCount = getLinearSearchComparisons(items, searchItems[i], itemsLength);
            cout << searchItems[i] << " took " << currentCount << " comparisons to find." << endl;
            totalCount += currentCount;
        }
        cout << endl;
        cout << "The average amount of comparisons is: " << double(totalCount) / NUM_SEARCHES << endl;;
}

void cAllBinarySearchComparions(string* items, string* searchItems, 
    int itemsLength=DEVILS_NUMBER, int searchItemsLength=NUM_SEARCHES){
    
    cout << "Showing binary comparison count for " << NUM_SEARCHES << " items." << endl;

    int totalCount = 0;
    for(int i=0; i < searchItemsLength; i++){
        int currentCount = getBinarySearchComparisons(items, searchItems[i], itemsLength);
        cout << searchItems[i] << " took " << currentCount << " comparisons to find." << endl;
        totalCount += currentCount;
    }
    cout << endl;
    cout << "The average amount of comparisons is: " << double(totalCount) / NUM_SEARCHES << endl;;
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

int main(){
    setprecision(2);
    string* magicItems = getMagicItems();

    string* searchItems = new string[NUM_SEARCHES];
    fisherYatesShuffle(magicItems);
    for(int i=0; i < NUM_SEARCHES; i++){
        searchItems[i] = magicItems[i];
    }


    selectionSort(magicItems);
    cAllBinarySearchComparions(magicItems, searchItems);
    cout << endl;
    cAllLinearSearchComparions(magicItems, searchItems);
}

