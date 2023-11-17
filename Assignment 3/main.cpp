
/*
    Author: Patrick Tyler

    Compiled with g++:
        ex: `g++ ./main.cpp`
        g++.exe (Rev6, Built by MSYS2 project) 13.1.0
        Copyright (C) 2023 Free Software Foundation, Inc.

    Special Thanks: 
        The 3rd floor Handcock Atrium, my parents, family, friends, and of course
        my professor and mentor Alan.
        Without them I dont know how I would have gotten through this assignment. 
            (or gotten assigned it in the first place)
    
    Date Due: 10/27/2023
*/
#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include "oldcode.h"
#include<unordered_map>

using namespace std;

template <typename T>
struct BinNode{
    T data;
    BinNode<T>* left;
    BinNode<T>* right;
};


struct Vertex{
    string id;
    vector<Vertex*> neighbors;
};

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

class Graph {
    private:
        int nodeCount = 0;
        bool wasProcessed = false;
        unordered_map<string, int> keysToIndex;
        vector<vector<bool>>* matrix = new vector<vector<bool>>;
        vector<vector<string>>* adjacencyList = new vector<vector<string>>;

        unordered_map<string, Vertex*> keysToVector;
        Vertex* origin = nullptr;

    public:
        // on delete of instance
        ~Graph() {
            delete matrix;
            delete adjacencyList;
        }

