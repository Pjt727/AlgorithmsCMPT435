/*
    Author: Patrick Tyler

    Compiled with g++:
        g++.exe (Rev6, Built by MSYS2 project) 13.1.0
        Copyright (C) 2023 Free Software Foundation, Inc.

    Special Thanks: 
        My C++ debugger, my desk rubber duck, my parents, family, friends, and of course
        my professor and mentor Alan.
        Without them I dont know how I would have gotten through this assignment. 
            (or gotten assigned it in the first place)

    Date Due: 10/27/2023
*/
using namespace std;
#include<iostream>
#include<fstream>
#include <iomanip>
#include <cmath>
#include "oldcode.h"
#include<cstdlib>

// Dirty dirty constant
const int NUM_SEARCHES = 42; // Amount of linear and binary searches


// helper round function for base 10 numbers
// expected precision to be 10^precision digit
// ex: 92.47 -> 92.5 is round on the -1 precision
double round_prec(double num, int precision) {
    num /= pow(10, precision);
    num = round(num);
    num *= pow(10, precision);
    return num;
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
        comparisonCount++;
        if(items[imiddle] == item) {
            break;
        }
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

// Javascript frontend developers would need to npm install a package for this LOL
// https://qz.com/646467/how-one-programmer-broke-the-internet-by-deleting-a-tiny-piece-of-code 
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
    double average = round_prec(double(totalCount) / NUM_SEARCHES, -2);
    cout << "The average amount of comparisons is: " << average << endl;;
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
    double average = round_prec(double(totalCount) / NUM_SEARCHES, -2);
    cout << "The average amount of comparisons is: " << average << endl << endl;
}


// Only a string hash table bc the hashing function will only work for strings
class HashTable {
    private:
        // Pointer to array of Nodes
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
                // independent of casing
                c = std::toupper(c);
            }
            // ascii sum of characters
            int asciiTotal = 0;
            for(char c: value){
                asciiTotal += toascii(c);
            }
            
            // multiply the asciiTotal by a large prime number to
            //  spread the ascii totals 
            return (asciiTotal * hashPrime) % hashSize;
        }

        void add(string value){
            // create new node
            auto newNode = new Node<string>;
            newNode->data = value;
            newNode->next = nullptr;

            // hash node value to find which tail to add it to
            int hashValue = hash(value);
            Node<string>*& tail = tails[hashValue];
            if(tail == nullptr){
                // case this hash value has no elements in it
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
            //  it feels weird if it were to return 0 even though the program checked head
            if(traversingNode == nullptr) { return comparisonCount; }

            while((traversingNode->data != value)){
                traversingNode = traversingNode->next;
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
        cout << rightPad(currentCount, 2) << " comparisons to find " << searchItems[i] << endl;
        totalCount += currentCount;
    }
    cout << endl;
    double average = round_prec(double(totalCount) / NUM_SEARCHES, -2);
    cout << "The average amount of comparisons is: " << average << endl;;
}


int main(){
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
