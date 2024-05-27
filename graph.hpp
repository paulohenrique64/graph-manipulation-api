#pragma once
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <fstream>
#include "list.hpp"

using namespace std;

class Edge {
    public:
        Edge() = default;
        Edge(int source, int destination);
        Edge(int source, int destination, int weight);
        void reverse();
        int getWeight() const;
        int getSource() const;
        int getDestination() const;
        void setWeight(int weight);
        void setSource(int weight);
        void setDestination(int weight);
        bool hasWeight() const;
        bool operator==(const Edge& other);
        bool operator!=(const Edge& other);      
        bool operator<(const Edge& other);   
        bool operator>(const Edge& other);                     
        Edge& operator=(const Edge& other);
        friend ostream& operator<<(ostream& os, const Edge& edge);

    private:
        int source;
        int destination;
        int* weight = nullptr;
};


class Graph {
    public:
        Graph() = default;
        Graph(List<int> vertexList); 
        Graph(List<int> vertexList, List<Edge> edgeList); 
        Graph(List<int> vertexList, List<Edge> edgeList, List<int> weightList);
        Graph(Graph& other);
        Graph(Graph&& other); 
        Graph& operator=(Graph& other); 
        Graph& operator=(Graph&& other); 
        virtual void removeEdge(Edge edge);
        virtual void addEdge(Edge edge);
        virtual bool isDirected();
        virtual bool isConnected(); 
        bool isWeighted();
        void removeVertex(int vertex);
        void addVertex(int vertex);
        bool hasEdge(Edge edge);
        bool hasVertex(int vertex);
        int getNumVertex();
        int vertexAt(int index);
        int indexOfVertex(int vertex);
        int getNumEdges();
        Edge edgeAt(int index);
        int indexOfEdge(Edge edge);
        int getVertexDegree(int vertex);
        int getNumOddDegreeVertex(); 
        List<Edge> getEdgeList();
        List<int> getVertexList();
        List<List<int>> getAdjacencyList();
        List<int> getAloneVertexList();
        virtual bool haveCycle(); 
        void printAdjacencyMatrix();
        void printAdjacencyList();
        void printFormatedData();
        List<Graph> getDFSTree();
        List<Graph> getBFSTree();
        Graph getKruskalTree();
        Graph getPrimTree();

    protected:
        List<Edge> edgeList;
        List<int> vertexList;
        List<List<int>> adjacencyMatrix;
        List<List<int>> adjacencyList;
        void updateAdjacencyMatrix();
        void updateAdjacencyList();
        void DFS(int vertex, Graph graph, List<bool>& visited);
        virtual bool DFSCycle(int vertex, Graph graph, List<bool>& visited);
        void DFSTree(int vertex, Graph graph, Graph& tree, List<bool>& visited);
        void BFSTree(int vertex, Graph graph, Graph& tree, List<bool>& visited, List<int>& queue);
        void removeMultipleEdges(List<Edge>& edgeList);

    private:
        bool directGraph = false;
};

Edge::Edge(int source, int destination) {
    this->source = source;
    this->destination = destination;
}

Edge::Edge(int source, int destination, int weight) {
    this->source = source;
    this->destination = destination;
    this->setWeight(weight);
}

int Edge::getSource() const {
    return this->source;
}

int Edge::getDestination() const {
    return this->destination;
}

void Edge::setSource(int source) {
    this->source = source;
}

void Edge::setDestination(int destination) {
    this->destination = destination;
}

void Edge::setWeight(int weight) {
    this->weight = new int;
    *this->weight = weight;
}

bool Edge::hasWeight() const {
    return this->weight != nullptr;
}

int Edge::getWeight() const {
    if (this->hasWeight()) 
        return *this->weight;

    return -4564565;
}

void Edge::reverse() {
    int aux = this->destination;
    this->destination = this->source;
    this->source = aux;
}

