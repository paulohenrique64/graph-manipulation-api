#ifdef _WIN32
    #define OPEN_IMAGE_COMMAND "(algum comando que funcione no windows) ./images/last.png"
#else
    #define OPEN_IMAGE_COMMAND "xdg-open ./images/last.png > /dev/null 2>&1 &"
#endif

#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <string>
#include <ctime>
#include "graph.hpp"
#include "digraph.hpp"

using namespace std;

Graph* generateGraphFromFile(string filePath, bool directGraph);
void generateGraphDotFile(Graph graph);
void generateGraphImage(string dotFilePath, string imagePath);
void generateGraphTextFile(Graph graph);
string generateGraphFileName(string extension, bool digraph);


Graph* generateGraphFromFile(string filePath, bool directGraph) {
    ifstream graphFile;
    exception e;
    Graph* newGraph = nullptr;
    List<int> vertexList;
    List<Edge> edgeList;
    List<int> weightList;

    string stringBuffer;
    stringstream streamBuffer;
    int vertex, source, destination, weight;
    char trash;

    graphFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        graphFile.open(filePath);
    } catch (ifstream::failure fileError) {
        throw fileError;
    }


    // "V = {x,y};"
    getline(graphFile, stringBuffer, '{');
    if (stringBuffer != "V = ") throw e;
    getline(graphFile, stringBuffer, ';');
    streamBuffer.str(stringBuffer);

    do { 
        streamBuffer >> vertex >> trash; 
        if (streamBuffer.fail() or (trash != ',' && trash != '}')) throw e;
        vertexList.insert(vertex);
    } while (trash != '}');


    // "A = {x,y};" ou "A = {(x,y),(w,x),(y,z)};" ou "A = {};"
    getline(graphFile, stringBuffer, '{');
    if (stringBuffer != " A = ") throw e;
    getline(graphFile, stringBuffer, ';');
    streamBuffer.str(stringBuffer);

    if (stringBuffer.length() == 4) { 
        streamBuffer >> source >> trash >> destination;
        if (streamBuffer.fail()) throw e;
        edgeList.insert(Edge(source, destination));
    } else if (stringBuffer.length() > 4) {
        do { 
            streamBuffer >> trash >> source >> trash >> destination >> trash >> trash;
            if (streamBuffer.fail()) throw e;
            edgeList.insert(Edge(source, destination));
        } while (trash != '}');
    }


    // "P = {10,20,30,40,50}; se existir"
    try {
        getline(graphFile, stringBuffer, '{');
    } catch (exception e) {
        directGraph ? newGraph = new Digraph(vertexList, edgeList) : newGraph = new Graph(vertexList, edgeList);  
        return newGraph;
    }

    if (stringBuffer != " P = ") throw e;
    getline(graphFile, stringBuffer, ';');
    streamBuffer.str(stringBuffer);
    do { 
        streamBuffer >> weight >> trash;
        if (streamBuffer.fail() or (trash != ',' && trash != '}')) 
            throw e;
        weightList.insert(weight);
    } while (trash != '}');
    directGraph ? newGraph = new Digraph(vertexList, edgeList, weightList) : newGraph = new Graph(vertexList, edgeList, weightList); 

    return newGraph;
}

void generateGraphDotFile(Graph graph) {
    List<int> vertexList = graph.getVertexList(), aloneVertexList = graph.getAloneVertexList();
    List<Edge> edgeList = graph.getEdgeList(), edgeListCopy = edgeList;
    string fileName = generateGraphFileName("dot", graph.isDirected());

    ofstream output("./dot/" + fileName, ios::trunc);
    graph.isDirected() ? output << "digraph {" : output << "graph {";

    while (edgeListCopy.length() > 0) {
        output << edgeListCopy.at(0).getSource();
        graph.isDirected() ? output << " -> " : output << " -- ";
        output << edgeListCopy.at(0).getDestination();

        if (edgeListCopy.at(0).hasWeight())
            output << "[label=\"" << edgeListCopy.at(0).getWeight() << "\"]";
        output << ";";

        if (!graph.isDirected()) edgeListCopy.remove(edgeListCopy.at(0).getReverse());
        edgeListCopy.remove(edgeListCopy.at(0));
    }

    for (int i = 0; i < aloneVertexList.length(); i++) 
        output << aloneVertexList.at(i) << ";";
    
    output << "}";
    output.close();

    string imagePath = "./images/" + generateGraphFileName("png", graph.isDirected());
    string dotFilePath = "./dot/" + fileName;

    generateGraphImage(dotFilePath, imagePath);
}

void generateGraphImage(string dotFilePath, string imagePath) {
    string command1 = "fdp -Tpng " + dotFilePath + " -o ./images/last.png && " + OPEN_IMAGE_COMMAND;
    string command2 = "fdp -Tpng " + dotFilePath + " -o " + imagePath;
    
    system(command1.c_str());
    system(command2.c_str());

    cout << "image created successfully" << endl;
}

void generateGraphTextFile(Graph graph) {
    ofstream output1;
    ofstream output2;
    string buffer;

    buffer += "V = {";

    for (int i = 0; i < graph.getNumVertex(); i++) {
        buffer += to_string(graph.vertexAt(i));

        if (i != graph.getNumVertex() - 1) {
            buffer += ",";
        }
    }

    buffer += "}; A = {";

    for (int i = 0; i < graph.getNumEdges(); i++) {
        buffer += "(";
        buffer += to_string(graph.edgeAt(i).getSource());
        buffer += ",";
        buffer += to_string(graph.edgeAt(i).getDestination());
        buffer += ")";

        if (i != graph.getNumEdges() - 1) {
            buffer += ",";
        }
    }
    
    buffer += "};";

    output1.open("./text/" + generateGraphFileName("txt", graph.isDirected()), ios::trunc);
    output2.open("./text/last.txt", ios::trunc);

    output1 << buffer;
    output2 << buffer;

    output1.close();
    output2.close();
}

string generateGraphFileName(string extension, bool digraph) {
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
    dateTime += to_string(ltm->tm_mon + 1); // mes
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