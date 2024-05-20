#pragma once
#include <iostream>
#include <exception>
#include "graph.hpp"
#include "list.hpp"

class Digraph : public Graph {
    public:
        Digraph(List<int> vertixList, List<Edge> edgeList);
        void removeEdge(Edge edge);
        void addEdge(Edge edge);
        bool isDirected();

    private:
        bool directGraph = true;
};

Digraph::Digraph(List<int> vertixList, List<Edge> edgeList) {
    this->vertixList = vertixList;
    this->edgeList = edgeList;
}

void Digraph::removeEdge(Edge edge) {
    this->edgeList.remove(edge);
}

void Digraph::addEdge(Edge edge) {
    this->edgeList.insert(edge);
}

bool Digraph::isDirected() {
    return this->directGraph;
}

