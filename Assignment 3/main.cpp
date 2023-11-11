
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
#include<unordered_map>

using namespace std;

template <typename T>
struct Node{
    T data;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
};

struct Vector{
    string* id;
    vector<Vector*> neighbors;
};

class Graph {
    private:
        int node_count = 0;
        bool was_processed = false;
        unordered_map<string, int> keys_to_index;
        vector<vector<bool>>* matrix;
        vector<string>* adjacency_list;

        unordered_map<string, Vector*> keys_to_vector;
        Vector* origin = nullptr;

    public:
        // on delete of instance
        ~Graph() {
            delete matrix;
            delete adjacency_list;
        }

        void read_commands(string filepath = "./graphs1.txt"){
            ifstream graphCommandStream("magicitems.txt");
            
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
                            this->new_graph();
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
                            this->add_node(&buffer);
                            context = NONE;
                            buffer = "";
                            goto NEXT_CHAR;
                        }
                        break;
                    case EDGE:
                        // can support other end commands like ; for instance
                        if(ch == '\n'){
                            auto keys = split_keys(&buffer);
                            this->add_edge(&keys.first, &keys.second);
                            delete &keys;
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
                    context = NONE;
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

                buffer += ch;
            }
            // Because buffer is passed to other functions I think I need to delete it 
            delete &buffer;
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
                if(c == deliminator){ continue; }
                key1 += c;
            }
            for(int* i = &runningIndex; (*i) < inputSize; (*i)++){
                c = input->at((*i));
                if(c == ' '){ continue; }
                if(c == deliminator){ continue; }
                key2 += c;
            }

            return make_pair(key1, key2);
        }

        void new_graph(){

        }

        // Only need to add to the maps
        void add_node(string* key){
            Vector* newVector = new Vector;
            newVector->id = key;
            if( origin == nullptr){
                origin = newVector;
            }

            keys_to_index.insert({(*key), node_count});
            keys_to_vector.insert({(*key), newVector});

            node_count++;
        }

        // Add an undirected link between keys
        void add_edge(string* key1, string* key2){
            // Getting indexes for keys
            int indexKey1 = keys_to_index[(*key1)];
            int indexKey2 = keys_to_index[(*key2)];

            // Adding edge on matrix
            matrix[indexKey1][indexKey2].push_back(true);
            matrix[indexKey2][indexKey1].push_back(true);

            // adding edge on adjacency list
            adjacency_list[indexKey1].push_back((*key2));
            adjacency_list[indexKey2].push_back((*key1));

            // Getting Vectors for keys
            Vector* vectorKey1 = keys_to_vector[(*key1)];
            Vector* vectorKey2 = keys_to_vector[(*key2)];
            vectorKey1->neighbors.push_back(vectorKey2);
            vectorKey2->neighbors.push_back(vectorKey1);
        }
};

void testGraph(){
    auto graph = new Graph();
}

int main(){
    testGraph();
}