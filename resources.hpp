#pragma once
#include <string>
#include "graph.hpp"

using namespace std;

#ifdef _WIN32
    #define OPEN_IMAGE_COMMAND "(algum comando que funcione no windows) ./images/last.png"
#else
    #define OPEN_IMAGE_COMMAND "xdg-open ./images/last.png > /dev/null 2>&1 &"
#endif

void generateGraphDotFile(Graph graph);
void generateTextFile(Graph graph);