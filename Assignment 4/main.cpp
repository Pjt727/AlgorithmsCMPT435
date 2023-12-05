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
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include "oldcode.h"
#include<unordered_map>

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
            newVertex->neighbors = (*new vector<Vertex*>);
            if( origin == nullptr){
                origin = newVertex;
            }
            vertices->push_back(newVertex);
            keysToVertex.insert({(*key), newVertex});
            nodeCount++;
        }

        // Add a directed link between keys
        void addEdge(string* key1, string* key2){
            // Getting Vectors for keys
            Vertex* vectorKey1 = keysToVertex[(*key1)];
            Vertex* vectorKey2 = keysToVertex[(*key2)];

            vectorKey1->neighbors.push_back(vectorKey2);
        }
};
