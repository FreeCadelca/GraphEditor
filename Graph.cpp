//
// Created by Dmitriy on 20.04.2024.
//

#include "Graph.h"

Graph* Graph::instance = nullptr;

Graph::Graph() = default;

Graph* Graph::getInstance() {
    if (instance == nullptr) {
        instance = new Graph();
    }
    return instance;
}

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
//
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

std::string Graph::getPrintoutAlgorithm() {
    return this->printoutAlgorithm;
}

void Graph::run_bfs(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и ребрах в графе
    if (this->coords.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата BFS
    std::stringstream result;
    result << "Обход в ширину начиная с вершины " << start_vertex << ":\n";
    result << "Посещенные вершины: ";

    // Очередь для обхода графа в ширину
    std::queue<char> q;
    // Множество для отслеживания посещенных вершин
    std::set<char> visited;

    // Начинаем обход с заданной начальной вершины
    q.push(start_vertex);
    visited.insert(start_vertex);

    // Обводка вершины и уход в слип
    Canvas::getInstance()->outline_vertex(start_vertex, Color(0.6, 1, 0.6, 0.5));
    usleep(100000);

    // Пока очередь не пуста, обходим граф в ширину
    while (!q.empty()) {
        char current_vertex = q.front();
        q.pop();

        // Обводка вершины и уход в слип
        Canvas::getInstance()->outline_vertex(current_vertex, Color(0.6, 1, 0.6, 0.5));
        usleep(100000);

        // Добавляем текущую вершину в результат обхода
        result << current_vertex << " ";

        // Получаем соседей текущей вершины из таблицы смежности
        auto neighbors = this->adjacent_list[current_vertex];
        // Для каждого соседа, если он еще не посещен, добавляем его в очередь и отмечаем как посещенный
        for (char neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);

                // Обводка вершины и уход в слип
                Canvas::getInstance()->outline_vertex(neighbor, Color(0.6, 1, 0.6, 0.5));
                usleep(100000);
            }
        }
    }

    // Выводим результат BFS в программное окно
    this->printoutAlgorithm = result.str();
}

void Graph::dfs_util(char vertex, std::set<char>& visited, std::stringstream& result) {
    // Помечаем текущую вершину как посещенную
    visited.insert(vertex);

    // Добавляем текущую вершину в результат
    result << vertex << " ";

    // Получаем соседей текущей вершины из таблицы смежности
    auto neighbors = this->adjacent_list[vertex];

    // Для каждого соседа, если он еще не посещен, рекурсивно запускаем обход в глубину
    for (char neighbor : neighbors) {
        if (visited.find(neighbor) == visited.end()) {
            dfs_util(neighbor, visited, result);
        }
    }
}

// Вспомогательная функция для рекурсивного обхода графа в глубину
void Graph::run_dfs(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и ребрах в графе
    if (coords.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата DFS
    std::stringstream result;
    result << "Обход в глубину начиная с вершины " << start_vertex << ":\n";
    result << "Посещенные вершины: ";

    // Множество для отслеживания посещенных вершин
    std::set<char> visited;

    // Запускаем рекурсивную функцию DFS для поиска в глубину
    dfs_util(start_vertex, visited, result);

    // Выводим результат DFS в программное окно
    this->printoutAlgorithm = result.str();
}

void Graph::runAlgorithm(const std::string& algorithm, char start_vertex) {
    if (algorithm == "BFS") {
        this->run_bfs(start_vertex);
    } else if (algorithm == "DFS") {
        this->run_dfs(start_vertex);
    } else {
        this->printoutAlgorithm = "Sorry, not avaible(";
    }
}