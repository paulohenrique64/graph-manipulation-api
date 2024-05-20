#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <string>
#include <ctime>
#include "graph.hpp"
#include "digraph.hpp"

using namespace std;

#ifdef _WIN32
    #define OPEN_IMAGE_COMMAND "(algum comando que funcione no windows) ./images/last.png"
#else
    #define OPEN_IMAGE_COMMAND "xdg-open ./images/last.png > /dev/null 2>&1 &"
#endif

bool isConnected(Graph graph);
void DFS(int vertix, Graph graph, List<bool>& visited);
Graph* generateGraphFromFile(string filePath, bool directGraph);
void generateGraphDotFile(Graph graph);
void generateTextFile(Graph graph);
string generateFileName(string extension, bool digraph);


// se o grafo recebido for um digrafo, ele deve ser convertido em um grafo para verificar se eh conexo
bool isConnected(Graph graph) {
    List<bool> visited;
    List<int> vertixList = graph.getVertixList();
    List<Edge> edgeList = graph.getEdgeList();
    Graph newGraph(vertixList, edgeList); 

    for (int i = 0; i < vertixList.length(); i++) visited.insert(false);

    DFS(newGraph.vertixAt(0), newGraph, visited);

    for (int i = 0; i < visited.length(); i++) 
        if (!visited.at(i)) return false;

    return true;
}

void DFS(int vertix, Graph graph, List<bool>& visited) {
    List<List<int>> adjacencyList = graph.getAdjacencyList();
    List<int> vertixList = adjacencyList.at(graph.indexOfVertix(vertix));

    visited.insertAt(graph.indexOfVertix(vertix), true);

    for (int i = 0; i < vertixList.length(); i++) 
        if (!visited.at(graph.indexOfVertix(vertixList.at(i)))) DFS(vertixList.at(i), graph, visited);
}

Graph* generateGraphFromFile(string filePath, bool directGraph) {
    ifstream graphFile;
    string buffer;
    Graph* newGraph = nullptr;
    List<int> vertixList;
    List<Edge> edgeList;

    Edge edge;
    int vertix;
    char trash;

    graphFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        graphFile.open(filePath);
    } catch (ifstream::failure fileError) {
        throw fileError;
    }

    // tenta fazer o parse de "V = {x,y};"
    getline(graphFile, buffer, '{');
    if (buffer != "V = ") throw runtime_error("parse error");
    getline(graphFile, buffer, ';');
    stringstream stringBuffer(buffer);

    do { 
        stringBuffer >> vertix;
        stringBuffer >> trash;

        if (stringBuffer.fail() or (trash != ',' && trash != '}')) throw runtime_error("parse error");

        vertixList.insert(vertix);
    } while (trash != '}');


    // tenta fazer o parse de "A = {x,y};" / "A = {(x,y),(w,x),(y,z)};" / "A = {};"
    getline(graphFile, buffer, '{');
    if (buffer != " A = ") throw runtime_error("parse error");
    getline(graphFile, buffer, ';');
    stringstream stringBuffer1(buffer);

    if (buffer.length() == 4) { 
        stringBuffer1 >> vertix;
        stringBuffer1 >> trash;
        edge.vertix1 = vertix;

        stringBuffer1 >> vertix;
        edge.vertix2 = vertix;

        if (stringBuffer1.fail()) throw runtime_error("parse error");

        edgeList.insert(edge);
    } else if (buffer.length() > 4) {
        do { 
            stringBuffer1 >> trash;
            stringBuffer1 >> vertix;
            edge.vertix1 = vertix;

            stringBuffer1 >> trash;
            stringBuffer1 >> vertix;
            edge.vertix2 = vertix;

            stringBuffer1 >> trash;
            stringBuffer1 >> trash;

            if (stringBuffer1.fail()) throw runtime_error("parse error");

            edgeList.insert(edge);
        } while (trash != '}');
    }

    if (directGraph) {
        newGraph = new Digraph(vertixList, edgeList);
    } else {
        newGraph = new Graph(vertixList, edgeList);
    }

    return newGraph;
}

