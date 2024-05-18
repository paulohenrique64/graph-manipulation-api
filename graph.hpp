#pragma once
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <fstream>
#include "list.hpp"

using namespace std;

struct Edge {
    int vertix1;
    int vertix2;
    bool operator==(const Edge& other);
    bool operator!=(const Edge& other);                        
    Edge operator=(const Edge& other);
    friend ostream& operator<<(ostream& os, const Edge& edge);
};

class Graph {
    public:
        Graph() = default; // construtor vazio
        Graph(string filePath, bool directGraph); // construtor recebendo arquivo
        Graph(Graph& other); // construtor de copia
        void removeEdge(Edge edge);
        void addEdge(Edge edge);
        void removeVertix(int vertix);
        void addVertix(int vertix);
        bool hasEdge(Edge edge, bool reverseVerification = false);
        bool hasVertix(int vertix);
        bool isDirected();
        int getNumVertix();
        int vertixAt(int index);
        int indexOfVertix(int vertix);
        int getNumEdges();
        Edge edgeAt(int index);
        int indexOfEdge(Edge edge);
        int getVertixDegree(int vertix);

        List<List<int>> getAdjacencyList();
        void printAdjacencyMatrix();
        void printAdjacencyList();
        void printFormatedData();

    private:
        bool directGraph = false;
        List<Edge> edgeList;
        List<int> vertixList;
        List<List<int>> adjacencyMatrix;
        List<List<int>> adjacencyList;

        void updateAdjacencyMatrix();
        void updateAdjacencyList();
        void deleteGraph();
};


bool Edge::operator==(const Edge& other) {
    return (this->vertix1 == other.vertix1 && this->vertix2 == other.vertix2);
}

bool Edge::operator!=(const Edge& other) {
    return (this->vertix1 != other.vertix1 or this->vertix2 != other.vertix2);
}
                                
Edge Edge::operator=(const Edge& other) { 
    if (this != &other) {
        this->vertix1 = other.vertix1;
        this->vertix2 = other.vertix2;
    }
    return *this;
}

ostream& operator<<(ostream& os, const Edge& edge) {
    os << "(" << edge.vertix1 << ", " << edge.vertix2 << ")";
    return os;
}


Graph::Graph(string filePath, bool directGraph) {
    ifstream graphFile;
    string buffer;
    exception parseError;
    Edge edge;
    int num;
    char trash;

    this->directGraph = directGraph;
    
    graphFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        graphFile.open(filePath);
    } catch (ifstream::failure fileError) {
        throw fileError;
    }

    // tenta fazer o parse de "V = {x,y};"
    getline(graphFile, buffer, '{');
    if (buffer != "V = ") throw parseError;
    getline(graphFile, buffer, ';');
    stringstream stringBuffer(buffer);

    do { 
        stringBuffer >> num;
        stringBuffer >> trash;

        if (stringBuffer.fail() or (trash != ',' && trash != '}')) throw parseError;

        this->addVertix(num);
    } while (trash != '}');

    // tenta fazer o parse de "A = {x,y};"
    // tenta fazer o parse de "A = {(x,y),(w,x),(y,z)};"
    // tenta fazer o parse de "A = {};"
    getline(graphFile, buffer, '{');
    if (buffer != " A = ") throw parseError;
    getline(graphFile, buffer, ';');
    stringstream stringBuffer1(buffer);

    if (buffer.length() == 4) { 
        stringBuffer1 >> num;
        stringBuffer1 >> trash;
        edge.vertix1 = num;

        stringBuffer1 >> num;
        edge.vertix2 = num;

        if (stringBuffer1.fail()) throw parseError;

        try {
            this->addEdge(edge);
        } catch(exception e) {
            throw parseError;
        }
    } else if (buffer.length() > 4) { 
        do { 
            stringBuffer1 >> trash;
            stringBuffer1 >> num;
            edge.vertix1 = num;

            stringBuffer1 >> trash;
            stringBuffer1 >> num;
            edge.vertix2 = num;
            stringBuffer1 >> trash;
            stringBuffer1 >> trash;

            if (stringBuffer1.fail()) throw parseError;
            
            try {
                this->addEdge(edge);
            } catch(exception e) {
                throw parseError;
            }
        } while (trash != '}');
    }
}

Graph::Graph(Graph& other) {
    List<Edge> newEdgeList;
    List<int> newVertixList;

    if (this != &other)  {
        this->directGraph = other.isDirected();

        for (int i = 0; i < other.getNumEdges(); i++) {
            newEdgeList.insert(other.edgeAt(i));
        }

        for (int i = 0; i < other.getNumVertix(); i++) {
            newVertixList.insert(other.vertixAt(i));
        }

        this->edgeList = newEdgeList;
        this->vertixList = newVertixList;
    }
}

