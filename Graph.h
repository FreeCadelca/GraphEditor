//
// Created by Dmitriy on 20.04.2024.
//

#ifndef GRAPHEDITOR_GRAPH_H
#define GRAPHEDITOR_GRAPH_H

#include <map>
#include <vector>
#include <string>


class Graph {
private:
//    struct WeightedEdge {
//        char v_from;
//        char v_to;
//        int weight;
//    };

//    struct WeightedEdgeToVertex {
//        char v_to;
//        int weight;
//    };
public:
    std::map<char, std::vector<char>> adjacent_list; // список смежности
//    std::map<char, std::map<char, int>> adjacent_matrix; // матрица смежности
    std::vector<std::vector<int>> adjacent_matrix; // матрица смежности
//    std::vector<WeightedEdge> adjacent; // список рёбер
    const std::string TITLES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";//названия вершин
    int ID_NEXT_TITLE = 0;//номер следующей вершины для выбора
    std::map<char, std::pair<int, int>> coords; //координаты вершин
    int nextWeight = 1;

    Graph();
    void addVertex(double x, double y);
    void addEdge(char v_from, char v_to);
    std::string getPrintoutAdjList();
    std::string getPrintoutAdjMatrix();
//    std::string getPrintoutEdgeList();

};


#endif //GRAPHEDITOR_GRAPH_H
