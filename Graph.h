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


class GraphTestSuite;


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
        /// акназар
        bool operator!=(const Edge& other) const {
            return !(*this == other);
        }
        ///
    };


    std::string printoutAlgorithm;

    void run_bfs(char start_vertex);

    void run_dfs(char start_vertex);

    void dijkstra(char start_vertex, char end_vertex = '\n');

    void bellman_ford(char start_vertex, char end_vertex = '\n');

    void kruskal();

    char find(std::map<char, char> &parent, char vertex);

    void union_sets(std::map<char, char> &parent, char u, char v);

    void prim();

    friend class GraphTestSuite;
    friend class GraphTestAccessor;

public:
    std::map<char, std::vector<char>> adjacent_list; // Список смежности
//    std::map<char, std::map<char, int>> adjacent_matrix; // Матрица смежности
    std::vector<std::vector<int>> adjacent_matrix; // Матрица смежности
//    std::vector<WeightedEdge> adjacent; // Список рёбер
    const std::string TITLES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";// Названия вершин
    int ID_NEXT_TITLE = 0;// Номер следующей вершины для выбора
    std::map<char, std::pair<int, int>> coords; // Координаты вершин
    int nextWeight = 1;

    static Graph *getInstance();

    void addVertex(double x, double y);

    void addEdge(char v_from, char v_to, int weight = -1000000000);

    std::string getPrintoutAdjList();

    std::string getPrintoutAdjMatrix();

    std::string getPrintoutAlgorithm();

    void runAlgorithm(const std::string& algorithm, char start_vertex = '\n', char end_vertex = '\n');
};

class GraphTestAccessor {
public:
    static void run_bfs(Graph& graph, char start_vertex) {
        graph.run_bfs(start_vertex);
    }
    static void run_dfs(Graph& graph, char start_vertex) {
        graph.run_dfs(start_vertex);
    }
    static void dijkstra(Graph& graph, char start_vertex) {
        graph.dijkstra(start_vertex);
    }
    static void bellman_ford(Graph& graph, char start_vertex) {
        graph.bellman_ford(start_vertex);
    }
    static void kruskal(Graph& graph) {
        graph.kruskal();
    }
    static void prim(Graph& graph) {
        graph.prim();
    }
    static char find(Graph& graph, std::map<char, char> &parent, char vertex) {
        return graph.find(parent, vertex);
    }
    static void union_sets(Graph& graph, std::map<char, char> &parent, char u, char v) {
        graph.union_sets(parent, u, v);
    }
};


#endif //GRAPHEDITOR_GRAPH_H
