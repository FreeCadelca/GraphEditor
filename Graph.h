//
// Created by Dmitriy on 20.04.2024.
//

#ifndef GRAPHEDITOR_GRAPH_H
#define GRAPHEDITOR_GRAPH_H

#include <unistd.h>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <cstdlib>
#include <sstream>
#include "Color.h"
#include "Canvas.h"


class Graph {
private:
    static Graph* instance;
    Graph();
//    struct WeightedEdge {
//        char v_from;
//        char v_to;
//        int weight;
//    };

//    struct WeightedEdgeToVertex {
//        char v_to;
//        int weight;
//    };
//
    std::string printoutAlgorithm;
    void run_bfs(char start_vertex);
    void dfs_util(char vertex, std::set<char>& visited, std::stringstream& result);
    void run_dfs(char start_vertex);
//    void (*outline_vertex)(char vertex, Color outline_color);
public:
    std::map<char, std::vector<char>> adjacent_list; // список смежности
//    std::map<char, std::map<char, int>> adjacent_matrix; // матрица смежности
    std::vector<std::vector<int>> adjacent_matrix; // матрица смежности
//    std::vector<WeightedEdge> adjacent; // список рёбер
    const std::string TITLES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";//названия вершин
    int ID_NEXT_TITLE = 0;//номер следующей вершины для выбора
    std::map<char, std::pair<int, int>> coords; //координаты вершин
    int nextWeight = 1;

    static Graph* getInstance();
    void addVertex(double x, double y);
    void addEdge(char v_from, char v_to);
    std::string getPrintoutAdjList();
    std::string getPrintoutAdjMatrix();
//    std::string getPrintoutEdgeList();
    std::string getPrintoutAlgorithm();
    void runAlgorithm(const std::string& algorithm);
};


#endif //GRAPHEDITOR_GRAPH_H