void generateGraphDotFile(Graph graph) {
    List<int> vertixList = graph.getVertixList();
    List<Edge> edgeList = graph.getEdgeList();
    List<Edge> edgeAlreadyInFile;

    ofstream output;
    string fileName = generateFileName("dot", graph.isDirected());
    string imageName = generateFileName("png", graph.isDirected());
    output.open("./dot/" + fileName, ios::trunc);

    if (graph.isDirected()) {
        output << "digraph {";
        for (int i = 0; i < edgeList.length(); i++) output << edgeList.at(i).vertix1 << " -> " << edgeList.at(i).vertix2 << ";";
    } else {
        output << "graph {";
        for (int i = 0; i < edgeList.length(); i++) {
            Edge reverseEdge;
            reverseEdge.vertix1 = edgeList.at(i).vertix2;
            reverseEdge.vertix2 = edgeList.at(i).vertix1;
            if (!edgeAlreadyInFile.has(edgeList.at(i)) and !edgeAlreadyInFile.has(reverseEdge)) 
                output << edgeList.at(i).vertix1 << " -- " << edgeList.at(i).vertix2 << ";";
            edgeAlreadyInFile.insert(edgeList.at(i));
            edgeAlreadyInFile.insert(reverseEdge);
        }
    }

    bool exist;
    for (int i = 0; i < vertixList.length(); i++) {
        exist = false;

        for (int j = 0; j < edgeList.length(); j++) 
            if (vertixList.at(i) == edgeList.at(j).vertix1 or vertixList.at(i) == edgeList.at(j).vertix2) exist = true;

        if (!exist) output << vertixList.at(i) << ";";
    }    

    output << "}";
    output.close();

    // gerar imagem a partir do arquivo dot e abrir para visualizacao
    // last.png
    string command1 = "dot -Tpng ./dot/" + fileName + " -o ./images/last.png && ";
    command1 += OPEN_IMAGE_COMMAND;

    // nome formatado
    string command2 = "dot -Tpng ./dot/" + fileName + " -o ./images/" + imageName;

    // executar os comandos descritos acima
    system(command1.c_str());
    system(command2.c_str());

    cout << "image created successfully" << endl;
}

void generateTextFile(Graph graph) {
    ofstream output1;
    ofstream output2;
    string buffer;

    buffer += "V = {";

    for (int i = 0; i < graph.getNumVertix(); i++) {
        buffer += to_string(graph.vertixAt(i));

        if (i != graph.getNumVertix() - 1) {
            buffer += ",";
        }
    }

    buffer += "}; A = {";

    for (int i = 0; i < graph.getNumEdges(); i++) {
        buffer += "(";
        buffer += to_string(graph.edgeAt(i).vertix1);
        buffer += ",";
        buffer += to_string(graph.edgeAt(i).vertix2);
        buffer += ")";

        if (i != graph.getNumEdges() - 1) {
            buffer += ",";
        }
    }
    
    buffer += "};";

    output1.open("./text/" + generateFileName("txt", graph.isDirected()), ios::trunc);
    output2.open("./text/last.txt", ios::trunc);

    output1 << buffer;
    output2 << buffer;

    output1.close();
    output2.close();
}

string generateFileName(string extension, bool digraph) {
    string dateTime, fileName;
    time_t now = time(0); 
    tm *ltm = localtime(&now);

    if (digraph) {
        fileName = "digraph_";
    } else {
        fileName = "graph_";
    }

    if (ltm->tm_mday < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_mday); // dia
    dateTime += "-";

    if (ltm->tm_mon < 10) dateTime += "0"; 
    dateTime += to_string(ltm->tm_mon); // mes
    dateTime += "-";

    dateTime += to_string(1900 + ltm->tm_year); // ano
    dateTime += "_";

    if (ltm->tm_hour < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_hour); // hora
    dateTime += ":";

    if (ltm->tm_min < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_min); // minuto
    dateTime += ":";

    if (ltm->tm_sec < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_sec); // segundo

    fileName += dateTime;
    fileName += '.';
    fileName += extension;

    return fileName;
}