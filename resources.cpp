#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>
#include <ctime>
#include "resources.hpp"
#include "graph.hpp"

using namespace std;

void generateGraphDotFile(Graph graph) {
    // data e hora atual baseado no sistema
    time_t now = time(0);

    tm *ltm = localtime(&now);

    string dotFileName;
    
    if (graph.isDirected()) {
        dotFileName = "digraph_";
    } else {
        dotFileName = "graph_";
    }

    // hora
    if (ltm->tm_hour < 10) dotFileName += "0";
    dotFileName += to_string(ltm->tm_hour);
    dotFileName += ":";

    // minuto
    if (ltm->tm_min < 10) dotFileName += "0";
    dotFileName += to_string(ltm->tm_min);
    dotFileName += ":";

    // segundo
    if (ltm->tm_sec < 10) dotFileName += "0";
    dotFileName += to_string(ltm->tm_sec);
    dotFileName += "_";

    // dia
    if (ltm->tm_mday < 10) dotFileName += "0";
    dotFileName += to_string(ltm->tm_mday);
    dotFileName += "-";

    // mes
    if (ltm->tm_mon < 10) dotFileName += "0";
    dotFileName += to_string(ltm->tm_mon);
    dotFileName += "-";

    // ano
    dotFileName += to_string(1900 + ltm->tm_year);

    // nome da imagem
    string imageName = dotFileName;

    // formato
    dotFileName += ".dot";
    imageName += ".png";

    ofstream output("./dot/" + dotFileName, ios::trunc);

    if (graph.isDirected()) {
        output << "digraph {";
        for (int i = 0; i < graph.getNumEdges(); i++) {
            output << graph.edgeAt(i).vertix1 << " -> " << graph.edgeAt(i).vertix2 << ";";
        }
    } else {
        output << "graph {";
        for (int i = 0; i < graph.getNumEdges(); i++) {
            output << graph.edgeAt(i).vertix1 << " -- " << graph.edgeAt(i).vertix2 << ";";
        }
    }

    int *vertixAloneList = graph.getAloneVertixList();
    for (int i = 0; i < graph.getNumAloneVertix(); i++) {
        output << vertixAloneList[i] << ";";
    }       
    delete[] vertixAloneList;   

    output << "}";
    output.close();

    // salvar todas as imagems com o mesmo nome e abrir para visualizacao
    string command1 = "dot -Tpng ./dot/" + dotFileName + " -o ./images/last.png && ";
    command1 += OPEN_IMAGE_COMMAND;

    // salvar todas as imagens com nomes direfentes
    string command2 = "dot -Tpng ./dot/" + dotFileName + " -o ./images/" + imageName;

    system(command1.c_str());
    system(command2.c_str());
    cout << "image created successfully" << endl;
}

void generateTextFile(Graph graph) {
    time_t now = time(0); // data e hora atual baseado no sistema
    ofstream output1;
    ofstream output2;
    stringstream buffer;
    string auxBuffer, textFileName;

    tm *ltm = localtime(&now);

    if (graph.isDirected()) {
        textFileName = "digraph_";
    } else {
        textFileName = "graph_";
    }

    // hora
    if (ltm->tm_hour < 10) textFileName += "0";
    textFileName += to_string(ltm->tm_hour);
    textFileName += ":";

    // minuto
    if (ltm->tm_min < 10) textFileName += "0";
    textFileName += to_string(ltm->tm_min);
    textFileName += ":";

    // segundo
    if (ltm->tm_sec < 10) textFileName += "0";
    textFileName += to_string(ltm->tm_sec);
    textFileName += "_";

    // dia
    if (ltm->tm_mday < 10) textFileName += "0";
    textFileName += to_string(ltm->tm_mday);
    textFileName += "-";

    // mes
    if (ltm->tm_mon < 10) textFileName += "0";
    textFileName += to_string(ltm->tm_mon);
    textFileName += "-";

    // ano
    textFileName += to_string(1900 + ltm->tm_year);

    // nome da imagem
    string imageName = textFileName;

    // formato
    textFileName += ".txt";

    output1.open("./text/" + textFileName, ios::trunc);
    output2.open("./text/last.txt", ios::trunc);

    buffer << "V = {";

    for (int i = 0; i < graph.getNumVertix(); i++) {
        buffer << graph.vertixAt(i);
        if (i != graph.getNumVertix() - 1) {
            buffer << ",";
        }
    }

    buffer << "}; A = {";

    for (int i = 0; i < graph.getNumEdges(); i++) {
        buffer << "(" << graph.edgeAt(i).vertix1 << "," << graph.edgeAt(i).vertix2 << ")";
        if (i != graph.getNumEdges() - 1) {
            buffer << ",";
        }
    }
    
    buffer << "};";

    getline(buffer, auxBuffer, '\n');
    output1 << auxBuffer;
    output2 << auxBuffer;

    output1.close();
    output2.close();
}