bool Edge::operator==(const Edge& other) {
    bool equal = true;

    if (this->source != other.source or this->destination != other.destination)
        equal = false;
    
    if (other.hasWeight() and this->hasWeight() and other.getWeight() != this->getWeight())
        equal = false;

    return equal;
}

bool Edge::operator!=(const Edge& other) {
    bool diff = true;

    if (this->source == other.source and this->destination == other.destination)
        diff = false;
    
    if (other.hasWeight() and this->hasWeight() and other.getWeight() == this->getWeight())
        diff = false;

    return diff;
}

bool Edge::operator<(const Edge& other) {
    if (!other.hasWeight() or !this->hasWeight()) 
        return false;

    return this->getWeight() < other.getWeight();
}  

bool Edge::operator>(const Edge& other) {
    if (!other.hasWeight() or !this->hasWeight()) 
        return false;

    return this->getWeight() > other.getWeight();
} 
                                
Edge& Edge::operator=(const Edge& other) { 
    if (this != &other) {
        this->source = other.getSource();
        this->destination = other.getDestination();

        if (other.hasWeight())
            this->setWeight(other.getWeight());
    }
    return *this;
}

ostream& operator<<(ostream& os, const Edge& edge) {
    os << "(" << edge.source << "," << edge.destination;
    
    if (edge.hasWeight())
        os << "," << edge.getWeight();

    os << ")";
    return os;
}

Graph::Graph(List<int> vertexList) {
    this->vertexList = vertexList;
}

Graph::Graph(List<int> vertexList, List<Edge> edgeList) {
    this->vertexList = vertexList;
    
    for (int i = 0; i < edgeList.length(); i++) 
        this->addEdge(edgeList.at(i));
}

Graph::Graph(List<int> vertexList, List<Edge> edgeList, List<int> weightList) {
    exception e;

    if (weightList.length() != edgeList.length()) 
        throw e;

    this->vertexList = vertexList;

    for (int i = 0; i < edgeList.length(); i++) {
        edgeList.at(i).setWeight(weightList.at(i));
        this->addEdge(edgeList.at(i));
    }
}

Graph::Graph(Graph& other) {
    if (this != &other)  {
        this->directGraph = other.isDirected();
        this->vertexList = other.getVertexList();
        this->edgeList = other.getEdgeList();  
    }
}

Graph::Graph(Graph&& other) {
    if (this != &other)  {
        this->directGraph = other.directGraph;
        this->vertexList = other.vertexList;
        this->edgeList = other.edgeList;   
    }
}

Graph& Graph::operator=(Graph& other) {
    if (this != &other)  {
        this->directGraph = other.isDirected();
        this->vertexList = other.getVertexList();
        this->edgeList = other.getEdgeList();  
    }

    return *this;
}

Graph&  Graph::operator=(Graph&& other) {
    if (this != &other)  {
        this->directGraph = other.directGraph;
        this->vertexList = other.vertexList;
        this->edgeList = other.edgeList;  
    }

    return *this;
}