        void readCommands(string filepath = "./graphs1.txt"){
            ifstream graphCommandStream(filepath);
            
            string buffer = "";

            enum CommandContext { NEW, ADD, VERTEX, EDGE, NONE };
            CommandContext context = NONE;
            char ch;

            // Reading line by line is for people with a life
            //    This would read over reach line at least twice

            // GOTO are used bc of switch that is nested (even when continues would work for consistency)
            bool isCharLeft = true;
            NEXT_CHAR:while(isCharLeft){
                //  because big mean Alan did not end of the file with a new line (which is best practice)
                //      I have to also check for end of file can process in that case as well
                if(!(graphCommandStream >> noskipws >> ch)){
                    isCharLeft = false;
                    // need to jump over the check that adds buffer
                    goto ALAN_IS_MEAN;
                }
                
                // only process command when command is finished on \n and whitespace
                if((ch != '\n' && ch != ' ')){
                    buffer += ch;
                    goto NEXT_CHAR;
                }

                // context actions
                ALAN_IS_MEAN:switch(context){
                    case NONE:
                        break;
                    case NEW:
                        if(buffer == "graph"){
                            this->newGraph();
                            context = NONE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        break;
                    case ADD:
                        if(buffer == "vertex"){
                            context = VERTEX;                
                            buffer = "";
                            goto NEXT_CHAR;
                        } else if(buffer == "edge"){
                            context = EDGE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        break;
                    case VERTEX:
                        // can support other end commands like ; for instance or end of file
                        if(ch == '\n' || !isCharLeft){
                            this->addNode(&buffer);
                            context = NONE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        break;
                    case EDGE:
                        // can support other end commands like ; for instance or end of file
                        if(ch == '\n' || !isCharLeft){
                            auto keys = split_keys(&buffer);
                            this->addEdge(&keys.first, &keys.second);
                            context = NONE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        break;
                }

                // setting context... Supports in line commenting
                if(buffer == "--"){
                    // line is commented
                    buffer = "";
                    // find end of line or file
                    while(graphCommandStream >> noskipws >> ch){
                        if(ch == '\n'){ break; }
                    }
                    goto NEXT_CHAR;
                }
                // will usually short circuit next two checks
                if(buffer == "new"){ 

                    context = NEW;
                    buffer = "";
                    goto NEXT_CHAR;
                }
                if(buffer == "add"){ 
                    context = ADD;
                    buffer = "";
                    goto NEXT_CHAR;
                }
            }

            graphCommandStream.close();
        }

        // skips spaces, key2 starts after -
        pair<string, string> split_keys(string* input, char deliminator = '-'){
            string key1 = "";
            string key2 = "";
            int inputSize = (*input).size();
            int runningIndex = 0;
            char c;
            for(int* i = &runningIndex; (*i) < inputSize; (*i)++){
                c = input->at((*i));
                if(c == ' '){ continue; }
                if(c == deliminator){ 
                    (*i)++;
                    break;
                }
                key1 += c;
            }
            for(int* i = &runningIndex; (*i) < inputSize; (*i)++){
                c = input->at((*i));
                if(c == ' '){ continue; }
                key2 += c;
            }

            return make_pair(key1, key2);
        }

        void display(){
            // find max length key for padding and init a seen hashmap for traversals
            int maxKeyLength = 0;
            for(auto& keyIndex : this->keysToIndex){
                if(keyIndex.first.length() > maxKeyLength){
                    maxKeyLength = keyIndex.first.length();
                }
            }
            // reverse map for keysToIndex
            unordered_map<int, string> indexToKeys;
            for( auto& pair : this->keysToIndex){
                indexToKeys.insert({pair.second, pair.first});
            }
            // displaying as matrix 
            cout << endl << "Showing the graph as a matrix: " << endl << endl;
            // header row
            const string COLUMN_SPACING = " ";
            for(int i = 0; i < maxKeyLength; i++){
                cout << " ";
            }
            cout << COLUMN_SPACING;
            for(int i = 0; i < this->matrix->size(); i++){
                cout << middlePad(indexToKeys[i], maxKeyLength);
                cout << COLUMN_SPACING;
            }
            cout << endl;
            // rest of rows
            string representation;
            for(int i = 0; i < this->matrix->size(); i++){
                cout << leftPad(indexToKeys[i], maxKeyLength) << COLUMN_SPACING;
                for(const bool& isOnNeighbor : (*this->matrix)[i]){
                    if(isOnNeighbor){
                        representation = "1";
                    } else {
                        representation = ".";
                    }
                    cout << middlePad(representation, maxKeyLength) << COLUMN_SPACING;
                }
                cout << endl;
            }

            // adjacency list

            cout << endl << "Showing the graph as an adjacency list: " << endl << endl;
            for(int i = 0; i < this->adjacencyList->size(); i++){
                cout << leftPad("[" + indexToKeys[i] + "]", maxKeyLength + 2) << COLUMN_SPACING << COLUMN_SPACING;
                for(auto& neighbor : (*this->adjacencyList)[i]){
                    cout << rightPad(neighbor, maxKeyLength) << COLUMN_SPACING;
                }
                cout << endl;
            }


            // Results of searches on object graph
            unordered_map<string, bool> isSeen;
            for(auto& keyIndex : this->keysToIndex){
                isSeen.insert({keyIndex.first, false});
            }
            string dfsPath = "";
            generateDfsPath(this->origin, &isSeen, &dfsPath);
            cout << endl << "The path of DFS starting at the first inserted vertex is:" << endl;
            cout << dfsPath << endl << endl;

            cout << "The path of BFS starting at the first inserted vertex is:" << endl;
            string bfsPath = getBfsPath(this->origin);
            cout << bfsPath << endl;
        }

        // Ideally traversal specific data should not be imbedded in the objects themselves
        //     Therefore maps are used to tell if a vertex has been visited
        void generateDfsPath(Vertex* curVertex, unordered_map<string, bool>* isSeen, string* runningPath){
            const string DELIMINATOR = " ";
            if(!(*isSeen)[curVertex->id]){
                (*runningPath) += curVertex->id + DELIMINATOR;
                (*isSeen)[curVertex->id] = true;
            }
            for(auto& neighbor : curVertex->neighbors){
                if(!(*isSeen)[neighbor->id]){
                    generateDfsPath(neighbor, isSeen, runningPath);
                }
            }
        }

        string getBfsPath(Vertex* startVertex){
            const string DELIMINATOR = " ";
            string runningPath = "";

            // Creating hashmap fro seen vertices 
            unordered_map<string, bool> isSeen;
            for(auto& keyIndex : this->keysToIndex){
                isSeen.insert({keyIndex.first, false});
            }

            // Traversing node in queue starting with the input
            //    and then enqueuing each neighbor
            Queue<Vertex*>* vertices = new Queue<Vertex*>();
            vertices->enqueue(startVertex);
            isSeen[startVertex->id] = true;
            while(!vertices->isEmpty()){
                auto curVertex = vertices->dequeue();
                runningPath += curVertex->id + DELIMINATOR;
                for(auto& neighbor : curVertex->neighbors){
                    if(!isSeen[neighbor->id]){
                        vertices->enqueue(neighbor);
                        isSeen[neighbor->id] = true;
                    }
                }
            }
            delete vertices;
            return runningPath;
        }

        void newGraph(){
            if(this->wasProcessed){
                this->display();

                // default settings
                this->keysToIndex.clear();

                // need to manually delete Vectors bc they are pointers
                //      rust wouldn't let you make compile a memory mistake here
                for(auto pair : keysToVector){
                    delete pair.second;
                }
                this->keysToVector.clear();
                this->matrix->clear();
                this->adjacencyList->clear();
                this->origin = nullptr;
                this->nodeCount = 0;
            }

            this->wasProcessed = true;
        }

        // Only need to add to the maps
        void addNode(string* key){
            Vertex* newVector = new Vertex;
            newVector->id = (*key);
            newVector->neighbors = (*new vector<Vertex*>);
            if( origin == nullptr){
                origin = newVector;
            }
            keysToIndex.insert({(*key), this->nodeCount});
            keysToVector.insert({(*key), newVector});
            nodeCount++;

            // Resize vectors to fit all nodes 
            adjacencyList->resize(nodeCount);
            matrix->resize(nodeCount);
            for(auto& neighbors : (*matrix)){
                neighbors.resize(nodeCount);
            }
        }

        // Add an undirected link between keys
        void addEdge(string* key1, string* key2){
            // Getting indexes for keys
            int indexKey1 = keysToIndex[(*key1)];
            int indexKey2 = keysToIndex[(*key2)];


            // Adding edge on matrix
            (*matrix)[indexKey1][indexKey2] = true;
            (*matrix)[indexKey2][indexKey1] = true;

            // adding edge on adjacency list
            (*adjacencyList)[indexKey1].push_back((*key2));
            (*adjacencyList)[indexKey2].push_back((*key1));

            // Getting Vectors for keys
            Vertex* vectorKey1 = keysToVector[(*key1)];
            Vertex* vectorKey2 = keysToVector[(*key2)];


            vectorKey1->neighbors.push_back(vectorKey2);
            vectorKey2->neighbors.push_back(vectorKey1);
        }
};

template <typename T>
class BinarySearchTree {
    private:
        BinNode<T>* head = nullptr;
    
    public:
        void cInsert(T data, int maxDataLength){
            string DELIMINATOR = " ";
            string runningPath = rightPad(data + ": ", maxDataLength + 2);
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
                    runningPath += "R" + DELIMINATOR;
                    consideredNode = consideredNode->right;
                } else {
                    runningPath += "L" + DELIMINATOR;
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

            cout << runningPath << endl;
        }

        int cSearch(T data, int maxDataLength){
            string DELIMINATOR = " ";
            string runningPath = rightPad(data + ": ", maxDataLength + 2);
            auto consideredNode = head;
            // start at one for the first while check
            int comparisonCount = 1;
            while(consideredNode != nullptr && consideredNode->data != data){
                comparisonCount++;
                if(data >= consideredNode->data){
                    runningPath += "R" + DELIMINATOR;
                    consideredNode = consideredNode->right;
                } else {
                    runningPath += "L" + DELIMINATOR;
                    consideredNode = consideredNode->left;
                }
            }

            cout << rightPad(to_string(comparisonCount), 2) << " comparisons for " << runningPath << endl;

            return comparisonCount;
        }

        void cInOrderTraversal(BinNode<T>* node){ 
            if(node == nullptr) { return; }
            cInOrderTraversal(node->left);
            cout << node->data << endl;
            cInOrderTraversal(node->right);
        }

        BinNode<T>* getHead(){
            return this->head;
        }
};


void testGraph(){
    auto graph = new Graph();
    graph->readCommands();

    // display the last one graph
    graph->display();
}

void testBST(){
    auto allMagicItems = getMagicItems("./magicitems.txt");
    auto magicItemsInBst = getMagicItems("./magicitems-find-in-bst.txt");
    auto bst = new BinarySearchTree<string>();
    
    // getting max length for the display to look better
    int allMaxLength = 0;
    int curLength;
    for(auto magicItem : (*allMagicItems)){
        curLength = magicItem.length();
        if(curLength > allMaxLength){
            allMaxLength = curLength;
        }
    }

    // adding all items to bst
    cout << "Showing insertions for all magic items" << endl << endl;
    for(auto magicItem : (*allMagicItems)){
        bst->cInsert(magicItem, allMaxLength);
    }

    // Pre-order traversal
    cout << endl << "Showing in-order traversal of magic items" << endl << endl;
    bst->cInOrderTraversal(bst->getHead());

    // getting max of other magic items for the display to look better
    int bstMaxLength = 0;
    for(auto magicItem : (*magicItemsInBst)){
        curLength = magicItem.length();
        if(curLength > bstMaxLength){
            bstMaxLength = curLength;
        }
    }

    float totalNum = 0;
    float runningSum = 0;
    cout << endl << "Showing searches for bst magic times" << endl << endl;
    for(auto magicItem : (*magicItemsInBst)){
        totalNum++;
        runningSum += bst->cSearch(magicItem, bstMaxLength);
    }
    cout << endl << "The average search number of comparisons is: "  << round_prec(runningSum / totalNum, -2) << endl;



}

int main(){
    testGraph();
    testBST();
}