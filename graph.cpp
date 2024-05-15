#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include "graph.hpp"

using namespace std;

//
// implementacao das funcoes da classe Map
//
Map::Map(int* list, int length) {
    this->mapList = new NodeMap[length];

    for (int i = 0; i < length; i++) {
        this->mapList[i].key = this->currentkey;
        this->mapList[i].value = list[i];
        this->currentkey++;
    }
}

int Map::getPositionOf(int value) {
    for (int i = 0; i < currentkey; i++) {
        if (this->mapList[i].value == value) {
            return i;
        }
    }
    return -1;
}

//
// implementacao das funcoes da classe Graph
//
// construtor
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

// construtor de copia
Graph::Graph(Graph& other) {
    if (this != &other)  {
        this->numEdges = other.getNumEdges();
        this->numVertix = other.getNumVertix();
        this->directGraph = other.isDirected();

        Edge *newEdgeList = new Edge[this->numEdges];
        for (int i = 0; i < this->numEdges; i++) {
            newEdgeList[i] = other.edgeAt(i);
        }
        this->edgeList = newEdgeList;

        int *newVertixList = new int[this->numVertix];
        for (int i = 0; i < this->numVertix; i++) {
            newVertixList[i] = other.vertixAt(i);
        }
        this->vertixList = newVertixList; 
    }
}

// destrutor
Graph::~Graph() {
    this->deleteGraph();
}

