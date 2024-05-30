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
        Edge(const Edge& other);
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
        virtual void addEdge(Edge edge);
        void addVertex(int vertex);
        virtual void removeEdge(Edge edge);
        void removeVertex(int vertex);
        bool hasEdge(Edge edge);
        bool hasVertex(int vertex);
        Edge edgeAt(int index);
        Edge edgeAt(int source, int destination);
        int vertexAt(int index);
        int getNumEdges();
        int getNumVertex();
        int getEdgeIndex(Edge edge);
        int getVertexIndex(int vertex);
        int getVertexDegree(int vertex);
        int getNumVertexOddDegree(); 
        List<Edge> getEdgeList();
        List<int> getVertexList();
        List<List<int>> getAdjacencyList();
        List<int> getVertexAloneList();
        List<List<int>> getDFSOrderLists();
        List<List<int>> getBFSOrderLists();
        Graph getKruskalTree();
        Graph getPrimTree();
        virtual bool directed();
        virtual bool connected(); 
        bool weighted(); 
        virtual bool haveCycle(); 
        void printAdjacencyMatrix();
        void printAdjacencyList();
        void printFormatedData();

    protected:
        List<Edge> edgeList;
        List<int> vertexList;
        List<List<int>> adjacencyMatrix;
        List<List<int>> adjacencyList;
        void updateAdjacencyMatrix();
        void updateAdjacencyList();
        void DFS(int vertex, Graph graph, bool* visited);
        bool DFSCycle(int vertex, Graph graph, bool* visited);
        void DFSRecStack(int vertex, Graph graph, bool* visited, List<int>& recStack);
        void BFSRecStack(int vertex, Graph graph, bool* visited, List<int>& recStack, List<int>& queue);

    private:
        bool directGraph = false;
};

// constructor
Edge::Edge(int source, int destination) {
    this->source = source;
    this->destination = destination;
}

// constructor
Edge::Edge(int source, int destination, int weight) {
    this->source = source;
    this->destination = destination;
    this->setWeight(weight);
}

// copy constructor
Edge::Edge(const Edge& other) {
    this->source = other.getSource();
    this->destination = other.getDestination();
    if (other.hasWeight())
        this->setWeight(other.getWeight());
} 

// return edge source
int Edge::getSource() const {
    return this->source;
}

// return edge destination
int Edge::getDestination() const {
    return this->destination;
}

// set edge source
void Edge::setSource(int source) {
    this->source = source;
}

// set edge destination
void Edge::setDestination(int destination) {
    this->destination = destination;
}

// set edge weight
void Edge::setWeight(int weight) {
    this->weight = new int;
    *this->weight = weight;
}

// return true if edge has weight
bool Edge::hasWeight() const {
    return this->weight != nullptr;
}

// return edge weight
int Edge::getWeight() const {
    return *this->weight;
}

// make edge reverse
void Edge::reverse() {
    int aux = this->destination;
    this->destination = this->source;
    this->source = aux;
}

// operator == overload
bool Edge::operator==(const Edge& other) {
    bool equal = false;

    if (this->source == other.source and this->destination == other.destination)
        equal = true;
    
    if (other.hasWeight() and this->hasWeight() and other.getWeight() != this->getWeight())
        equal = false;

    return equal;
}

// operator != overload
bool Edge::operator!=(const Edge& other) {
    bool diff = false;

    if (this->source != other.source or this->destination != other.destination)
        diff = true;
    
    if (other.hasWeight() and this->hasWeight() and other.getWeight() == this->getWeight())
        diff = false;

    return diff;
}

// operator < overload
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
    
    for (int i = 0; i < edgeList.size(); i++) 
        this->addEdge(edgeList[i]);
}

Graph::Graph(List<int> vertexList, List<Edge> edgeList, List<int> weightList) {
    exception e;

    if (weightList.size() != edgeList.size()) 
        throw e;

    this->vertexList = vertexList;

    for (int i = 0; i < edgeList.size(); i++) {
        edgeList[i].setWeight(weightList[i]);
        this->addEdge(edgeList[i]);
    }
}

