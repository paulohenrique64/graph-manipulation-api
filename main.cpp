#include <iostream>
#include <fstream>
#include <exception>
#include <iomanip>
#include <chrono>
#include <thread>
#include "graph.hpp"
#include "digraph.hpp"
#include "resources.hpp"

using namespace std;
using namespace chrono;

// chronometer variables
time_point<steady_clock> startTime, endTime; 

// functions prototypes
void removalsAndInsertionsMenu(Graph* graph);
void representationsMenu(Graph graph);
void verificationsMenu(Graph* graph);
void treesMenu(Graph graph);
void algorithmsMenu(Graph* graph);
void backMenu();
void printExecTime();


int main(int argc, char *argv[]) {
    string filePath = "input.txt"; 
    bool directGraph = false;
    char option;

    if (argc == 2) filePath = argv[1];  

    cout << "Running program for directed graph?(Y/N)";
    cin >> option;

    if (option == 'Y' or option == 'y') directGraph = true;

    try {
        Graph *graph = generateGraphFromFile(filePath, directGraph);

        do {
            system("clear || cls");
            cout << "Graph Manipulator" << endl << endl;
            cout << "1 - representations" << endl;
            cout << "2 - removals and insertions" << endl;
            cout << "3 - verifications" << endl;
            cout << "4 - search and minimal trees" << endl;
            cout << "5 - algorithms" << endl;
            cout << "0 - quit" << endl << endl;
            cout << "choose a option:";

            cin >> option;

            switch(option) {
                case '1': 
                    representationsMenu(*graph);
                    break;
                case '2': 
                    removalsAndInsertionsMenu(graph);
                    break;
                case '3':
                    verificationsMenu(graph);
                    break;
                case '4':
                    treesMenu(*graph);
                    break;
                case '5':
                    algorithmsMenu(graph);
                    break;
                default:
                    option = '0';
                    break;
            }

            if (option != '0') {
                // if the system is not a windows
                #ifndef _WIN32
                    printExecTime();                 
                #endif
                backMenu();
            } else {
                cout << endl << "saving changes..." << endl;
                generateGraphText(*graph, filePath);
                cout << "exiting..." << endl;
            }

        } while(option != '0');
    } catch (ifstream::failure fileError) {
        cerr << "failed to open/read/close the selected file." << endl;
        return 2;
    } catch (exception e) {
        cerr << "failed to parse data." << endl;
        return 3;
    } 

    return 0;
}

void representationsMenu(Graph graph) {
    int option;

    system("clear || cls");
    cout << "representations:" << endl << endl;
    cout << "1 - adjacency matrix" << endl;
    cout << "2 - adjacency list" << endl;
    cout << "3 - graphic representation" << endl;
    cout << "4 - debug" << endl << endl;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    startTime = steady_clock::now();
    switch(option) {
        case 1: {
            cout << "adjacency matrix" << endl << endl;
            graph.printAdjacencyMatrix();
            break;
        }
        case 2: {
            cout << "adjacency list" << endl << endl;
            graph.printAdjacencyList();
            break;
        }
        case 3: 
            generateGraphImage(graph, "fdp", "graph representation");
            break;
        case 4: {
            cout << "formated data" << endl << endl;
            graph.printFormatedData();
            break;
        }
        default:
            break;
    }
    endTime = steady_clock::now();
}

void removalsAndInsertionsMenu(Graph* graph) {
    int option, vertex, source, destination, weight;

    system("clear || cls");
    cout << "removals and insertions:" << endl << endl;
    cout << "1 - remove edge" << endl;
    cout << "2 - add edge" << endl;
    cout << "3 - remove vertex" << endl;
    cout << "4 - add vertex" << endl << endl;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    switch(option) {
        case 1: {
            cout << "enter a source edge vertex:";
            cin >> source;
            cout << "enter a destination edge vertex:";
            cin >> destination;

            Edge edge(source, destination);

            if (graph->weighted()) {
                cout << "enter a edge weight:";
                cin >> weight;
                edge.setWeight(weight);
            } 

            startTime = steady_clock::now();
            graph->removeEdge(edge);
            endTime = steady_clock::now();
            break;
        }
        case 2: {
            cout << "enter a source edge vertex:";
            cin >> source;
            cout << "enter a destination edge vertex:";
            cin >> destination;

            Edge edge(source, destination);

            if (graph->weighted()) {
                cout << "enter a edge weight:";
                cin >> weight;
                edge.setWeight(weight);
            } 

            try {
                startTime = steady_clock::now();
                graph->addEdge(edge);
            } catch (exception e) {
                cout << "failed to add seleced edge" << endl;
            }
            endTime = steady_clock::now();

            break;
        }
        case 3: {
            cout << "enter a vertex to remove:";
            cin >> vertex;
            startTime = steady_clock::now();
            graph->removeVertex(vertex);
            endTime = steady_clock::now();
            break;
        }
        case 4: { 
            cout << "enter a vertex to add:";
            cin >> vertex;
            startTime = steady_clock::now();
            graph->addVertex(vertex);
            endTime = steady_clock::now();
            break;
        }
        default:
            break;
    }
}

