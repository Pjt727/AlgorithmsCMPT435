
/*
    Author: Patrick Tyler

    Compiled with g++:
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
    Node<T>* left;
    Node<T>* right;
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

            enum CommandContext { NEW, ADD, VERTEX, EDGE, COMMENT, NONE };
            CommandContext context = NONE;
            char ch;

            // Reading line by line is for people with a life
            //    This would read over reach line at least twice

            // GOTO are used bc of switch that is nested (even when continues would work for consistency)
            NEXT_CHAR:while(graphCommandStream >> noskipws >> ch){
                if(context == COMMENT){
                    if(ch == '\n'){
                        context = NONE;
                        // buffer will always "" in sample graphs text but this seems reasonable
                        buffer = "";
                    }
                    goto NEXT_CHAR;
                }

                // context actions
                switch(context){
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
                        // can support other end commands like ; for instance
                        if(ch == '\n'){
                            this->addNode(&buffer);
                            context = NONE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        break;
                    case EDGE:
                        // can support other end commands like ; for instance
                        if(ch == '\n'){
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
                    context = COMMENT;
                    goto NEXT_CHAR;
                }
                // will usually short circuit next two checks
                if(buffer == "new" && context == NONE){ 

                    context = NEW;
                    buffer = "";
                    goto NEXT_CHAR;
                }
                if(buffer == "add" && context == NONE){ 
                    context = ADD;
                    buffer = "";
                    goto NEXT_CHAR;
                }
                if(ch != '\n'){
                    buffer += ch;
                }
            }
            // Because buffer is passed to other functions I think I need to delete it 
            // delete &buffer;
            graphCommandStream.close();
        }

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
            cout << "The path of DFS search starting at the first inserting vertex is:" << endl;
            cout << dfsPath << endl << endl;

            cout << "The path of BFS search starting at the first inserting vertex is:" << endl;
            string bfsPath = getBfsPath(this->origin);
            cout << bfsPath << endl;
        }

        // Ideally traversal specific data should not be imbedded in the objects themselves
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
            return runningPath;
        }

        void newGraph(){
            if(this->wasProcessed){
                this->display();

                // default settings
                this->keysToIndex.clear();
                this->keysToVector.clear(); // need to manually delete Vectors objects
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


void testGraph(){
    auto graph = new Graph();
    graph->readCommands();

    // display the last one graph
    graph->display();
}

int main(){
    testGraph();
}