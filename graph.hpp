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
        Edge getEdge(int source, int destination);
        bool hasVertex(int vertex);
        int getNumVertex();
        int getNumEdges();
        int vertexAt(int index);
        Edge edgeAt(int index);
        int indexOfVertex(int vertex);
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
        void DFS(int vertex, Graph graph, bool* visited);
        virtual bool DFSCycle(int vertex, Graph graph, bool* visited);
        void DFSTree(int vertex, Graph graph, Graph& tree, bool* visited);
        void BFSTree(int vertex, Graph graph, Graph& tree, bool* visited, List<int>& queue);

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
    int numVertex = graph.getNumVertex();
    bool visited[numVertex];

    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;

    this->DFS(graph.vertexAt(0), graph, visited);

    for (int i = 0; i < numVertex; i++) 
        if (!visited[i]) 
            return false;

    return true;
}

void Graph::DFS(int vertex, Graph graph, bool* visited) {
    int vertexIndex = graph.indexOfVertex(vertex);

    List<int> vertexList = graph.getAdjacencyList().at(vertexIndex);

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.length(); i++) {
        int adjIndex, adjVertex;

        adjVertex = vertexList.at(i);
        adjIndex = graph.indexOfVertex(adjVertex);

        if (!visited[adjIndex]) 
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

Edge Graph::getEdge(int source, int destination) {
    for (int i = 0; i < this->edgeList.length(); i++) {
        Edge edge = this->edgeList.at(i);
        if (edge.getSource() == source and edge.getDestination() == destination) 
            return edge;
    }

    throw runtime_error("this edge does not exists");
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
    int numVertex = graph.getNumVertex();
    bool visited[numVertex];

    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;

    for (int i = 0; i < numVertex; i++) {
        int vertex = graph.vertexAt(i);

        if (!visited[i]) 
            if (this->DFSCycle(vertex, graph, visited)) 
                return true;
    }

    return false;
}

bool Graph::DFSCycle(int vertex, Graph graph, bool* visited) {
    int vertexIndex = graph.indexOfVertex(vertex);
    List<int> vertexList = graph.getAdjacencyList().at(vertexIndex);

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.length(); i++) {
        int adjVertex = vertexList.at(i);
        int adjVertexIndex = graph.indexOfVertex(adjVertex);

        if (!visited[adjVertexIndex]) {
            graph.removeEdge(Edge(adjVertex, vertex));

            if (this->DFSCycle(adjVertex, graph, visited)) 
                return true;
        } else {
            return true;
        }
    }
           
    return false;
}

List<Graph> Graph::getDFSTree() {
    Graph graph = *this;
    List<Graph> treeList;
    int numVertex = graph.getNumVertex();
    bool visited[numVertex];
    
    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;

    for (int i = 0; i < numVertex; i++) {
        if (!visited[i]) {
            Graph tree(graph.getVertexList());
            this->DFSTree(graph.vertexAt(i), graph, tree, visited);
            treeList.insert(tree);
        }
    } 

    return treeList;
}

void Graph::DFSTree(int vertex, Graph graph, Graph& tree, bool* visited) {
    int vertexIndex = graph.indexOfVertex(vertex);

    List<int> vertexList = graph.getAdjacencyList().at(vertexIndex);

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.length(); i++) {
        int adjVertex = vertexList.at(i);
        int adjVertexIndex = graph.indexOfVertex(adjVertex);

        if (!visited[adjVertexIndex]) {
            tree.addEdge(Edge(vertex, adjVertex));
            this->DFSTree(adjVertex, graph, tree, visited);
        }
    }
}

List<Graph> Graph::getBFSTree() {
    Graph graph = *this;
    List<Graph> treeList;
    List<int> queue;
    int numVertex = graph.getNumVertex();
    bool visited[numVertex];
    
    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;

    for (int i = 0; i < graph.getNumVertex(); i++) {
        if (!visited[i]) {
            Graph tree;
            tree.addVertex(graph.vertexAt(i));
            this->BFSTree(graph.vertexAt(i), graph, tree, visited, queue);
            treeList.insert(tree);
        }
    } 

    return treeList;
}

void Graph::BFSTree(int vertex, Graph graph, Graph& tree, bool* visited, List<int>& queue) {
    int vertexIndex = graph.indexOfVertex(vertex);

    List<int> vertexList = graph.getAdjacencyList().at(vertexIndex);

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.length(); i++) {
        int adjVertex = vertexList.at(i);
        int adjVertexIndex = graph.indexOfVertex(adjVertex);

        if (!visited[adjVertexIndex]) {
            tree.addVertex(adjVertex);
            tree.addEdge(Edge(vertex, adjVertex));

            visited[adjVertexIndex] = true;
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
    List<List<int>> adjacencyList = this->getAdjacencyList();
    List<int> vertexList = this->vertexList;
    List<int> queue = this->vertexList;
    int key[vertexList.length()];
    int parents[vertexList.length()];
    int u, v;

    for (int i = 0; i < vertexList.length(); i++) {
        key[i] = 9999999;
        parents[i] = -1;
    }

    key[0] = 0;

    while (!queue.isEmpty()) {
        int minKeyIndex = 0;

        for (int i = 0; i < queue.length(); i++) {
            int index = vertexList.indexOf(queue.at(i));

            if (key[index] < key[vertexList.indexOf(queue.at(minKeyIndex))]) 
                minKeyIndex = i;
        }

        u = queue.at(minKeyIndex);
        queue.remove(u);

        List<int> adj = adjacencyList.at(vertexList.indexOf(u));

        for (int i = 0; i < adj.length(); i++) {
            v = adj.at(i);
            int vIndex = vertexList.indexOf(v);
            Edge edge = this->getEdge(u, v);
            int weight = edge.getWeight();

            if (queue.has(v) and weight < key[vIndex]) {
                parents[vIndex] = u;
                key[vIndex] = weight;
            }
        }   
    }

    Graph prim(vertexList);

    for (int i = 0; i < vertexList.length(); i++) 
        if (parents[i] != -1) 
            prim.addEdge(Edge(parents[i], vertexList.at(i), key[i]));

    return prim;
}