void verificationsMenu(Graph* graph) {
    int option, vertex;

    system("clear || cls");
    cout << "verifications:" << endl << endl;
    cout << "1 - number of vertices" << endl;
    cout << "2 - number of edges" << endl;
    cout << "3 - degree of a vertex" << endl;
    cout << "4 - is the graph connected?" << endl;
    if (graph->directed()) cout << "5 - is the graph strongly connected?" << endl;
    cout << "6 - does the graph have cycles?" << endl;
    cout << "7 - is the graph Eulerian?" << endl << endl;;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    startTime = steady_clock::now();
    switch(option) {
        case 1: {
            cout << "num vertex: " << graph->getNumVertex() << endl;
            endTime = steady_clock::now();
            break;
        }
        case 2: {
            cout << "num edges: " << graph->getNumEdges() << endl;
            endTime = steady_clock::now();
            break;
        }
        case 3: { 
            cout << "enter a vertex: ";
            cin >> vertex;
            system("clear || cls");
            cout << "degree of vertex " << vertex << ": " << graph->getVertexDegree(vertex) << endl;
            endTime = steady_clock::now();
            break;
        }
        case 4: {
            graph->connected() ? cout << "true" << endl : cout << "false" << endl;
            endTime = steady_clock::now();
            break;
        }
        case 5: { 
            if (!graph->directed()) {
                cout << "invalid function for type Graph, required type: Digraph" << endl;
                return;
            }
         
            Digraph* digraph = dynamic_cast<Digraph*>(graph);
            digraph->stronglyConnected() ? cout << "true" << endl : cout << "false" << endl;
            endTime = steady_clock::now();
            break;
        }
        case 6: {
            graph->haveCycle() ? cout << "true" << endl : cout << "false" << endl;
            endTime = steady_clock::now();
            break;
        }
        case 7: {
            int numOddVertexDegree = graph->getNumVertexOddDegree();
            endTime = steady_clock::now();

            cout  << "this graph is ";

            switch(numOddVertexDegree) {
                case 0:
                    cout << "eulerian" << endl;
                    break;
                case 2:
                    cout << "semi-eulerian" << endl;
                    break;
                default:
                    cout << "non-eulerian" << endl;
                    break;
            }

            break;
        }
        default:
            break;
    }
}

void treesMenu(Graph graph) {
    int option;
    string title;

    system("clear || cls");
    cout << "trees:" << endl << endl;
    cout << "1 - deep first search tree" << endl;
    cout << "2 - breadth first search tree" << endl;
    cout << "3 - kruskall tree" << endl;
    cout << "4 - prim tree" << endl << endl;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    startTime = steady_clock::now();
    switch(option) {
        case 1: case 2: {
            List<List<int>> orderLists;
            Digraph tree; 

            if (option == 1) {
                orderLists = graph.getDFSOrderLists();
                endTime = steady_clock::now();
                title = "deep first search";
            } else {
                orderLists = graph.getBFSOrderLists();
                endTime = steady_clock::now();
                title = "breadth first search";
            }

            // printing order search lists
            cout << title << endl << endl;
            for (int i = 0; i < orderLists.size(); i++) 
                orderLists[i].printList();
            cout << endl;
            
            // generating tree from order search lists
            for (int i = 0; i < orderLists.size(); i++) {
                for (int j = 0; j < orderLists[i].size(); j++) 
                    tree.addVertex(orderLists[i][j]);

                for (int j = 0; j < orderLists[i].size() - 1; j++) 
                    tree.addEdge(Edge(orderLists[i][j], orderLists[i][j+1]));
            }

            generateGraphImage(tree, "dot", title);
            break;
        }
        case 3: case 4: {
            Graph tree;

            if (!graph.weighted() or !graph.connected() or graph.directed()) {
                cout << "the graph must be weighted, connected and non-directed for use this function" << endl;
                return;
            }

            if (option == 3) {
                tree = graph.getKruskalTree();
                title = "kruskall tree";
            } else {
                tree = graph.getPrimTree();
                title = "prim tree";
            }

            endTime = steady_clock::now();
            generateGraphImage(tree, "fdp", title);
            break;
        }
        default:
            break;
    }
}

void algorithmsMenu(Graph* graph) {
    int option;

    system("clear || cls");
    cout << "algorithms:" << endl << endl;
    cout << "1 - kahn for topological sort" << endl;
    cout << "2 - DFS for topological sort" << endl;
    cout << "3 - Kosaraju for strongly component" << endl << endl;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    if (!graph->directed()) {
        cout << "the graph must be directed for use this function" << endl;  
        return;
    }

    Digraph* digraph = dynamic_cast<Digraph*>(graph);
     
    startTime = steady_clock::now();
    switch(option) {
        case 1: case 2: {
            List<int> topList;
            string title;

            if (digraph->haveCycle()) {
                cout << "the graph cannot contain a cycle" << endl;
                return;
            }

            if (option == 1) {
                topList = digraph->getKahnTopologicalSort();
                endTime = steady_clock::now();
                title = "kahn topological sort";
            } else {
                topList = digraph->getDFSTopologicalSort();     
                endTime = steady_clock::now();
                title = "DFS topological sort";
            }

            // generating tree from topological order list
            Digraph digraph(topList);  
            for (int i = 0; i < topList.size() - 1; i++) 
                digraph.addEdge(Edge(topList[i], topList[i + 1]));
            
            // printing topological search list
            cout << title << endl << endl;
            topList.printList();
            cout << endl;

            generateGraphImage(digraph, "dot", title);
            break;
        }
        case 3: {
            List<List<int>> components = digraph->getKosarajuComponents();
            endTime = steady_clock::now();
            string title = "strongly components";

            // printing strongly components
            cout << title << endl << endl;
            for (int i = 0; i < components.size(); i++) {
                components[i].printList();
            }
            cout << endl;

            generateGraphImage(*digraph, "fdp", title, &components);
            break;
        }
        default: 
            break;
    } 
}

void backMenu() {
    string buffer;
    cout << endl << "type any key to back to menu:";
    cin.ignore(500, '\n');
    getline(cin, buffer, '\n');
}

void printExecTime() {
    duration<double> elapsedSeconds = endTime - startTime;
    cout << endl << "execution time: " << setprecision(8) << fixed << elapsedSeconds.count() << "s\n";
}
