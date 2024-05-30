#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <string>
#include <ctime>
#include "graph.hpp"
#include "digraph.hpp"

using namespace std;

// commands for opening generated image in each operational system
#ifdef _WIN32
    // windows command
    #define OPEN_IMAGE_COMMAND "start"
    #define OUTPUT_BUFFER "";
#else
    // linux command
    #define OPEN_IMAGE_COMMAND "xdg-open"
    // directed output for a null file
    #define OUTPUT_BUFFER " > /dev/null 2> /dev/null &"
#endif

// rgb random colors
const int numColors = 20;

// this aproach will be replaced for a random rgb generator function
const string colors[numColors] = {
    "#33a8c7","#a0e426","#9336fd","#fdf148","#ffab00",
    "#f77976","#f77976","#f050ae","#f050ae","#d883ff",
    "#52e3e1","#52e3e1","#00f5d4","#00bbf9","#f15bb50",
    "#9b5de5","#00bbf9","#00f5d4","#00bbf9","#f15bb50"
};

// functions prototypes
Graph* generateGraphFromFile(string filePath, bool directGraph);
void generateGraphImage(Graph graph, string engine = "fdp", string title = "", List<List<int>>* components = nullptr);
void generateGraphText(Graph graph, string filePath);
string generateGraphFileName(string extension, bool digraph);


// open the file in filepath and parse it in a new graph object
Graph* generateGraphFromFile(string filePath, bool directGraph) {
    ifstream file;
    exception e;
    Graph* graph = nullptr;
    List<int> vertexList;
    List<Edge> edgeList;

    string s;
    stringstream buffer;
    int vertex, source, destination, weight;
    char trash;

    file.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        file.open(filePath);
    } catch (ifstream::failure fileError) {
        throw fileError;
    }

    // "V = {x,y};"
    getline(file, s, '{');
    if (s != "V = ") throw e;
    getline(file, s, ';');
    buffer.str(s);

    do { 
        buffer >> vertex >> trash; 
        if (buffer.fail() or (trash != ',' && trash != '}')) throw e;
        vertexList.insert(vertex);
    } while (trash != '}');


    // "A = {x,y};" or "A = {(x,y),(w,x),(y,z)};" or "A = {};"
    getline(file, s, '{');
    if (s != " A = ") throw e;
    getline(file, s, ';');
    buffer.str(s);

    if (s.length() == 4) { 
        buffer >> source >> trash >> destination;
        if (buffer.fail()) throw e;
        edgeList.insert(Edge(source, destination));
    } else if (s.length() > 4) {
        do { 
            buffer >> trash >> source >> trash >> destination >> trash >> trash;
            if (buffer.fail()) throw e;
            edgeList.insert(Edge(source, destination));
        } while (trash != '}');
    }

    // "P = {10,20,30,40,50};"
    try {   
        getline(file, s, '{');
    } catch (exception e) {
        file.close();
        directGraph ? graph = new Digraph(vertexList, edgeList) : graph = new Graph(vertexList, edgeList);   
        return graph; 
    }

    string cp;
    for (int i = 0; i < s.length(); i++) 
        if (s[i] != ' ') cp += s[i];
    
    if (cp.length() == 0) {
        file.close();
        directGraph ? graph = new Digraph(vertexList, edgeList) : graph = new Graph(vertexList, edgeList);   
        return graph; 
    }

    if (s != " P = ") throw e;
    List<int> weightList;
    getline(file, s, ';');
    buffer.str(s);

    do { 
        buffer >> weight >> trash;
        if (buffer.fail() or (trash != ',' && trash != '}')) 
            throw e;
        weightList.insert(weight);
    } while (trash != '}');

    file.close();
    directGraph ? graph = new Digraph(vertexList, edgeList, weightList) : graph = new Graph(vertexList, edgeList, weightList); 
    return graph;
}

// generate a image from graph object
void generateGraphImage(Graph graph, string engine, string title, List<List<int>>* components) {
    List<int> vertexList = graph.getVertexList();
    List<int> aloneVertexList = graph.getVertexAloneList();
    List<Edge> edgeList = graph.getEdgeList();

    string command;
    string imagePath = "./data/images/" + generateGraphFileName("png", graph.directed());
    string dotFilePath = "./data/dot/" + generateGraphFileName("dot", graph.directed());
    ofstream output(dotFilePath, ios::trunc);

    graph.directed() ? output << "digraph {" : output << "graph {";
    output << "label=\""+ title + "\"\nlabelloc = t;sep=\"0.8\";";

    // components 
    if (components != nullptr) {
        int numComponents = components->size();

        if (numComponents > numColors) {
            cout << "there are not enough colors to print the graph" << endl;
            return;
        }

        for (int i = 0; i < numComponents; i++) {
            List<int> component = components->at(i);

            for (int j = 0; j < component.size(); j++)
                output << component[j] <<" [fillcolor=\"" << colors[i] << "\" style=filled];";
        }
    }
    
    while (edgeList.size() > 0) {
        Edge next = edgeList.removeFirst();

        output << next.getSource();
        graph.directed() ? output << " -> " : output << " -- ";
        output << next.getDestination();

        if (next.hasWeight())
            output << "[label=\"" << next.getWeight() << "\"]";
        output << ";";

        if (!graph.directed()) {
            next.reverse();
            edgeList.remove(next);
        }
    }

    for (int i = 0; i < aloneVertexList.size(); i++) 
        output << aloneVertexList[i] << ";";
    
    output << "}";
    output.close();

    command = engine + " -Tpng " + dotFilePath + " -o ./data/images/last.png";
    system(command.c_str());

    command = engine + " -Tpng " + dotFilePath + " -o " + imagePath + " && " + OPEN_IMAGE_COMMAND + " " + imagePath + OUTPUT_BUFFER;
    system(command.c_str());

    cout << "image created successfully" << endl;
}

// generate text file from graph object
void generateGraphText(Graph graph, string filePath) {
    List<int> vertexList = graph.getVertexList();
    List<Edge> edgeList;
    List<int> weightList;

    ofstream output(filePath, ios::trunc);
    stringstream buffer;

    // get edges and weights
    // do not collect multiple edges on a graph
    while (graph.getNumEdges() > 0) {
        Edge edge = graph.edgeAt(0);

        edgeList.insert(edge);

        if (graph.weighted()) 
            weightList.insert(edge.getWeight());

        if (!graph.directed()) {
            edge.reverse();
            graph.removeEdge(edge);
        }

        graph.removeEdge(edge);
    }

    // vertex
    buffer << "V = {";

    for (int i = 0; i < vertexList.size(); i++) {
        buffer << vertexList[i];

        if (i != vertexList.size() - 1) 
            buffer << ",";
    }

    // edges
    buffer << "}; A = {";

    for (int i = 0; i < edgeList.size(); i++) {
        buffer << "(" << edgeList[i].getSource() << "," << edgeList[i].getDestination() << ")";
        if (i != edgeList.size() - 1) buffer << ",";
    }
    
    buffer << "};";

    // weights
    if (weightList.size() > 0) {
        buffer << " P = {";

        for (int i = 0; i < weightList.size(); i++) {
            buffer << weightList[i];
            if (i != weightList.size() - 1) buffer << ",";
        }

        buffer << "};";
    }

    output << buffer.str();
    output.close();
}

// generates a graph filename using datetime
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
