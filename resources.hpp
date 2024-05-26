#ifdef _WIN32
    #define OPEN_IMAGE_COMMAND "start"
    #define OUTPUT_BUFFER "";
#else
    #define OPEN_IMAGE_COMMAND "xdg-open"
    #define OUTPUT_BUFFER " > /dev/null 2> /dev/null &"
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
void generateGraphImage(Graph graph, string engine = "fdp");
void generateGraphText(Graph graph);
string generateGraphFileName(string extension, bool digraph);


Graph* generateGraphFromFile(string filePath, bool directGraph) {
    ifstream graphFile;
    exception e;
    Graph* newGraph = nullptr;
    List<int> vertexList;
    List<Edge> edgeList;

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
    List<int> weightList;

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

void generateGraphImage(Graph graph, string engine) {
    List<int> vertexList = graph.getVertexList(), aloneVertexList = graph.getAloneVertexList();
    List<Edge> edgeList = graph.getEdgeList(), edgeListCopy = edgeList;

    string imageName = generateGraphFileName("png", graph.isDirected());
    string fileName = generateGraphFileName("dot", graph.isDirected());
    ofstream output("./dot/" + fileName, ios::trunc);

    graph.isDirected() ? output << "digraph {" : output << "graph {";

    while (edgeListCopy.length() > 0) {
        Edge next = edgeListCopy.removeFirst();

        output << next.getSource();
        graph.isDirected() ? output << " -> " : output << " -- ";
        output << next.getDestination();

        if (next.hasWeight())
            output << "[label=\"" << next.getWeight() << "\"]";
        output << ";";

        if (!graph.isDirected()) {
            next.reverse();
            edgeListCopy.remove(next);
        }
    }

    for (int i = 0; i < aloneVertexList.length(); i++) 
        output << aloneVertexList.at(i) << ";";
    
    output << "}";
    output.close();

    string imagePath = "./images/" + imageName;
    string dotFilePath = "./dot/" + fileName;

    string command1 = engine + " -Tpng " + dotFilePath + " -o ./images/last.png";
    string command2 = engine + " -Tpng " + dotFilePath + " -o " + imagePath + " && " + OPEN_IMAGE_COMMAND + " " + imagePath + OUTPUT_BUFFER;
    
    system(command1.c_str());
    system(command2.c_str());

    cout << "image created successfully" << endl;
}

void generateGraphText(Graph graph) {
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
    dateTime += "-";

    if (ltm->tm_min < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_min); // minuto
    dateTime += "-";

    if (ltm->tm_sec < 10) dateTime += "0";
    dateTime += to_string(ltm->tm_sec); // segundo

    fileName += dateTime;
    fileName += '.';
    fileName += extension;

    return fileName;
}