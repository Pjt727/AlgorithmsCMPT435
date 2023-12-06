/*
    Author: Patrick Tyler

    Compiled with g++:
        ex: `g++ ./main.cpp`
        g++.exe (Rev6, Built by MSYS2 project) 13.1.0
        Copyright (C) 2023 Free Software Foundation, Inc.

    Special Thanks: 
        The 3rd floor Handcock Atrium snacks, my parents, family, friends, and of course
        my professor and mentor Alan.
        Without them I dont know how I would have gotten through this assignment. 
            (or gotten assigned it in the first place)
    
    Date Due: 10/27/2023
*/
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include "oldcode.h"
#include<unordered_map>
#include<tuple>

using namespace std;

struct Vertex{
    string id;
    vector<pair<Vertex*, int>> neighbors;
};


class Graph {
    private:
        int nodeCount = 0;
        bool wasProcessed = false;

        unordered_map<string, Vertex*> keysToVertex;
        Vertex* origin = nullptr;
        vector<Vertex*>* vertices = new vector<Vertex*>;

    public:
        // on delete of instance
        ~Graph() {
            for(auto vertex : (*this->vertices )){
                delete vertex;
            }
            delete vertices;
        }

        void readCommands(string filepath = "./graphs2.txt"){
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
                            this->addVertex(&buffer);
                            context = NONE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        break;
                    case EDGE:
                        // can support other end commands like ; for instance or end of file
                        if(ch == '\n' || !isCharLeft){
                            auto keys = split_keys(&buffer);
                            auto edge1 = get<0>(keys);
                            auto edge2 = get<1>(keys);
                            auto weight = get<2>(keys);
                            this->addEdge(&edge1, &edge2, weight);
                            context = NONE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        buffer += ch;
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
                } else if(buffer == "new"){ 
                    context = NEW;
                    buffer = "";
                } else if (buffer == "add"){ 
                    context = ADD;
                    buffer = "";
                }
            }

            graphCommandStream.close();
        }

        // skips spaces, key2 starts after -
        tuple<string, string, int> split_keys(string* input, char deliminator = '-'){
            string key1 = "";
            string key2 = "";
            string str_weight = "";
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
            while(input->at(runningIndex) == ' '){
                runningIndex += 1;
            }

            bool keyAdded = false;
            for(int* i = &runningIndex; (*i) < inputSize; (*i)++){
                c = input->at((*i));
                if(c == ' '){ 
                    if(keyAdded) {
                        break;
                    }
                    continue;
                }
                key2 += c;
                keyAdded = true;
            }

            for(int* i = &runningIndex; (*i) < inputSize; (*i)++) {
                c = input->at((*i));
                str_weight += c;
            }
            int weight = stoi(str_weight);
            return make_tuple(key1, key2, weight);
        }

        void display(){
            cBellmanFordPath();
            cout << endl << endl;
        }

        void newGraph(){
            if(this->wasProcessed){
                this->display();

                // need to manually delete vertices bc they are pointers
                //      rust wouldn't let you make compile a memory mistake here
                for(auto vertex : (*this->vertices )){
                    delete vertex;
                }
                this->vertices->clear();
                this->keysToVertex.clear();
                this->origin = nullptr;
                this->nodeCount = 0;
            }

            this->wasProcessed = true;
        }

        // Only need to add to the maps
        void addVertex(string* key){
            Vertex* newVertex = new Vertex;
            newVertex->id = (*key);
            newVertex->neighbors = (*new vector<pair<Vertex*, int>>);
            if( origin == nullptr){
                origin = newVertex;
            }
            vertices->push_back(newVertex);
            keysToVertex.insert({(*key), newVertex});
            nodeCount++;
        }

        // Add a directed link between keys
        void addEdge(string* key1, string* key2, int weight){
            // Getting Vectors for keys
            Vertex* vectorKey1 = keysToVertex[(*key1)];
            Vertex* vectorKey2 = keysToVertex[(*key2)];

            vectorKey1->neighbors.push_back(make_pair(vectorKey2, weight));
        }

        // bellman ford algorithm
        void cBellmanFordPath() {
            int vertexCardinality = this->vertices->size();

            // mapping distance and previous
            unordered_map<Vertex*, int> distanceFromOrigin;
            unordered_map<Vertex*, Vertex*> predecessors;
            for(auto vertex : *this->vertices) {
                distanceFromOrigin.insert({vertex, (vertex == origin) ? 0 : INT_MAX});
                predecessors.insert({vertex, nullptr});
            }

            // Relax every edge of every node vertex cardinality times
            //   to propagate path values across the entire graph
            for(int i=1; i < vertexCardinality; i++){
                for(auto& sourceVertex : *this->vertices ){
                    for(auto& toEdge : sourceVertex->neighbors) {
                        auto& destinationVertex = toEdge.first;
                        auto weight = toEdge.second;
                        // relax 
                        if(distanceFromOrigin[sourceVertex] == INT_MAX) {
                            continue;
                        }
                        int weightFromSource = distanceFromOrigin[sourceVertex] + weight;
                        if( weightFromSource < distanceFromOrigin[destinationVertex]){
                            distanceFromOrigin[destinationVertex] = weightFromSource;
                            predecessors[destinationVertex] = sourceVertex;
                        }
                    }
                }
            }

            // check for negative cycles which would create indefinite answers
            for(auto& sourceVertex : *this->vertices ){
                for(auto& toEdge : sourceVertex->neighbors) {
                    auto& destinationVertex = toEdge.first;
                    auto weight = toEdge.second;
                    // relax 
                    if(distanceFromOrigin[sourceVertex] == INT_MAX) {
                        continue;
                    }
                    int weightFromSource = distanceFromOrigin[sourceVertex] + weight;
                    if( weightFromSource < distanceFromOrigin[destinationVertex]){
                        // there is a negative weight cycle
                        cout << "This graph has a negative weight cycle" << endl;
                        return;
                    }
                }
            }

            // See the results (I am too lazy to write the tedious types 
            //    for doing this in a different function)
            for(auto& vertex : *this->vertices) {
                if(vertex == this->origin) { continue; }
                cout << this->origin->id << " -> " << vertex->id << " is ";
                cout << distanceFromOrigin[vertex] << "; path is ";
                auto pathToVertex = new Stack<Vertex*>();
                auto runningVertex = vertex;
                while(runningVertex != nullptr) {
                    pathToVertex->push(runningVertex);
                    runningVertex = predecessors[runningVertex];
                }
                runningVertex = pathToVertex->pop();
                cout << runningVertex->id;
                while(!pathToVertex->isEmpty()) {
                    runningVertex = pathToVertex->pop();
                    cout << " -> " << runningVertex->id;
                }
                cout << endl;
            }
        }


};



int main() {
    auto graph = new Graph();
    graph->readCommands();
}
