
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
    vector<Vector> neighbors;
};

class Graph {
    private:
        int node_count = 0;
        unordered_map<string*, int> keys_to_index;
        vector<vector<bool>>* matrix = new vector<vector<bool>>;
        vector<vector<string*>>* adjacency_list = new vector<vector<string*>>;

        unordered_map<string*, Vector*> keys_to_vector;
        Vector* origin = nullptr;

    public:
        void read_commands(string filepath = './graphs1.txt'){
            ifstream graphCommandStream("magicitems.txt");
            
            string line;
            while(getline(graphCommandStream, line)){
                string buffer = "";


            }
            graphCommandStream.close();
        }

        // Only need to add to the maps
        void add_node(string* key){
            Vector* newVector = new Vector;
            newVector->id = key;
            if( origin == nullptr){
                origin = newVector;
            }

            keys_to_index.insert({key, node_count});
            keys_to_vector.insert({key, newVector});

            node_count++;
        }

        // Add an undirected link between keys
        void add_edge(string* key1, string* key2){
            int indexKey1 = keys_to_index[key1];
            int indexKey2 = keys_to_index[key2];

            Vector* vectorKey1 = keys_to_vector[key1];
            Vector* vectorKey2 = keys_to_vector[key2];
            
            matrix[indexKey1][indexKey2] = true;
            matrix[indexKey2][indexKey1]  = true;

            adjacency_list[indexKey1].push_back(key2);
            adjacency_list[indexKey2].push_back(key1);
        }
};
