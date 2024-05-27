#pragma once
#include <iostream>
#include <exception>
#include "graph.hpp"
#include "list.hpp"

class Digraph : public Graph {
    public:
        Digraph() = default;
        Digraph(List<int> vertexList);
        Digraph(List<int> vertexList, List<Edge> edgeList);
        Digraph(List<int> vertexList, List<Edge> edgeList, List<int> weightList);
        void removeEdge(Edge edge);
        void addEdge(Edge edge);
        bool isDirected();
        bool isConnected(); 
        bool isStronglyConnected(); 
        bool haveCycle(); 
        void reverse();

        List<int> getKahnTopologicalSort(); 
        List<int> getDFSTopologicalSort(); 
        // List<Graph> getKosarajuComponents();

    private:
        bool directGraph = true;
        bool DFSCycle(int vertex, Digraph digraph, List<bool>& visited, List<bool>& recStack);
        void DFSTopologicalSort(int vertex, Digraph digraph, List<bool>& visited, List<int>& topList);
};

Digraph::Digraph(List<int> vertexList) {
    this->vertexList = vertexList;
}

Digraph::Digraph(List<int> vertexList, List<Edge> edgeList) {
    this->vertexList = vertexList;

    for (int i = 0; i < edgeList.length(); i++) 
        this->addEdge(edgeList.at(i));
}

Digraph::Digraph(List<int> vertexList, List<Edge> edgeList, List<int> weightList) {
    exception e;

    if (weightList.length() != edgeList.length()) 
        throw e;
    
    this->vertexList = vertexList;

    for (int i = 0; i < edgeList.length(); i++) {
        edgeList.at(i).setWeight(weightList.at(i));
        this->addEdge(edgeList.at(i));
    }
}

void Digraph::removeEdge(Edge edge) {
    this->edgeList.remove(edge);
}

void Digraph::addEdge(Edge edge) {
    exception e;

    if (!this->hasVertex(edge.getSource()) or !this->hasVertex(edge.getDestination())) 
        throw e;
    
    this->edgeList.insert(edge);}

bool Digraph::isDirected() {
    return this->directGraph;
}

bool Digraph::isConnected() {
    return Graph(this->vertexList, this->edgeList).isConnected();
}

bool Digraph::isStronglyConnected() {
    Digraph digraph = *this;
    Digraph reverseDigraph = *this;
    List<bool> visited;

    reverseDigraph.reverse();

    for (int i = 0; i < digraph.getNumVertex(); i++) 
        visited.insert(false);

    this->DFS(digraph.vertexAt(0), digraph, visited);

    for (int i = 0; i < visited.length(); i++) 
        if (!visited.at(i)) 
            return false;

    for (int i = 0; i < visited.length(); i++) 
        visited.insertAt(i, false);

    this->DFS(reverseDigraph.vertexAt(0), reverseDigraph, visited);

    for (int i = 0; i < visited.length(); i++) 
        if (!visited.at(i)) 
            return false;

    return true;
}

void Digraph::reverse() {  
    for (int i = 0; i < this->edgeList.length(); i++) 
        edgeList.at(i).reverse();
}

bool Digraph::haveCycle() {
    Digraph digraph = *this;

    List<bool> visited;
    List<bool> recStack;

    for (int i = 0; i < digraph.getNumVertex(); i++) {
        visited.insert(false);
        recStack.insert(false);
    }

    for (int i = 0; i < digraph.getNumVertex(); i++)
        if (!visited.at(digraph.indexOfVertex(digraph.vertexAt(i)))) 
            if (this->DFSCycle(digraph.vertexAt(i), digraph, visited, recStack)) 
                return true;
  
    return false;
}

bool Digraph::DFSCycle(int vertex, Digraph digraph, List<bool>& visited, List<bool>& recStack) {
    int vertexIndex = digraph.indexOfVertex(vertex);

    visited.insertAt(vertexIndex, true);
    recStack.insertAt(vertexIndex, true);

    List<int> vertexList = digraph.getAdjacencyList().at(vertexIndex);

    for (int i = 0; i < vertexList.length(); i++) {
        int adjVertex = vertexList.at(i);
        int adjVertexIndex = digraph.indexOfVertex(adjVertex);

        if (!visited.at(adjVertexIndex)) {
            if (this->DFSCycle(adjVertex, digraph, visited, recStack)) {
                return true;
            }
        } else if (recStack.at(adjVertexIndex)) {
            return true;
        }
    }

    recStack.insertAt(vertexIndex, false);

    return false;
}

List<int> Digraph::getKahnTopologicalSort() {
    List<int> topList;
    Digraph digraph = *this;

    while (digraph.getNumVertex() > 0) {
        for (int i = 0; i < digraph.getNumVertex(); i++) {
            int vertex = digraph.vertexAt(i);

            if (digraph.getVertexDegree(vertex) == 0) {
                digraph.removeVertex(vertex);
                topList.insert(vertex);
            }
        }
    }
    
    return topList;
}

List<int> Digraph::getDFSTopologicalSort() {
    Digraph digraph = *this;
    List<bool> visited;
    List<int> topList;

    for (int i = 0; i < digraph.getNumVertex(); i++) 
        visited.insert(false);
    
    for (int i = 0; i < digraph.getNumVertex(); i++)
        if (!visited.at(digraph.indexOfVertex(digraph.vertexAt(i)))) 
            this->DFSTopologicalSort(digraph.vertexAt(i), digraph, visited, topList);

    topList.reverse();

    return topList;
}

void Digraph::DFSTopologicalSort(int vertex, Digraph digraph, List<bool>& visited, List<int>& topList) {
    int vertexIndex = digraph.indexOfVertex(vertex);
    List<int> vertexList = digraph.getAdjacencyList().at(vertexIndex);

    visited.insertAt(vertexIndex, true);

    for (int i = 0; i < vertexList.length(); i++) {
        int adjVertex = vertexList.at(i);
        int adjVertexIndex = digraph.indexOfVertex(adjVertex);

        if (!visited.at(adjVertexIndex)) 
            this->DFSTopologicalSort(adjVertex, digraph, visited, topList);
    }

    topList.insert(vertex);
}

// List<Graph> Digraph::getKosarajuComponents() {
//  
// }