Graph::Graph(Graph& other) {
    if (this != &other)  {
        this->directGraph = other.directed();
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
        this->directGraph = other.directed();
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

    cout << "  ";
    for (int i = 0; i < this->vertexList.size(); i++) {
        cout << this->vertexList[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < this->adjacencyMatrix.size(); i++) {
        cout << this->vertexList[i] << " ";
        for (int j = 0; j < this->adjacencyMatrix[i].size(); j++) {
            cout << this->adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::printAdjacencyList() {
    this->updateAdjacencyList();

    for (int i = 0; i < this->adjacencyList.size(); i++) {
        cout << this->vertexList[i] << " -> ";
        for (int j = 0; j < this->adjacencyList[i].size(); j++) {
            cout << this->adjacencyList[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::printFormatedData() {
    cout << "V = {";

    for (int i = 0; i < this->vertexList.size(); i++) {
        cout << this->vertexList[i];
        if (i != this->vertexList.size() - 1) 
            cout << ",";
    }

    cout << "}; A = {";

    for (int i = 0; i < this->edgeList.size(); i++) {
        cout << this->edgeList[i];
        if (i != this->edgeList.size() - 1) 
            cout << ",";
    }

    cout << "};" << endl;
}

void Graph::updateAdjacencyMatrix() {
    int matSize = this->vertexList.size();
    List<List<int>> newAdjacencyMatrix(matSize, List<int>(matSize, 0));

    for (int i = 0; i < this->edgeList.size(); i++) {
        int sourceIndex = this->vertexList.indexOf(this->edgeList[i].getSource());
        int destinationIndex = this->vertexList.indexOf(this->edgeList[i].getDestination());

        newAdjacencyMatrix[sourceIndex][destinationIndex]++;
    }
       
    this->adjacencyMatrix = newAdjacencyMatrix;
}

void Graph::updateAdjacencyList() {
    int adjSize = this->vertexList.size();
    List<List<int>> newAdjacencyList(adjSize, List<int>());

    for (int i = 0; i < adjSize; i++) 
        for (int j = 0; j < this->edgeList.size(); j++) 
            if (this->vertexList[i] == this->edgeList[j].getSource()) 
                newAdjacencyList[i].insert(this->edgeList[j].getDestination());
        
    this->adjacencyList = newAdjacencyList;
}

int Graph::getVertexDegree(int vertex) {
    int count = 0;

    for (int i = 0; i < this->edgeList.size(); i++) 
        if (this->edgeList[i].getDestination() == vertex) 
            count++;

    return count;
}

void Graph::removeVertex(int vertex) {
    List<Edge> oldEdgeList = this->edgeList;
    this->vertexList.remove(vertex);

    for (int i = 0; i < oldEdgeList.size(); i++) 
        if (oldEdgeList[i].getSource() == vertex or oldEdgeList[i].getDestination() == vertex) 
            this->edgeList.remove(oldEdgeList[i]);
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
    return this->vertexList.size();
}

int Graph::vertexAt(int index) {
    return this->vertexList[index];
}

int Graph::getNumEdges() {
    return this->edgeList.size();
}

Edge Graph::edgeAt(int index) {
    return this->edgeList[index];
}

Edge Graph::edgeAt(int source, int destination) {
    for (int i = 0; i < this->edgeList.size(); i++) {
        Edge edge = this->edgeList[i];

        if (edge.getSource() == source and edge.getDestination() == destination) 
            return edge;
    }

    throw runtime_error("this edge does not exists");
}

int Graph::getEdgeIndex(Edge edge) {
    return this->edgeList.indexOf(edge);
}

int Graph::getVertexIndex(int vertex) {
    return this->vertexList.indexOf(vertex);
}

bool Graph::directed() {
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

bool Graph::weighted() {
    if (this->edgeList.empty()) 
        return false;

    return this->edgeList[0].hasWeight();
}

bool Graph::connected() {
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
    int vertexIndex = graph.getVertexIndex(vertex);

    List<int> vertexList = graph.getAdjacencyList()[vertexIndex];

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.size(); i++) {
        int adjIndex, adjVertex;

        adjVertex = vertexList[i];
        adjIndex = graph.getVertexIndex(adjVertex);

        if (!visited[adjIndex]) 
            this->DFS(adjVertex, graph, visited);
    }
}

int Graph::getNumVertexOddDegree() {
    int count = 0;

    for (int i = 0; i < this->vertexList.size(); i++) 
        if (this->getVertexDegree(this->vertexList[i]) % 2 != 0) 
            count++;

    return count;
}

List<int> Graph::getVertexAloneList() {
    List<int> aloneVertexList;
    bool exist;

    for (int i = 0; i < this->vertexList.size(); i++) {
        exist = false;

        for (int j = 0; j < this->edgeList.size(); j++) 
            if (this->vertexList[i] == this->edgeList[j].getSource() or this->vertexList[i] == this->edgeList[j].getDestination()) 
                exist = true;

        if (!exist) 
            aloneVertexList.insert(this->vertexList[i]);
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
    int vertexIndex = graph.getVertexIndex(vertex);
    List<int> vertexList = graph.getAdjacencyList()[vertexIndex];

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.size(); i++) {
        int adjVertex = vertexList[i];
        int adjVertexIndex = graph.getVertexIndex(adjVertex);

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

List<List<int>> Graph::getDFSOrderLists() {
    List<List<int>> recStackList;
    List<int> recStack;
    Graph graph = *this;
    int numVertex = graph.getNumVertex();
    bool visited[numVertex];
    
    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;

    for (int i = 0; i < numVertex; i++) {
        if (!visited[i]) {
            this->DFSRecStack(graph.vertexAt(i), graph, visited, recStack);
            recStack.reverse();
            recStackList.insert(recStack);
            recStack.clear();
        }
    } 

    return recStackList;
}

void Graph::DFSRecStack(int vertex, Graph graph, bool* visited, List<int>& recStack) {
    int vertexIndex = graph.getVertexIndex(vertex);

    List<int> adjVertexList = graph.getAdjacencyList()[vertexIndex];

    visited[vertexIndex] = true;

    for (int i = 0; i < adjVertexList.size(); i++) {
        int adjVertex = adjVertexList[i];
        int adjVertexIndex = graph.getVertexIndex(adjVertex);

        if (!visited[adjVertexIndex])
            this->DFSRecStack(adjVertex, graph, visited, recStack);
    }

    recStack.insert(vertex);
}

List<List<int>> Graph::getBFSOrderLists() {
    List<List<int>> recStackList;
    List<int> queue;
    List<int> recStack;

    Graph graph = *this;
    int numVertex = graph.getNumVertex();
    bool visited[numVertex];
    
    for (int i = 0; i < numVertex; i++) 
        visited[i] = false;

    for (int i = 0; i < graph.getNumVertex(); i++) {
        if (!visited[i]) {
            this->BFSRecStack(graph.vertexAt(i), graph, visited, recStack, queue);
            recStack.reverse();
            recStackList.insert(recStack);
            recStack.clear();
        }
    } 

    return recStackList;
}

void Graph::BFSRecStack(int vertex, Graph graph, bool* visited, List<int>& recStack, List<int>& queue) {
    int vertexIndex = graph.getVertexIndex(vertex);

    List<int> vertexList = graph.getAdjacencyList()[vertexIndex];

    visited[vertexIndex] = true;

    for (int i = 0; i < vertexList.size(); i++) {
        int adjVertex = vertexList[i];
        int adjVertexIndex = graph.getVertexIndex(adjVertex);

        if (!visited[adjVertexIndex]) {
            visited[adjVertexIndex] = true;
            queue.insert(adjVertex);
        }
    }

    while (!queue.empty()) {
        int nextVisitVertex = queue.removeFirst();
        int nextVisitVertexIndex = graph.getVertexIndex(nextVisitVertex);
        this->BFSRecStack(nextVisitVertex, graph, visited, recStack, queue);
    }

    recStack.insert(vertex);
}

Graph Graph::getKruskalTree() {
    Graph kruskal(this->vertexList);
    List<Edge> edgeList = this->edgeList;

    edgeList.sort();
    
    while (!edgeList.empty()) {
        Edge next = edgeList.removeFirst();
        kruskal.addEdge(next);

        if (kruskal.haveCycle())
            kruskal.removeEdge(next);
    }

    return kruskal;
}

Graph Graph::getPrimTree() {
    List<int> queue = this->vertexList;
    int key[this->vertexList.size()];
    int parents[this->vertexList.size()];
    int u, v;

    for (int i = 0; i <  this->vertexList.size(); i++) {
        key[i] = 9999999;
        parents[i] = -1;
    }

    key[0] = 0;

    while (!queue.empty()) {
        int minKeyIndex = 0;

        for (int i = 0; i < queue.size(); i++) {
            int index = this->vertexList.indexOf(queue[i]);

            if (key[index] < key[this->vertexList.indexOf(queue[minKeyIndex])]) 
                minKeyIndex = i;
        }

        u = queue[minKeyIndex];
        queue.remove(u);

        List<int> adj = this->getAdjacencyList()[this->vertexList.indexOf(u)];

        for (int i = 0; i < adj.size(); i++) {
            v = adj[i];
            int vIndex = this->vertexList.indexOf(v);
            Edge edge = this->edgeAt(u, v);
            int weight = edge.getWeight();

            if (queue.has(v) and weight < key[vIndex]) {
                parents[vIndex] = u;
                key[vIndex] = weight;
            }
        }   
    }

    Graph prim(this->vertexList);

    for (int i = 0; i < this->vertexList.size(); i++) 
        if (parents[i] != -1) 
            prim.addEdge(Edge(parents[i], this->vertexList[i], key[i]));

    return prim;
}