void Graph::printAdjacencyMatrix() {
    this->updateAdjacencyMatrix();
    List<List<int>> adjacencyMatrix = this->adjacencyMatrix;
    List<int> vertixList = this->vertixList;

    cout << "  ";
    for (int i = 0; i < vertixList.length(); i++) {
        cout << vertixList.at(i) << " ";
    }
    cout << endl;

    for (int i = 0; i < adjacencyMatrix.length(); i++) {
        cout << vertixList.at(i) << " ";
        for (int j = 0; j < adjacencyMatrix.at(i).length(); j++) {
            cout << adjacencyMatrix.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

void Graph::printAdjacencyList() {
    this->updateAdjacencyList();

    List<List<int>> adjacencyList = this->adjacencyList;
    List<int> vertixList = this->vertixList;

    for (int i = 0; i < adjacencyList.length(); i++) {
        cout << vertixList.at(i) << " -> ";
        for (int j = 0; j < adjacencyList.at(i).length(); j++) {
            cout << adjacencyList.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

void Graph::printFormatedData() {
    List<int> vertixList = this->vertixList;
    List<Edge> edgeList = this->edgeList;

    cout << "V = {";

    for (int i = 0; i < vertixList.length(); i++) {
        cout << vertixList.at(i);
        if (i != vertixList.length() - 1) cout << ",";
    }

    cout << "}; A = {";

    for (int i = 0; i < edgeList.length(); i++) {
        cout << "(" << edgeList.at(i).vertix1 << "," << edgeList.at(i).vertix2 << ")";
        if (i != edgeList.length() - 1) cout << ",";
    }

    cout << "};" << endl;
}

bool Graph::hasEdge(Edge edge, bool reverseVerification) {
    bool has = false;

    if (this->edgeList.has(edge)) has = true;

    if (reverseVerification) {
        Edge reverseEdge;
        reverseEdge.vertix1 = edge.vertix2;
        reverseEdge.vertix2 = edge.vertix1;
        if (this->edgeList.has(reverseEdge)) has = true;
    }

    return has;
}

void Graph::updateAdjacencyMatrix() {
    List<List<int>> newAdjacencyMatrix;
    List<int> vertixList = this->vertixList;
    List<Edge> edgeList = this->edgeList;

    for (int i = 0; i < vertixList.length(); i++) {
        List<int> newList;
        for (int j = 0; j < vertixList.length(); j++) {
            newList.insert(0);
        }
        newAdjacencyMatrix.insert(newList);
    }

    for (int i = 0; i < edgeList.length(); i++) {
        newAdjacencyMatrix.at(vertixList.indexOf(edgeList.at(i).vertix1)).insertAt(vertixList.indexOf(edgeList.at(i).vertix2) , 1);
    }   

    this->adjacencyMatrix = newAdjacencyMatrix;
}

void Graph::updateAdjacencyList() {
    List<List<int>> newAdjacencyList;
    List<int> vertixList = this->vertixList;
    List<Edge> edgeList = this->edgeList;

    for (int i = 0; i < vertixList.length(); i++) {
        List<int> newList;
        newAdjacencyList.insert(newList);
    }

    for (int i = 0; i < vertixList.length(); i++) {
        for (int j = 0; j < edgeList.length(); j++) {
            if (this->edgeList.at(j).vertix1 == vertixList.at(i)) newAdjacencyList.at(i).insert(edgeList.at(j).vertix2);
        }
    }

    this->adjacencyList = newAdjacencyList;
}

int Graph::getVertixDegree(int vertix) {
    List<Edge> edgeList = this->edgeList;
    int count = 0;

    for (int i = 0; i < edgeList.length(); i++) {
        if (!this->directGraph and (edgeList.at(i).vertix1 == vertix or edgeList.at(i).vertix2 == vertix)) {
            count++;
        } else if (edgeList.at(i).vertix2 == vertix) {
            count++;
        }
    }

    return count;
}

void Graph::removeEdge(Edge edge) {
    if (!this->hasEdge(edge)) return;
    this->edgeList.remove(edge);
}

void Graph::addEdge(Edge edge) {
    exception insertionError;
    if (!this->hasVertix(edge.vertix1) or !this->hasVertix(edge.vertix2)) throw insertionError;
    this->edgeList.insert(edge);
}

void Graph::removeVertix(int vertix) {
    if (!this->hasVertix(vertix)) return;

    List<Edge> edgeList = this->edgeList;

    this->vertixList.remove(vertix);

    for (int i = 0; i < edgeList.length(); i++) {
        if (edgeList.at(i).vertix1 == vertix or edgeList.at(i).vertix2 == vertix) this->edgeList.remove(edgeList.at(i));
    }
}

void Graph::addVertix(int vertix) {
    this->vertixList.insert(vertix);
}

bool Graph::hasVertix(int vertix) {
    return this->vertixList.has(vertix);
}

bool Graph::isDirected() {
    return this->directGraph;
}

int Graph::getNumVertix() {
    return this->vertixList.length();
}

int Graph::vertixAt(int index) {
    return this->vertixList.at(index);
}

int Graph::getNumEdges() {
    return this->edgeList.length();
}

Edge Graph::edgeAt(int index) {
    return this->edgeList.at(index);
}

int Graph::indexOfEdge(Edge edge) {
    return this->edgeList.indexOf(edge);
}

int Graph::indexOfVertix(int vertix) {
    return this->vertixList.indexOf(vertix);
}

List<List<int>> Graph::getAdjacencyList() {
    this->updateAdjacencyList();
    List<List<int>> newAdjacencyList = this->adjacencyList;
    return newAdjacencyList;
}
