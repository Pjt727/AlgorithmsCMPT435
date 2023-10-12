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
using namespace std;
#include<iostream>
#include<fstream>
#include <iomanip>
#include "oldcode.h"
#include<cstdlib>

// Dirty dirty constant
const int NUM_SEARCHES = 42; // Amount of linear and binary searches

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
        // comparisonCount++; // Does this count as a comparison?
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

string rightPad(int num, int size){
    string padded = to_string(num);
    int curLength = padded.length();
    while(curLength < size){
        padded += " ";
        curLength++;
    }
    return padded;
}

void cAllLinearSearchComparions(string* items, string* searchItems, 
    int itemsLength=DEVILS_NUMBER, int searchItemsLength=NUM_SEARCHES){
    cout << "Showing linear search comparison count for " << NUM_SEARCHES << " items." << endl;

    int totalCount = 0;
    for(int i=0; i < searchItemsLength; i++){
        int currentCount = getLinearSearchComparisons(items, searchItems[i], itemsLength);
        cout << rightPad(currentCount, 3) << " comparisons to find " << searchItems[i] << endl;
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
        cout << rightPad(currentCount, 2) << " comparisons to find " << searchItems[i] << endl;
        totalCount += currentCount;
    }
    cout << endl;
    cout << "The average amount of comparisons is: " << double(totalCount) / NUM_SEARCHES << endl;;
}


// Only a string hash table bc the hashing function will other work for strings
class HashTable {
    private:
        Node<string>** heads;
        Node<string>** tails;
        int hashSize;
        int hashPrime;

    public:
        HashTable(int hashSize=250, int hashPrime=100003){
            this->hashSize = hashSize;
            this->hashPrime = hashPrime;
            heads = new Node<string>*[hashSize];
            tails = new Node<string>*[hashSize];
            for(int i=0; i < hashSize; i++){
                heads[i] = nullptr;
                tails[i] = nullptr;
            }
        }

        int hash(string value){
            for (char& c : value) {
                c = std::toupper(c);
            }
            int asciiTotal = 0;
            for(char c: value){
                asciiTotal += toascii(c);
            }
            
            // multiply the ascciTotal by a large prime number to
            //  spread the ascii totals 
            return (asciiTotal * hashPrime) % hashSize;
        }

        void add(string value){
            auto newNode = new Node<string>;
            newNode->data = value;
            newNode->next = nullptr;

            int hashValue = hash(value);
            Node<string>*& tail = tails[hashValue];
            if(tail == nullptr){
                Node<string>*& head = heads[hashValue];
                head = newNode;
                tail = newNode;
            } else{
                tail->next = newNode;
                tail = newNode;
            }
        }

        int getComparisons(string value){
            int hashValue = hash(value);
            
            int comparisonCount = 1;
            Node<string>*& traversingNode = heads[hashValue];
            // although i'm not counting this comparison for elements that are here
            //  I feel weird if I were to return 0 even though I check head
            if(traversingNode == nullptr) { return comparisonCount; }

            while((traversingNode->data != value)){
                traversingNode = traversingNode->next;
                // Instead of this "comparisons" I could use a try catch
                //  to handle the case where the program would try to access
                //  the data attribute of a nullptr
                if(traversingNode == nullptr) { break; }
                comparisonCount++;
            }

            return comparisonCount;
        }

        int getHashSize(){
            return this->hashSize;
        }
};

void cAllHashTableComparisons(HashTable* hashTable, string* searchItems, int searchItemsLength=NUM_SEARCHES){
    cout << "Showing chained hash table of size " << hashTable->getHashSize() << " search comparison count for ";
    cout << NUM_SEARCHES << " items." << endl;

    int totalCount = 0;
    for(int i=0; i < searchItemsLength; i++){
        int currentCount = hashTable->getComparisons(searchItems[i]);
        cout << rightPad(currentCount, 3) << " comparisons to find " << searchItems[i] << endl;
        totalCount += currentCount;
    }
    cout << endl;
    cout << "The average amount of comparisons is: " << double(totalCount) / NUM_SEARCHES << endl;;

}


int main(){
    setprecision(2);
    string* magicItems = getMagicItems();

    string* searchItems = new string[NUM_SEARCHES];
    fisherYatesShuffle(magicItems);
    for(int i=0; i < NUM_SEARCHES; i++){
        searchItems[i] = magicItems[i];
    }

    // Prep for search comparisons
    selectionSort(magicItems);

    // search comparisons with linear and binary methods
    cAllLinearSearchComparions(magicItems, searchItems);
    cout << endl;
    cAllBinarySearchComparions(magicItems, searchItems);

    // search comparisons using hastTable
    auto hashTable = new HashTable();
    for(int i=0; i < DEVILS_NUMBER; i++){
        hashTable->add(magicItems[i]);
    }
    cAllHashTableComparisons(hashTable, searchItems);
}
