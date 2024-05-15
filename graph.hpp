#pragma once
#include <string>

using namespace std;

struct Edge {
    int vertix1;
    int vertix2;
};

struct Node {
    int vertix;
    Node *next;
};

struct NodeMap {
    int key;
    int value;
};

class Graph {
    public:
        Graph() = default; // construtor vazio
        Graph(string filePath, bool directGraph); // construtor recebendo arquivo
        Graph(Graph& other); // construtor de copia
        ~Graph(); // destrutor
        void printAdjacencyMatrix();
        void printAdjacencyList();
        void printFormatedData();
        void removeEdge(Edge edge);
        void addEdge(Edge edge);
        void removeVertix(int vertix);
        void addVertix(int vertix);
        bool hasEdge(Edge edge);
        bool hasVertix(int vertix);
        bool isDirected();
        int getNumVertix();
        int vertixAt(int index);
        int getNumEdges();
        Edge edgeAt(int index);
        int* getAloneVertixList();
        int getNumAloneVertix();

    private:
        Edge *edgeList = nullptr;
        int numEdges = 0;
        int *vertixList = nullptr;
        int numVertix = 0;
        bool directGraph = false;
        int **adjacencyMatrix = nullptr;
        Node **adjacencyList = nullptr; 
        int *vertixAloneList = nullptr;
        int numAloneVertix = 0;

        void updateAdjacencyMatrix();
        void updateAdjacencyList();
        void updateVertixAloneList();
        void deleteGraph();
};

class Map {
    public:
        Map(int* list, int length);
        int getPositionOf(int value);

    private:
        NodeMap *mapList = nullptr;
        int currentkey = 0;
};