void Graph::printAdjacencyMatrix() {
    this->updateAdjacencyMatrix();

    List<List<int>> adjacencyMatrix = this->adjacencyMatrix;
    List<int> vertexList = this->vertexList;

    cout << "  ";
    for (int i = 0; i < vertexList.length(); i++) {
        cout << vertexList.at(i) << " ";
    }
    cout << endl;

    for (int i = 0; i < adjacencyMatrix.length(); i++) {
        cout << vertexList.at(i) << " ";
        for (int j = 0; j < adjacencyMatrix.at(i).length(); j++) {
            cout << adjacencyMatrix.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

void Graph::printAdjacencyList() {
    this->updateAdjacencyList();

    List<List<int>> adjacencyList = this->adjacencyList;
    List<int> vertexList = this->vertexList;

    for (int i = 0; i < adjacencyList.length(); i++) {
        cout << vertexList.at(i) << " -> ";
        for (int j = 0; j < adjacencyList.at(i).length(); j++) {
            cout << adjacencyList.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

void Graph::printFormatedData() {
    List<int> vertexList = this->vertexList;
    List<Edge> edgeList = this->edgeList;

    cout << "V = {";

    for (int i = 0; i < vertexList.length(); i++) {
        cout << vertexList.at(i);
        if (i != vertexList.length() - 1) 
            cout << ",";
    }

    cout << "}; A = {";

    for (int i = 0; i < edgeList.length(); i++) {
        cout << edgeList.at(i);
        if (i != edgeList.length() - 1) 
            cout << ",";
    }

    cout << "};" << endl;
}

void Graph::updateAdjacencyMatrix() {
    List<List<int>> newAdjacencyMatrix;
    List<int> vertexList = this->vertexList;
    List<Edge> edgeList = this->edgeList;

    for (int i = 0; i < vertexList.length(); i++) {
        List<int> newList;
        for (int j = 0; j < vertexList.length(); j++) 
            newList.insert(0);
        newAdjacencyMatrix.insert(newList);
    }

    for (int i = 0; i < edgeList.length(); i++) {
        newAdjacencyMatrix
            .at(vertexList.indexOf(edgeList.at(i).getSource()))
            .at(vertexList.indexOf(edgeList.at(i).getDestination()))++;
    }   

    this->adjacencyMatrix = newAdjacencyMatrix;
}

void Graph::updateAdjacencyList() {
    List<List<int>> newAdjacencyList;
    List<int> vertexList = this->vertexList;
    List<Edge> edgeList = this->edgeList;

    for (int i = 0; i < vertexList.length(); i++) {
        List<int> newList;
        newAdjacencyList.insert(newList);
    }

    for (int i = 0; i < vertexList.length(); i++) 
        for (int j = 0; j < edgeList.length(); j++) 
            if (vertexList.at(i) == edgeList.at(j).getSource()) 
                newAdjacencyList.at(i).insert(edgeList.at(j).getDestination());
        
    this->adjacencyList = newAdjacencyList;
}

int Graph::getVertexDegree(int vertex) {
    List<Edge> edgeList = this->edgeList;
    int count = 0;

    for (int i = 0; i < edgeList.length(); i++) 
        if (edgeList.at(i).getDestination() == vertex) 
            count++;

    return count;
}

void Graph::removeVertex(int vertex) {
    List<Edge> edgeList = this->edgeList;

    this->vertexList.remove(vertex);

    for (int i = 0; i < edgeList.length(); i++) 
        if (edgeList.at(i).getSource() == vertex or edgeList.at(i).getDestination() == vertex) 
            this->edgeList.remove(edgeList.at(i));
}

bool Graph::hasEdge(Edge edge) {
    return this->edgeList.has(edge);
}

void Graph::removeEdge(Edge edge) {
    this->edgeList.remove(edge);
    edge.reverse();
    this->edgeList.remove(edge);
}

void Graph::addEdge(Edge edge) {
    exception e;

    if (!this->hasVertex(edge.getSource()) or !this->hasVertex(edge.getDestination())) 
        throw e;

    this->edgeList.insert(edge);
    edge.reverse();
    this->edgeList.insert(edge);
}

void Graph::addVertex(int vertex) {
    this->vertexList.insert(vertex);
}

bool Graph::hasVertex(int vertex) {
    return this->vertexList.has(vertex);
}

int Graph::getNumVertex() {
    return this->vertexList.length();
}

int Graph::vertexAt(int index) {
    return this->vertexList.at(index);
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

int Graph::indexOfVertex(int vertex) {
    return this->vertexList.indexOf(vertex);
}

bool Graph::isDirected() {
    return this->directGraph;
}

List<List<int>> Graph::getAdjacencyList() {
    this->updateAdjacencyList();
    return this->adjacencyList;
}

List<Edge> Graph::getEdgeList() {
    return this->edgeList;
}

List<int> Graph::getVertexList() {
    return this->vertexList;
}

bool Graph::isWeighted() {
    if (this->edgeList.isEmpty()) 
        return false;

    return this->edgeList.at(0).hasWeight();
}

bool Graph::isConnected() {
    Graph graph = *this;
    List<bool> visited;

    for (int i = 0; i < graph.getNumVertex(); i++) 
        visited.insert(false);

    this->DFS(graph.vertexAt(0), graph, visited);

    for (int i = 0; i < visited.length(); i++) 
        if (!visited.at(i)) 
            return false;

    return true;
}

void Graph::DFS(int vertex, Graph graph, List<bool>& visited) {
    int vertexIndex = graph.indexOfVertex(vertex);
    List<int> vertexList = graph.getAdjacencyList().at(vertexIndex);

    visited.insertAt(vertexIndex, true);

    for (int i = 0; i < vertexList.length(); i++) {
        int adjIndex, adjVertex;

        adjVertex = vertexList.at(i);
        adjIndex = graph.indexOfVertex(adjVertex);

        if (!visited.at(adjIndex)) 
            this->DFS(adjVertex, graph, visited);
    }
}

int Graph::getNumOddDegreeVertex() {
    List<int> vertexList = this->vertexList;
    int count = 0;

    for (int i = 0; i < vertexList.length(); i++) 
        if (this->getVertexDegree(vertexList.at(i)) % 2 != 0) 
            count++;

    return count;
}

List<int> Graph::getAloneVertexList() {
    List<int> vertexList = this->vertexList;
    List<Edge> edgeList = this->edgeList;
    List<int> aloneVertexList;
    bool exist;

    for (int i = 0; i < vertexList.length(); i++) {
        exist = false;

        for (int j = 0; j < edgeList.length(); j++) 
            if (vertexList.at(i) == edgeList.at(j).getSource() or vertexList.at(i) == edgeList.at(j).getDestination()) 
                exist = true;

        if (!exist) 
            aloneVertexList.insert(vertexList.at(i));
    }    

    return aloneVertexList;
}

bool Graph::haveCycle() {
    Graph graph = *this;
    List<bool> visited;

    for (int i = 0; i < graph.getNumVertex(); i++) 
        visited.insert(false);

    for (int i = 0; i < graph.getNumVertex(); i++)
        if (!visited.at(i)) 
            if (this->DFSCycle(graph.vertexAt(i), graph, visited)) 
                return true;

    return false;
}

bool Graph::DFSCycle(int vertex, Graph graph, List<bool>& visited) {
    List<int> vertexList = graph.getAdjacencyList().at(graph.indexOfVertex(vertex));

    visited.insertAt(graph.indexOfVertex(vertex), true);

    for (int i = 0; i < vertexList.length(); i++) {
        if (!visited.at(graph.indexOfVertex(vertexList.at(i)))) {
            graph.removeEdge(Edge(vertexList.at(i), vertex));

            if (this->DFSCycle(vertexList.at(i), graph, visited)) 
                return true;
        } else 
            return true;
    }
           
    return false;
}

List<Graph> Graph::getDFSTree() {
    Graph graph = *this;
    List<bool> visited;
    List<Graph> treeList;

    for (int i = 0; i < graph.getNumVertex(); i++) 
        visited.insert(false);

    for (int i = 0; i < graph.getNumVertex(); i++) {
        if (!visited.at(i)) {
            Graph tree(graph.getVertexList());
            this->DFSTree(graph.vertexAt(i), graph, tree, visited);
            treeList.insert(tree);
        }
    } 

    return treeList;
}

void Graph::DFSTree(int vertex, Graph graph, Graph& tree, List<bool>& visited) {
    int vertexIndex = graph.indexOfVertex(vertex);
    List<int> vertexList = graph.getAdjacencyList().at(vertexIndex);

    visited.insertAt(vertexIndex, true);

    for (int i = 0; i < vertexList.length(); i++) {
        int adjVertex = vertexList.at(i);
        int adjVertexIndex = graph.indexOfVertex(adjVertex);

        if (!visited.at(adjVertexIndex)) {
            tree.addEdge(Edge(vertex, adjVertex));
            this->DFSTree(adjVertex, graph, tree, visited);
        }
    }
}

List<Graph> Graph::getBFSTree() {
    Graph graph = *this;
    List<bool> visited;
    List<int> queue;
    List<Graph> treeList;

    for (int i = 0; i < graph.getNumVertex(); i++) 
        visited.insert(false);

    for (int i = 0; i < graph.getNumVertex(); i++) {
        if (!visited.at(i)) {
            Graph tree;
            tree.addVertex(graph.vertexAt(i));
            this->BFSTree(graph.vertexAt(i), graph, tree, visited, queue);
            treeList.insert(tree);
        }
    } 

    return treeList;
}

void Graph::BFSTree(int vertex, Graph graph, Graph& tree, List<bool>& visited, List<int>& queue) {
    int vertexIndex = graph.indexOfVertex(vertex);
    List<int> vertexList = graph.getAdjacencyList().at(vertexIndex);

    visited.insertAt(vertexIndex, true);

    for (int i = 0; i < vertexList.length(); i++) {
        int adjVertex = vertexList.at(i), adjVertexIndex = graph.indexOfVertex(adjVertex);

        if (!visited.at(adjVertexIndex)) {
            tree.addVertex(adjVertex);
            tree.addEdge(Edge(vertex, adjVertex));
            visited.insertAt(adjVertexIndex, true);
            queue.insert(adjVertex);
        }
    }

    while (!queue.isEmpty()) {
        int nextVisitVertex = queue.removeFirst();
        int nextVisitVertexIndex = graph.indexOfVertex(nextVisitVertex);
        this->BFSTree(nextVisitVertex, graph, tree, visited, queue);
    }
}

Graph Graph::getKruskalTree() {
    Graph kruskal(this->vertexList);
    List<Edge> edgeList = this->edgeList;

    this->removeMultipleEdges(edgeList);
    edgeList.sort();
    
    while (!edgeList.isEmpty()) {
        Edge next = edgeList.removeFirst();
        kruskal.addEdge(next);

        if (kruskal.haveCycle())
            kruskal.removeEdge(next);
    }

    return kruskal;
}

Graph Graph::getPrimTree() {
    List<int> vertexList = this->vertexList;
    List<Edge> edgeList = this->edgeList;
    List<Edge> edgePrimList;
    int neededEdges = vertexList.length() - 1;

    this->removeMultipleEdges(edgeList);
    edgeList.sort();

    for (int i = 0; i < neededEdges; i++) 
        edgePrimList.insert(edgeList.at(i));

    Graph prim(vertexList, edgePrimList);

    return prim;
}

// remove edge cicle
// remove reverse edges
// remove multiple edges
void Graph::removeMultipleEdges(List<Edge>& edgeList) {
    List<Edge> newEdgeList;

    for (int i = 0; i < edgeList.length(); i++) {
        Edge edge = edgeList.at(i);
        Edge reverse(edge.getDestination(), edge.getSource());
        bool exist;

        if (!newEdgeList.has(reverse) and edge.getSource() != edge.getDestination()) {
            exist = false;

            for (int j = 0; j < newEdgeList.length(); j++) {
                Edge search = newEdgeList.at(j);

                if (edge.getSource() == search.getSource() and edge.getDestination() == search.getDestination()) {
                    if (edge.getWeight() < search.getWeight()) 
                        newEdgeList.insertAt(j, edge);
                    exist = true;
                } 
            }

            if (!exist)
                newEdgeList.insert(edge);
        }
    }

    edgeList = newEdgeList;
}