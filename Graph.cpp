//
// Created by Dmitriy on 20.04.2024.
//

#include "Graph.h"

Graph::Graph() = default;


void Graph::addVertex(double x, double y) {
    this->coords[TITLES[ID_NEXT_TITLE]] = {x, y};//добавляем новую вершину туда, куда нужно

    this->adjacent_list[TITLES[ID_NEXT_TITLE]] = {};

//    this->adjacent_matrix[TITLES[ID_NEXT_TITLE]] = std::map<char, int> ();
    if (ID_NEXT_TITLE == 0) {
        this->adjacent_matrix = std::vector<std::vector<int>>(1, std::vector<int>(1, 0));
    } else {
        adjacent_matrix.resize(
                adjacent_matrix.size() + 1,
                std::vector<int>(adjacent_matrix.size(), 0)
        );
        for (auto &i: adjacent_matrix) {
            i.resize(i.size() + 1, 0);
        }
    }
    ID_NEXT_TITLE++;
}

void Graph::addEdge(char v_from, char v_to) {
    this->adjacent_list[v_from].push_back(v_to);
// A = 65
    int index_v_from = (int) v_from - 65;
    int index_v_to = (int) v_to - 65;
    this->adjacent_matrix[index_v_from][index_v_to] = this->nextWeight;
}

std::string Graph::getPrintoutAdjList() {
    std::string output;
    int count_vertexes = (int) this->adjacent_list.size();
    for (auto i: this->adjacent_list) {
        char vertex = i.first;
        output.push_back(vertex);
        output += ":";
        for (auto edge: i.second) {
            output += " ";
            output.push_back(edge);
        }
        if (i.second.empty()) {
            output += " None";
        }
        output += ";\n";
    }
    return output;
}

std::string Graph::getPrintoutAdjMatrix() {
    int count_vertexes = (int) this->adjacent_list.size();
    std::string output = "    ";
    for (int i = 0; i < count_vertexes; i++) {
        output += this->TITLES[i];
        output += ' ';
    }
    output += '\n';
    for (int i = 0; i < count_vertexes; i++) {
        output += this->TITLES[i];
        output += ": ";
        for (auto j : this->adjacent_matrix[i]) {
            output += std::to_string(j) + " ";
        }
        output += '\n';
    }
    return output;
}
