#include <iostream>
#include <fstream>
#include <exception>
#include "graph.hpp"
#include "resources.hpp"

using namespace std;

void representationsMenu(Graph graph);
void removalsAndInsertionsMenu(Graph& graph);
void backMenu();

int main(int argc, char *argv[]) {
    string filePath = "input.txt";
    bool directGraph = false;
    char option;

    // caso eu queira deixar a funcao de indicar o caminho do arquivo como parametro de inicializacao
    // if (argc != 2) {
    //     cout << "usage " << argv[0] << " /path/to/graph" << endl;
    //     return 1;
    // }
    // filePath = argv[1];

    cout << "Running program for directed graph?(Y/N)";
    cin >> option;

    if (option == 'Y' or option == 'y') directGraph = true;

    try {
        Graph graph(filePath, directGraph);

        do {
            system("clear || cls");
            cout << "menu" << endl << endl;
            cout << "1 - representations" << endl;
            cout << "2 - removals and insertions" << endl;
            cout << "0 - quit" << endl << endl;
            cout << "choose a option:";

            cin >> option;

            switch(option) {
                case '1': {
                    representationsMenu(graph);
                    break;
                }
                case '2': {
                    removalsAndInsertionsMenu(graph);
                    break;
                }
                default:
                    option = '0';
                    break;
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
    cout << "3 - graphic representation" << endl << endl;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    switch(option) {
        case 1: 
            graph.printAdjacencyMatrix();
            break;
        case 2: 
            graph.printAdjacencyList();
            break;
        case 3: 
            generateGraphDotFile(graph);
            break;
        default:
            break;
    }

    backMenu();
}

void removalsAndInsertionsMenu(Graph& graph) {
    int option;

    system("clear || cls");
    cout << "removals and insertions:" << endl << endl;
    cout << "1 - remove edge" << endl;
    cout << "2 - add edge" << endl;
    cout << "3 - remove vertix" << endl;
    cout << "4 - add vertix" << endl << endl;
    cout << "choose a option:";
    cin >> option;
    system("clear || cls");

    Edge edge;
    int vertix;

    switch(option) {
        case 1: {
            cout << "enter a edge vertix 1:";
            cin >> edge.vertix1;
            cout << "enter a edge vertix 2:";
            cin >> edge.vertix2;
            graph.removeEdge(edge);
            break;
        }
        case 2: {
            cout << "enter a edge vertix 1:";
            cin >> edge.vertix1;
            cout << "enter a edge vertix 2:";
            cin >> edge.vertix2;

            try {
                graph.addEdge(edge);
            } catch (exception e) {
                cout << "failed to add seleced edge" << endl;
            }
            break;
        }
        case 3: {
            cout << "enter a vertix to remove:";
            cin >> vertix;
            graph.removeVertix(vertix);
            break;
        }
        case 4: { 
            cout << "enter a vertix to add:";
            cin >> vertix;
            graph.addVertix(vertix);
            break;
        }
        default:
            break;
    }

    generateTextFile(graph);
    backMenu();
}

void backMenu() {
    string buffer;
    cout << endl << "type any key to back to menu:";
    cin.ignore(500, '\n');
    getline(cin, buffer, '\n');
}
