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

void removalsAndInsertionsMenu(Graph* graph);
void representationsMenu(Graph graph);
void verificationsMenu(Graph* graph);
void treesMenu(Graph graph);
void algorithmsMenu(Graph* graph);
void backMenu();


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
            cout << "menu" << endl << endl;
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

            if (option != '0') 
                backMenu();
        } while(option != '0');
    } catch (ifstream::failure fileError) {
        cerr << "failed to open/read/close the selected file." << endl;
        return 2;
    } catch (exception e) {
        cerr << "failed to parse data." << endl;
        return 3;
    } 

    cout << "exiting..." << endl;
    return 0;
}

void representationsMenu(Graph graph) {
    time_point<system_clock> start, end; 
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

    start = system_clock::now();
    switch(option) {
        case 1: 
            graph.printAdjacencyMatrix();
            break;
        case 2: 
            graph.printAdjacencyList();
            break;
        case 3: 
            generateGraphImage(graph, "fdp", "graph representation");
            break;
        case 4:
            graph.printFormatedData();
            break;
        default:
            break;
    }
    end = system_clock::now();

    duration<double> elapsed_seconds = end - start;
    cout << endl << "execution time: " << setprecision(5) << fixed << elapsed_seconds.count() << "s\n";
}

void removalsAndInsertionsMenu(Graph* graph) {
    time_point<system_clock> start, end; 
    int option;
    int vertex, source, destination;

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
            start = system_clock::now();
            graph->removeEdge(Edge(source, destination));
            end = system_clock::now();
            break;
        }
        case 2: {
            cout << "enter a source edge vertex:";
            cin >> source;
            cout << "enter a destination edge vertex:";
            cin >> destination;

            try {
                start = system_clock::now();
                graph->addEdge(Edge(source, destination));
                end = system_clock::now();
            } catch (exception e) {
                cout << "failed to add seleced edge" << endl;
            }
            break;
        }
        case 3: {
            cout << "enter a vertex to remove:";
            cin >> vertex;
            start = system_clock::now();
            graph->removeVertex(vertex);
            end = system_clock::now();
            break;
        }
        case 4: { 
            cout << "enter a vertex to add:";
            cin >> vertex;
            start = system_clock::now();
            graph->addVertex(vertex);
            end = system_clock::now();
            break;
        }
        default:
            break;
    }

    duration<double> elapsed_seconds = end - start;
    cout << endl << "execution time: " << setprecision(5) << fixed << elapsed_seconds.count() << "s\n";

    generateGraphText(*graph);
}

void verificationsMenu(Graph* graph) {
    time_point<system_clock> start, end;
    int option;
    int vertex;

    system("clear || cls");
    cout << "verifications:" << endl << endl;
    cout << "1 - number of vertices" << endl;
    cout << "2 - number of edges" << endl;
    cout << "3 - degree of a vertex" << endl;
    cout << "4 - is the graph connected?" << endl;
    if (graph->isDirected()) cout << "5 - is the graph strongly connected?" << endl;
    cout << "6 - does the graph have cycles?" << endl;
    cout << "7 - is the graph Eulerian?" << endl << endl;;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    start = system_clock::now();
    switch(option) {
        case 1: 
            cout << "num vertex: " << graph->getNumVertex() << endl;
            break;
        case 2: 
            cout << "num edges: " << graph->getNumEdges() << endl;
            break;
        case 3: { 
            cout << "enter a vertex: ";
            cin >> vertex;
            system("clear || cls");
            cout << "degree of vertex " << vertex << ": " << graph->getVertexDegree(vertex) << endl;
            break;
        }
        case 4: 
            graph->isConnected() ? cout << "true" << endl : cout << "false" << endl;
            break;
        case 5: { 
            if (!graph->isDirected()) {
                cout << "invalid function for type Graph, required type: Digraph" << endl;
                return;
            }
         
            Digraph* digraph = dynamic_cast<Digraph*>(graph);
            digraph->isStronglyConnected() ? cout << "true" << endl : cout << "false" << endl;
            break;
        }
        case 6: 
            graph->haveCycle() ? cout << "true" << endl : cout << "false" << endl;
            break;
        case 7: {
            int numOddVertexDegree = graph->getNumOddDegreeVertex();
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
    end = system_clock::now();

    duration<double> elapsed_seconds = end - start;
    cout << endl << "execution time: " << setprecision(5) << fixed << elapsed_seconds.count() << "s\n";
}

void treesMenu(Graph graph) {
    time_point<system_clock> start, end;
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

    start = system_clock::now();
    switch(option) {
        case 1: case 2: {
            List<Graph> treeList;

            if (option == 1) {
                treeList = graph.getDFSTree();
                title = "deep first search tree";
            } else {
                treeList = graph.getBFSTree();
                title = "breadth first search tree";
            }
            
            end = system_clock::now();

            Graph unifiedTree;         
            for (int i = 0; i < treeList.length(); i++) {
                Graph tree = treeList.at(i);

                for (int j = 0; j < tree.getNumVertex(); j++)
                    unifiedTree.addVertex(tree.vertexAt(j));

                for (int j = 0; j < tree.getNumEdges(); j++) 
                    if (!unifiedTree.hasEdge(tree.edgeAt(j)))
                        unifiedTree.addEdge(tree.edgeAt(j));
            }

            generateGraphImage(unifiedTree, "dot", title);
            break;
        }
        case 3: case 4: {
            Graph tree;

            if (!graph.isWeighted() or !graph.isConnected() or graph.isDirected()) {
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

            end = system_clock::now();
            generateGraphImage(tree, "fdp", title);
            break;
        }
        default:
            break;
    }

    duration<double> elapsed_seconds = end - start;
    cout << endl << "execution time: " << setprecision(5) << fixed << elapsed_seconds.count() << "s\n";
}

void algorithmsMenu(Graph* graph) {
    time_point<system_clock> start, end; 
    int option;

    system("clear || cls");
    cout << "algorithms:" << endl << endl;
    cout << "1 - kahn for topological sort" << endl;
    cout << "2 - DFS for topological sort" << endl;
    cout << "3 - Kosaraju for strongly component" << endl << endl;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    if (!graph->isDirected()) {
        cout << "the graph must be directed for use this function" << endl;  
        return;
    }

    Digraph* digraph = dynamic_cast<Digraph*>(graph);
    start = system_clock::now();
    
    switch(option) {
        case 1: case 2: {
            List<int> topList;

            if (digraph->haveCycle()) {
                cout << "the graph cannot contain a cycle" << endl;
                return;
            }
            
            option == 1 ? topList = digraph->getKahnTopologicalSort() : topList = digraph->getDFSTopologicalSort();      
            end = system_clock::now();

            Digraph digraph(topList);  
            for (int i = 0; i < topList.length() - 1; i++) 
                digraph.addEdge(Edge(topList.at(i), topList.at(i + 1)));
            
            topList.printList();
            generateGraphImage(digraph, "dot", "one of ways for topological sort");
            break;
        }
        case 3: {
            break;
        }
        default:
            break;
    }   
   
    duration<double> elapsed_seconds = end - start;
    cout << endl << "execution time: " << setprecision(5) << fixed << elapsed_seconds.count() << "s\n";
}

void backMenu() {
    string buffer;
    cout << endl << "type any key to back to menu:";
    cin.ignore(500, '\n');
    getline(cin, buffer, '\n');
}