void Graph::printAdjacencyMatrix() {
    this->updateAdjacencyMatrix();

    cout << "  ";
    for (int i = 0; i < this->numVertix; i++) {
        cout << this->vertixList[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < this->numVertix; i++) {
        cout << this->vertixList[i] << " ";
        for (int j = 0; j < this->numVertix; j++) {
            cout << this->adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::printAdjacencyList() {
    this->updateAdjacencyList();

    for (int i = 0; i < this->numVertix; i++) {
        Node *aux = this->adjacencyList[i];

        cout << aux->vertix << " -> ";
        aux = aux->next;

        while (aux != nullptr) {
            cout << aux->vertix << " ";
            aux = aux->next;
        }
        cout << endl;
    }
}

void Graph::printFormatedData() {
    cout << "V = {";
    for (int i = 0; i < this->numVertix; i++) {
        cout << this->vertixList[i];
        if (i != this->numVertix - 1) {
            cout << ",";
        } 
    }
    cout << "}; A = {";
    for (int i = 0; i < this->numEdges; i++) {
        cout << "(" << this->edgeList[i].vertix1 << "," << this->edgeList[i].vertix2 << ")";
        if (i != this->numEdges - 1) {
            cout << ",";
        } 
    }
    cout << "};" << endl;
}

void Graph::removeEdge(Edge edge) {
    if (!this->hasEdge(edge)) {
        return;
    }

    Edge *newEdgeList = new Edge[this->numEdges-1];
    int j = 0;
    
    for (int i = 0; i < this->numEdges; i++) {
        if (this->edgeList[i].vertix1 != edge.vertix1 or this->edgeList[i].vertix2 != edge.vertix2) {
            newEdgeList[j].vertix1 = this->edgeList[i].vertix1;
            newEdgeList[j].vertix2 = this->edgeList[i].vertix2;
            j++;
        }
    }

    this->numEdges--;
    delete[] this->edgeList;
    this->edgeList = newEdgeList;
}

void Graph::addEdge(Edge edge) {
    Edge *newEdgeList = new Edge[this->numEdges+1]; 
    exception insertionError;

    if (!this->hasVertix(edge.vertix1) or !this->hasVertix(edge.vertix2)) {
        throw insertionError;
    }  

    for (int i = 0; i < this->numEdges; i++) {
        newEdgeList[i] = this->edgeList[i];
    } 
    newEdgeList[this->numEdges] = edge;

    this->numEdges++;
    delete[] this->edgeList;
    this->edgeList = newEdgeList;
}

void Graph::removeVertix(int vertix) {
    if (!this->hasVertix(vertix)) {
        return;
    }

    int *newVertixList = new int[this->numVertix-1];
    int j = 0;
    
    for (int i = 0; i < this->numVertix; i++) {
        if (this->vertixList[i] != vertix) {
            newVertixList[j++] = this->vertixList[i];
        }
    }

    for (int i = 0; i < this->numEdges; i++) {
        if (this->edgeList[i].vertix1 == vertix) { 
            this->removeEdge(this->edgeList[i]);
            i--;
        }

        if (this->edgeList[i].vertix2 == vertix) {
            this->removeEdge(this->edgeList[i]);
            i--;
        }
    }

    this->numVertix--;
    delete[] this->vertixList;
    this->vertixList = newVertixList;
}

void Graph::addVertix(int vertix) {
    int *newVertixList = new int[this->numVertix+1];   

    for (int i = 0; i < this->numVertix; i++) {
        newVertixList[i] = this->vertixList[i];
    } 
    newVertixList[this->numVertix] = vertix;

    this->numVertix++;
    delete[] this->vertixList;
    this->vertixList = newVertixList;
}

bool Graph::hasEdge(Edge edge) {
    for (int i = 0; i < this->numEdges; i++) {
        if (this->edgeList[i].vertix1 == edge.vertix1 and this->edgeList[i].vertix2 == edge.vertix2) {
            return true;
        }
    }
    return false;
}

bool Graph::hasVertix(int vertix) {
    for (int i = 0; i < this->numVertix; i++) {
        if (this->vertixList[i] == vertix) {
            return true;
        }
    }
    return false;
}

void Graph::updateAdjacencyMatrix() {
    // antes de criar uma nova
    // preciso deletar os dados da antiga
    // preciso implementar isso 
    adjacencyMatrix = new int*[this->numVertix];

    for (int i = 0; i < this->numVertix; i++) {
        adjacencyMatrix[i] = new int[this->numVertix];
    }

    for (int i = 0; i < this->numVertix; i++) {
        for (int j = 0; j < this->numVertix; j++) {
            this->adjacencyMatrix[i][j] = 0;
        }
    }

    Map m(this->vertixList, this->numVertix);

    for (int i = 0; i < numEdges; i++) {
        
        this->adjacencyMatrix[m.getPositionOf(this->edgeList[i].vertix1)][m.getPositionOf(this->edgeList[i].vertix2)] = 1;

        if (!this->directGraph) {
            this->adjacencyMatrix[m.getPositionOf(this->edgeList[i].vertix2)][m.getPositionOf(this->edgeList[i].vertix1)] = 1;
        }
    }   
}

void Graph::updateAdjacencyList() {
    // antes de criar uma nova
    // preciso deletar os dados da antiga
    // preciso implementar isso 
    this->adjacencyList = new Node*[numVertix];

    Map m(this->vertixList, this->numVertix);

    for (int i = 0; i < numVertix; i++) {
        Node *head = new Node;
        head->vertix = vertixList[i];
        head->next = nullptr;
        adjacencyList[i] = head;
    }

    for (int i = 0; i < numVertix; i++) {
        for (int j = 0; j < numEdges; j++) {
            if (this->edgeList[j].vertix1 == vertixList[i]) {
                Node *newNode = new Node;
                newNode->vertix = this->edgeList[j].vertix2;
                newNode->next = nullptr;

                Node *aux = adjacencyList[i];
                while (aux->next != nullptr) {
                    aux = aux->next;
                }

                aux->next = newNode;

                if (!this->directGraph) {
                    Node *newNode1 = new Node;
                    newNode1->vertix = this->edgeList[j].vertix1;
                    newNode1->next = nullptr;

                    aux = adjacencyList[m.getPositionOf(this->edgeList[j].vertix2)];
                    while (aux->next != nullptr) {
                        aux = aux->next;
                    }

                    aux->next = newNode1;
                }
            }
        }
    }
}

void Graph::deleteGraph() {
    this->numEdges = 0;
    this->numVertix = 0;
    delete[] this->vertixList;
    delete[] this->edgeList;

    for (int i = 0; i < this->numVertix; i++) {
        delete[] this->adjacencyList[i];
        delete[] this->adjacencyMatrix[i];
    }

    delete[] this->adjacencyList;
    delete[] this->adjacencyMatrix;
}

bool Graph::isDirected() {
    return this->directGraph;
}

int Graph::getNumVertix() {
    return this->numVertix;
}

int Graph::vertixAt(int index) {
    if (index >= 0 && index < this->numVertix) {
        return this->vertixList[index];
    } else {
        return -1;
    }
}

int Graph::getNumEdges() {
    return this->numEdges;
}

Edge Graph::edgeAt(int index) {
    Edge edge;

    edge.vertix1 = -1;
    edge.vertix2 = -1;

    if (index >= 0 && index < this->numEdges) {
        edge.vertix1 = this->edgeList[index].vertix1;
        edge.vertix2 = this->edgeList[index].vertix2;
    } 

    return edge;
}

void Graph::updateVertixAloneList() {
    if (this->vertixAloneList != nullptr) 
        delete[] this->vertixAloneList;

    bool exist;
    int *vertixAloneList = nullptr;
    int numAloneVertix = 0;

    for (int i = 0; i < this->numVertix; i++) {
        exist = false;

        for (int j = 0; j < this->numEdges and !exist; j++) {
            if (this->vertixList[i] == this->edgeList[j].vertix1 or this->vertixList[i] == this->edgeList[j].vertix2) {
                exist = true;
            }
        }

        if (!exist) {
            int *newVertixAloneList = new int[numAloneVertix+1];

            for (int j = 0; j < numAloneVertix; j++) {
                newVertixAloneList[j] = vertixAloneList[j];
            }

            newVertixAloneList[numAloneVertix++] = this->vertixList[i];
            delete[] vertixAloneList;
            vertixAloneList = newVertixAloneList;
        }
    }

    this->vertixAloneList = vertixAloneList;
    this->numAloneVertix = numAloneVertix;
}

int* Graph::getAloneVertixList() {
    this->updateVertixAloneList();
    int* copyAloneVertixList = new int[this->numAloneVertix];

    for (int i = 0; i < this->numAloneVertix; i++) {
        copyAloneVertixList[i] = this->vertixAloneList[i];
    }

    return copyAloneVertixList;
}

int Graph::getNumAloneVertix() {
    this->updateVertixAloneList();
    return this->numAloneVertix;
}