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
#include <stack>
#include <string>
#include <cstdlib>
#include <sstream>
#include "Color.h"
#include "Canvas.h"


class Graph {
private:
    static Graph* instance;
    Graph();

    // Структура для представления ребра
    struct Edge {
        char v_from;
        char v_to;
        int weight;

        Edge(char from, char to, int w) : v_from(from), v_to(to), weight(w) {}

        bool operator>(const Edge& other) const {
            return weight > other.weight;
        }

        bool operator<(const Edge& other) const {
            return weight < other.weight;
        }

        bool operator==(const Edge& other) const {
            return v_from == other.v_from && v_to == other.v_to && weight == other.weight;
        }
    };


    std::string printoutAlgorithm;

    void run_bfs(char start_vertex);

    void run_dfs(char start_vertex);

    void dijkstra(char start_vertex);

    void bellman_ford(char start_vertex);

    void kruskal(char start_vertex);

    char find(std::map<char, char> &parent, char vertex);

    void union_sets(std::map<char, char> &parent, char u, char v);

    void prim(char start_vertex);

public:
    std::map<char, std::vector<char>> adjacent_list; // список смежности
//    std::map<char, std::map<char, int>> adjacent_matrix; // матрица смежности
    std::vector<std::vector<int>> adjacent_matrix; // матрица смежности
//    std::vector<WeightedEdge> adjacent; // список рёбер
    const std::string TITLES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";//названия вершин
    int ID_NEXT_TITLE = 0;//номер следующей вершины для выбора
    std::map<char, std::pair<int, int>> coords; //координаты вершин
    int nextWeight = 1;

    static Graph *getInstance();

    void addVertex(double x, double y);

    void addEdge(char v_from, char v_to);

    std::string getPrintoutAdjList();

    std::string getPrintoutAdjMatrix();

    std::string getPrintoutAlgorithm();

    void runAlgorithm(const std::string& algorithm, char start_vertex = 'A');
};


#endif //GRAPHEDITOR_GRAPH_H
