#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <string>
#include <ctime>
#include "graph.hpp"

using namespace std;

#ifdef _WIN32
    #define OPEN_IMAGE_COMMAND "(algum comando que funcione no windows) ./images/last.png"
#else
    #define OPEN_IMAGE_COMMAND "xdg-open ./images/last.png > /dev/null 2>&1 &"
#endif

void generateGraphDotFile(Graph graph);
void generateTextFile(Graph graph);
string generateFileName(string extension, bool digraph);
bool isConnected(Graph graph);
void DFS(int vertix, Graph graph, List<bool>& visited);


void generateGraphDotFile(Graph graph) {
    List<int> vertixAloneList;
    ofstream output;
    Edge edge;
    bool exist;

    string fileName = generateFileName("dot", graph.isDirected());
    string imageName = generateFileName("png", graph.isDirected());

    output.open("./dot/" + fileName, ios::trunc);

    if (graph.isDirected()) {
        output << "digraph {";
        for (int i = 0; i < graph.getNumEdges(); i++) {
            edge = graph.edgeAt(i);
            output << edge.vertix1 << " -> " << edge.vertix2 << ";";
        }
    } else {
        Graph aux;

        output << "graph {";
        for (int i = 0; i < graph.getNumEdges(); i++) {
            edge = graph.edgeAt(i);

            if (!aux.hasEdge(edge, true)) output << edge.vertix1 << " -- " << edge.vertix2 << ";";

            aux.addVertix(edge.vertix1);
            aux.addVertix(edge.vertix2);
            aux.addEdge(edge);
        }
    }

    for (int i = 0; i < graph.getNumVertix(); i++) {
        exist = false;
        for (int j = 0; j < graph.getNumEdges() and !exist; j++) {
            if (graph.vertixAt(i) == graph.edgeAt(j).vertix1 or graph.vertixAt(i) == graph.edgeAt(j).vertix2) exist = true;
        }
        if (!exist) vertixAloneList.insert(graph.vertixAt(i));
    }

    for (int i = 0; i < vertixAloneList.length(); i++) {
        output << vertixAloneList.at(i) << ";";
    }       

    output << "}";
    output.close();

    // salvar todas as imagems com o mesmo nome e abrir para visualizacao
    string command1 = "dot -Tpng ./dot/" + fileName + " -o ./images/last.png && ";
    command1 += OPEN_IMAGE_COMMAND;

    // salvar todas as imagens com nomes direfentes
    string command2 = "dot -Tpng ./dot/" + fileName + " -o ./images/" + imageName;

    system(command1.c_str());
    system(command2.c_str());
    cout << "image created successfully" << endl;
}

void generateTextFile(Graph graph) {
    ofstream output1;
    ofstream output2;
    string buffer;

    output1.open("./text/" + generateFileName("txt", graph.isDirected()), ios::trunc);
    output2.open("./text/last.txt", ios::trunc);

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

    if (ltm->tm_hour < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_hour); // hora
    dateTime += ":";

    if (ltm->tm_min < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_min); // minuto
    dateTime += ":";

    if (ltm->tm_sec < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_sec); // segundo
    dateTime += "_";

    if (ltm->tm_mday < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_mday); // dia
    dateTime += "-";

    if (ltm->tm_mon < 10) dateTime += "0"; 
    dateTime += to_string(ltm->tm_mon); // mes
    dateTime += "-";

    dateTime += to_string(1900 + ltm->tm_year); // ano

    fileName += dateTime;
    fileName += '.';
    fileName += extension;

    return fileName;
}

bool isConnected(Graph graph) {
    List<bool> visited;

    for (int i = 0; i < graph.getNumVertix(); i++) {
        visited.insert(false);
    }
    
    DFS(graph.vertixAt(0), graph, visited);

    for (int i = 0; i < graph.getNumVertix(); i++) {
        if (!visited.at(i)) return false;
    }

    return true;
}

void DFS(int vertix, Graph graph, List<bool>& visited) {
    List<List<int>> adjacencyList = graph.getAdjacencyList();
    List<int> vertixList = adjacencyList.at(graph.indexOfVertix(vertix));

    visited.insertAt(graph.indexOfVertix(vertix), true);

    for (int i = 0; i < vertixList.length(); i++) {
        if (!visited.at(graph.indexOfVertix(vertixList.at(i)))) DFS(vertixList.at(i), graph, visited);
    }
}