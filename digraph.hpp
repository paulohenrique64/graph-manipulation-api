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
        void addEdge(Edge edge);
        void removeEdge(Edge edge);
        bool directed(); 
        bool connected();
        bool stronglyConnected(); 
        bool haveCycle(); 
        void transpose();
        List<int> getKahnTopologicalSort(); 
        List<int> getDFSTopologicalSort(); 
        List<List<int>> getKosarajuComponents();

    private:
        bool directGraph = true;
        bool DFSCycle(int vertex, Digraph digraph, bool* visited, bool* recStack);
        void DFSTopologicalSort(int vertex, Digraph digraph, bool* visited, List<int>& topList);
};

Digraph::Digraph(List<int> vertexList) {
    this->vertexList = vertexList;
}

Digraph::Digraph(List<int> vertexList, List<Edge> edgeList) {
    this->vertexList = vertexList;

    for (int i = 0; i < edgeList.size(); i++) 
        this->addEdge(edgeList[i]);
}

Digraph::Digraph(List<int> vertexList, List<Edge> edgeList, List<int> weightList) {
    exception e;

    if (weightList.size() != edgeList.size()) 
        throw e;
    
    this->vertexList = vertexList;

    for (int i = 0; i < edgeList.size(); i++) {
        edgeList[i].setWeight(weightList[i]);
        this->addEdge(edgeList[i]);
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

bool Digraph::directed() {
    return this->directGraph;
}

bool Digraph::connected() {
    return Graph(this->vertexList, this->edgeList).connected();
}

bool Digraph::stronglyConnected() {
    Digraph digraph = *this;
    int numVertex = digraph.getNumVertex();
    int initialVertex = digraph.vertexAt(0);
    bool visited[numVertex];
    
    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;

    this->DFS(initialVertex, digraph, visited);

    for (int i = 0; i < numVertex; i++) {
        if (!visited[i]) 
            return false;

        visited[i] = false;
    }

    digraph.transpose();

    this->DFS(initialVertex, digraph, visited);

    for (int i = 0; i < numVertex; i++) 
        if (!visited[i]) 
            return false;

    return true;
}

void Digraph::transpose() {  
    for (int i = 0; i < this->edgeList.size(); i++) 
        edgeList[i].reverse();
}

bool Digraph::haveCycle() {
    Digraph digraph = *this;
    int numVertex = digraph.getNumVertex();
    bool visited[numVertex];
    bool recStack[numVertex];

    for (int i = 0; i < numVertex; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    for (int i = 0; i < numVertex; i++) {
        int vertex = digraph.vertexAt(i);

        if (!visited[i]) 
            if (this->DFSCycle(vertex, digraph, visited, recStack)) 
                return true;
    }
  
    return false;
}

bool Digraph::DFSCycle(int vertex, Digraph digraph, bool* visited, bool* recStack) {
    int vertexIndex = digraph.getVertexIndex(vertex);

    List<int> adjVertexList = digraph.getAdjacencyList()[vertexIndex];

    visited[vertexIndex] = true;
    recStack[vertexIndex] = true;

    for (int i = 0; i < adjVertexList.size(); i++) {
        int adjVertex = adjVertexList[i];
        int adjVertexIndex = digraph.getVertexIndex(adjVertex);

        if (!visited[adjVertexIndex]) {
            if (this->DFSCycle(adjVertex, digraph, visited, recStack)) 
                return true;
        } else if (recStack[adjVertexIndex]) {
            return true;
        }
    }

    recStack[vertexIndex] = false;

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
    int numVertex = digraph.getNumVertex();
    bool visited[numVertex];
    List<int> topList;

    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;
    
    for (int i = 0; i < numVertex; i++) {
        int vertex = digraph.vertexAt(i);

        if (!visited[i]) 
            this->DFSTopologicalSort(vertex, digraph, visited, topList);
    }

    topList.reverse();

    return topList;
}

void Digraph::DFSTopologicalSort(int vertex, Digraph digraph, bool* visited, List<int>& topList) {
    int vertexIndex = digraph.getVertexIndex(vertex);

    List<int> vertexList = digraph.getAdjacencyList()[vertexIndex];

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.size(); i++) {
        int adjVertex = vertexList[i];

        if (!visited[i]) 
            this->DFSTopologicalSort(adjVertex, digraph, visited, topList);
    }

    topList.insert(vertex);
}

List<List<int>> Digraph::getKosarajuComponents() {
    Digraph digraph = *this;
    List<int> recStack;
    int numVertex = digraph.getNumVertex();
    bool visited[numVertex];

    for (int i = 0; i < numVertex; i++)
        visited[i] = false;

    for (int i = 0; i < numVertex; i++) {
        int vertex = digraph.vertexAt(i);

        if (!visited[i])
            this->DFSRecStack(vertex, digraph, visited, recStack);
    }

    for (int i = 0; i < numVertex; i++)
        visited[i] = false;

    recStack.reverse();
    digraph.transpose();

    List<int> newRecStack;
    List<List<int>> components;

    for (int i = 0; i < recStack.size(); i++) {
        List<int> component;
        int vertex = recStack[i];
        int vertexIndex = digraph.getVertexIndex(vertex);

        if (!visited[vertexIndex]) {
            this->DFSRecStack(vertex, digraph, visited, newRecStack);

            while (!newRecStack.empty()) 
                component.insert(newRecStack.removeFirst());

            components.insert(component);
        }
    }

    return components;
}
