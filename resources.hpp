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

const int numColors = 20;
const string colors[numColors] = {
    "#33a8c7","#a0e426","#9336fd","#fdf148","#ffab00",
    "#f77976","#f77976","#f050ae","#f050ae","#d883ff",
    "#52e3e1","#52e3e1","#00f5d4","#00bbf9","#f15bb50",
    "#9b5de5","#00bbf9","#00f5d4","#00bbf9","#f15bb50"
};

Graph* generateGraphFromFile(string filePath, bool directGraph);
void generateGraphImage(Graph graph, string engine = "fdp", string title = "", List<List<int>>* components = nullptr);
void generateGraphText(Graph graph);
string generateGraphFileName(string extension, bool digraph);

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


    // "A = {x,y};" ou "A = {(x,y),(w,x),(y,z)};" ou "A = {};"
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

    // "P = {10,20,30,40,50}; se existir"
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

void generateGraphImage(Graph graph, string engine, string title, List<List<int>>* components) {
    List<int> vertexList = graph.getVertexList(), aloneVertexList = graph.getAloneVertexList();
    List<Edge> edgeList = graph.getEdgeList(), edgeListCopy = edgeList;

    string imageName = generateGraphFileName("png", graph.isDirected());
    string fileName = generateGraphFileName("dot", graph.isDirected());
    ofstream output("./dot/" + fileName, ios::trunc);

    graph.isDirected() ? output << "digraph {" : output << "graph {";
    output << "label=\""+ title + "\"\nlabelloc = t;sep=\"0.8\";";

    // components (if exists)
    if (components != nullptr) {
        int numComponents = components->length();

        if (numComponents > numColors) {
            cout << "there are not enough colors to print the graph" << endl;
            return;
        }

        for (int i = 0; i < numComponents; i++) {
            List<int> component = components->at(i);

            for (int j = 0; j < component.length(); j++)
                output << component.at(j) <<" [fillcolor=\"" << colors[i] << "\" style=filled];";
        }
    }
    
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

void generateGraphSCCImage(Graph graph, List<List<int>> components, string engine, string title) {
    List<int> vertexList = graph.getVertexList(), aloneVertexList = graph.getAloneVertexList();
    List<Edge> edgeList = graph.getEdgeList(), edgeListCopy = edgeList;

    string imageName = generateGraphFileName("png", graph.isDirected());
    string fileName = generateGraphFileName("dot", graph.isDirected());
    ofstream output("./dot/" + fileName, ios::trunc);

    graph.isDirected() ? output << "digraph {" : output << "graph {";
    output << "label=\""+ title + "\"\nlabelloc = t;sep=\"0.8\";";



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